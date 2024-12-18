#include "gaits.h"
#include <cmath>
#include <esp_log.h>
#include "hexa_params.h"

namespace hexapod
{
    // Constructor Gaits
    Gaits::Gaits(GaitType gait, RobotType robot_type) :
        Xspeed(0.0f), Yspeed(0.0f), Rspeed(0.0f), liftHeight(45.0f),
        Xspeed_max(0.0f), Yspeed_max(0.0f), Rspeed_max(0.0f),
        num_legs_(robot_type == HEXAPOD ? 6 : 4),
        stepsInCycle(0), pushSteps(0), desfase(0),
        tranTime_ms(0), tranTime_s(0), cycleTime(0.0),
        gaitleg_order{0}, gait_step(1), leg_step(0),
        robot_type_(robot_type), current_gait(NUM_MAX_GAITS)
    {
        gait_select(gait);
        init_tgaits();
        ESP_LOGI(TAG_GAIT, "Gait object created for %s",
                 robot_type_ == HEXAPOD ? "Hexapod" : "Quadruped");
    }

    void Gaits::init_tgaits(void)
    {
        for (int leg = 0; leg < num_legs_; leg++)
        {
            tgait[leg].reset();
        }
        
        gait_step = 1;
    }

    void Gaits::gait_select(GaitType new_gait)
    {
        if (new_gait == current_gait) return;

        current_gait = new_gait;
        liftHeight = 45;

        setLegOrderByGait(current_gait);
        update_gait_params(current_gait);

        cycleTime = (stepsInCycle * static_cast<float>(tranTime_ms)) / 1000.0f;
        gait_step = 1;

        update_velocities();
    }

    //void Gaits::setLegOrderByGait(GaitType GaitType)
    //{
    //    static const uint8_t hexapod_orders[][6] = {
    //        {LEG_FIRST, LEG_FIFTH, LEG_THIRD, LEG_FOURTH, LEG_SECOND, LEG_SIXTH},  // RIPPLE
    //        {LEG_FIRST, LEG_SECOND, LEG_FIRST, LEG_SECOND, LEG_FIRST, LEG_SECOND}, // TRIPOD
    //        {LEG_FIRST, LEG_SECOND, LEG_THIRD, LEG_FOURTH, LEG_FIFTH, LEG_SIXTH}   // WAVE
    //    };
//
    //    static const uint8_t quadruped_orders[][4] = {
    //        {LEG_FIRST, LEG_SECOND, LEG_THIRD, LEG_FOURTH},  // WALK
    //        {LEG_FIRST, LEG_FIRST, LEG_SECOND, LEG_SECOND}   // TROT
    //    };
//
    //    const uint8_t *order = (robot_type_ == HEXAPOD) 
    //        ? hexapod_orders[GaitType]
    //        : quadruped_orders[GaitType];
//
    //    for (uint8_t i = 0; i < num_legs_; ++i)
    //    {
    //        gaitleg_order[i] = order[i];
    //    }
    //}

	// Nuevo método para configurar el orden de las patas según el gait
	void Gaits::setLegOrderByGait(GaitType GaitType)
	{
		if (robot_type_ == HEXAPOD)
		{
			setHexapodLegOrder(GaitType);
		}
		else if (robot_type_ == QUADRUPED)
		{
			setQuadrupedLegOrder(GaitType);
		}
		else
		{
			ESP_LOGE(TAG_GAIT, "Unsupported RobotType: %d", robot_type_);
		}
	}

	void Gaits::setHexapodLegOrder(GaitType GaitType)
	{
		switch (GaitType)
		{
			case RIPPLE_6:
			case RIPPLE_12:
			case RIPPLE_24:
				gaitleg_order[LEFT_FRONT]  = LEG_FIRST;
				gaitleg_order[LEFT_MIDDLE] = LEG_FIFTH;
				gaitleg_order[LEFT_REAR]   = LEG_THIRD;
				gaitleg_order[RIGHT_FRONT] = LEG_FOURTH;
				gaitleg_order[RIGHT_MIDDLE]= LEG_SECOND;
				gaitleg_order[RIGHT_REAR]  = LEG_SIXTH;
				break;

			case TRIPOD_6:
			case TRIPOD_12:
			case TRIPOD_24:
				gaitleg_order[LEFT_FRONT]  = LEG_FIRST;   // RIGHT_FRONT
				gaitleg_order[LEFT_MIDDLE] = LEG_SECOND;  // RIGHT_REAR
				gaitleg_order[LEFT_REAR]   = LEG_FIRST;   // LEFT_FRONT
				gaitleg_order[RIGHT_FRONT] = LEG_SECOND;  // LEFT_REAR
				gaitleg_order[RIGHT_MIDDLE]= LEG_FIRST;   // LEFT_MIDDLE
				gaitleg_order[RIGHT_REAR]  = LEG_SECOND;  // RIGHT_MIDDLE
				break;

			case WAVE_12:
			case WAVE_24:
				gaitleg_order[LEFT_FRONT]  = LEG_FIRST;   // RIGHT_FRONT
				gaitleg_order[LEFT_MIDDLE] = LEG_SECOND;  // RIGHT_REAR
				gaitleg_order[LEFT_REAR]   = LEG_THIRD;   // LEFT_FRONT
				gaitleg_order[RIGHT_FRONT] = LEG_FOURTH;  // LEFT_REAR
				gaitleg_order[RIGHT_MIDDLE]= LEG_FIFTH;   // RIGHT_MIDDLE
				gaitleg_order[RIGHT_REAR]  = LEG_SIXTH;   // LEFT_MIDDLE
				break;

			default:
				ESP_LOGW(TAG_GAIT, "Unsupported GaitType for Hexapod: %d", GaitType);
				break;
		}
	}

	void Gaits::setQuadrupedLegOrder(GaitType GaitType)
	{
		switch (GaitType)
		{
			//case WALK:
			//	gaitleg_order[LEFT_FRONT]  = LEG_FIRST;
			//	gaitleg_order[LEFT_REAR]   = LEG_THIRD;
			//	gaitleg_order[RIGHT_FRONT] = LEG_SECOND;
			//	gaitleg_order[RIGHT_REAR]  = LEG_FOURTH;
			//	break;
//
			//case TROT:
			//	gaitleg_order[LEFT_FRONT]  = LEG_FIRST;
			//	gaitleg_order[LEFT_REAR]   = LEG_FIRST;
			//	gaitleg_order[RIGHT_FRONT] = LEG_SECOND;
			//	gaitleg_order[RIGHT_REAR]  = LEG_SECOND;
			//	break;

			default:
				ESP_LOGW(TAG_GAIT, "Unsupported GaitType for Quadruped: %d", GaitType);
				break;
		}
	}

    void Gaits::update_gait_params(GaitType type)
	{
		if (robot_type_ == HEXAPOD)
    {
        switch (type)
        {
				case RIPPLE_6: 
					pushSteps = 4;
					stepsInCycle = 6;
					desfase = 1;
					tranTime_ms = 140;
                    tranTime_s = tranTime_ms / 1000.0f;
					break;

				case RIPPLE_12:
					pushSteps = 8;
					stepsInCycle = 12;
					desfase = 2;
					tranTime_ms = 120;
                    tranTime_s = tranTime_ms / 1000.0f;
					break;

				case RIPPLE_24:
					pushSteps = 12;
					stepsInCycle = 18;
					desfase = 3;
					tranTime_ms = 120;
                    tranTime_s = tranTime_ms / 1000.0f;
					break;

				case TRIPOD_6:
					pushSteps = 2;
					stepsInCycle = 4;
					desfase = 2;
					tranTime_ms = 140;
                    tranTime_s = tranTime_ms / 1000.0f;
					break;

				case TRIPOD_12:
					pushSteps = 4;
					stepsInCycle = 8;
					desfase = 4;
					tranTime_ms = 120;
                    tranTime_s = tranTime_ms / 1000.0f;
					break;

				case TRIPOD_24:
					pushSteps = 6;
					stepsInCycle = 12;
					desfase = 6;
					tranTime_ms = 120;
                    tranTime_s = tranTime_ms / 1000.0f;
					break;

				case WAVE_12:
					pushSteps = 10;
					stepsInCycle = 12;
					desfase = 2;
					tranTime_ms = 80;
                    tranTime_s = tranTime_ms / 1000.0f;
					break;

				case WAVE_24:
					pushSteps = 20;
					stepsInCycle = 24;
					desfase = 4;
					tranTime_ms = 80;
                    tranTime_s = tranTime_ms / 1000.0f;
					break;

				default:
					break;
        }
    }
	    else if (robot_type_ == QUADRUPED)
		{
		}
	}

	/**
	 * @brief 
	 * @details
	 *                            Distancia recorrida (mm)
	 *	 Veloc_max =   ----------------------------------------------
	*                      (tranTime_s(s) * pushsteps
	*/
    void Gaits::update_velocities(void)
    {
        float time_factor = tranTime_s * pushSteps;

        Xspeed_max = 60.0f / time_factor;
        Yspeed_max = 80.0f / time_factor;
        Rspeed_max = (M_PI / 7.0f) / time_factor;

        ESP_LOGI(TAG_GAIT, "Max speeds updated: X=%.2f, Y=%.2f, R=%.2f", Xspeed_max, Yspeed_max, Rspeed_max);
    }

	void Gaits::configureBody()
	{
		tranTime_ms = 140;
	}

	uint16_t Gaits::get_gait_transition_time(void)
	{
		return tranTime_ms;
	}

    transformations3D::Tmatrix Gaits::step(uint8_t leg)
    {
        leg_step = gait_step - (gaitleg_order[leg] - 1) * desfase;
		if (leg_step < 0)	leg_step=stepsInCycle + leg_step;
        if (leg_step == 0) leg_step = stepsInCycle;

        handle_step_phase(leg, leg_step);

        return tgait[leg];
    }

    void Gaits::handle_step_phase(uint8_t leg, int8_t leg_step)
	{
		if ( ( current_gait == RIPPLE_6) || ( current_gait == TRIPOD_6 ) || ( current_gait == WAVE_12 ) ) 	// ### 3 ETAPAS ###
		{
			handle_step_3_stages(leg, leg_step);
		}
		else if ( ( current_gait == RIPPLE_12 ) || ( current_gait == WAVE_24 ) || ( current_gait == TRIPOD_12 ) )	//	### 5 ETAPAS ###
		{
			handle_step_5_stages(leg, leg_step);
		}
		else if ( ( current_gait == RIPPLE_24 ) || ( current_gait == TRIPOD_24 ) ) //	### 7 ETAPAS ###
		{
			handle_step_7_stages(leg, leg_step);
		}
	}

    float Gaits::actual_speed_percentage(void)
    {
        return ( fabs(Yspeed) > fabs(Xspeed) ) ? (fabs(Yspeed) / Yspeed_max) : (fabs(Xspeed) / Xspeed_max);
    }

    void Gaits::handle_step_3_stages(uint8_t leg, int8_t leg_step)
    {
        if (leg_step == 1) // UP
        {
            tgait[leg].t_x = 0.0f;
            tgait[leg].t_y = 0.0f;
            tgait[leg].t_z = liftHeight;// * actual_speed_percentage();
            tgait[leg].rot_z = 0.0f;
        }
        else if (leg_step == 2) // DOWN
        {
            tgait[leg].t_x = (Xspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
            tgait[leg].t_y = (Yspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
            tgait[leg].t_z = 0.0f;
            tgait[leg].rot_z = (Rspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
        }
        else // MOVE BODY FORWARD
        {
            tgait[leg].t_x -= Xspeed * tranTime_s;
            tgait[leg].t_y -= Yspeed * tranTime_s;
            tgait[leg].t_z = 0.0f;
            tgait[leg].rot_z -= Rspeed * tranTime_s;
        }
    }

    void Gaits::handle_step_5_stages(uint8_t leg, int8_t leg_step)
    {
        if (leg_step == stepsInCycle) // UP/2
        {
            tgait[leg].t_x = tgait[leg].t_x / 2.0f;
            tgait[leg].t_y = tgait[leg].t_y / 2.0f;
            tgait[leg].t_z = liftHeight / 2.0f;
            tgait[leg].rot_z = tgait[leg].rot_z / 2.0f;
        }
        else if (leg_step == 1) // UP
        {
            tgait[leg].t_x = 0.0f;
            tgait[leg].t_y = 0.0f;
            tgait[leg].t_z = liftHeight;
            tgait[leg].rot_z = 0.0f;
        }
        else if (leg_step == 2) // DOWN/2
        {
            tgait[leg].t_x = (Xspeed * cycleTime * pushSteps) / (4.0f * stepsInCycle);
            tgait[leg].t_y = (Yspeed * cycleTime * pushSteps) / (4.0f * stepsInCycle);
            tgait[leg].t_z = liftHeight / 2.0f;
            tgait[leg].rot_z = (Rspeed * cycleTime * pushSteps) / (4.0f * stepsInCycle);
        }
        else if (leg_step == 3) // DOWN
        {
            tgait[leg].t_x = (Xspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
            tgait[leg].t_y = (Yspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
            tgait[leg].t_z = 0.0f;
            tgait[leg].rot_z = (Rspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
        }
        else // MOVE BODY FORWARD
        {
            tgait[leg].t_x -= Xspeed * tranTime_s;
            tgait[leg].t_y -= Yspeed * tranTime_s;
            tgait[leg].t_z = 0.0f;
            tgait[leg].rot_z -= Rspeed * tranTime_s;
        }
    }

    void Gaits::handle_step_7_stages(uint8_t leg, int8_t leg_step)
    {
        if (leg_step == stepsInCycle - 1) // UP 1/3
        {
            tgait[leg].t_x = tgait[leg].t_x / 3.0f;
            tgait[leg].t_y = tgait[leg].t_y / 3.0f;
            tgait[leg].t_z = liftHeight / 3.0f;
            tgait[leg].rot_z = tgait[leg].rot_z / 3.0f;
        }
        else if (leg_step == stepsInCycle) // UP 2/3
        {
            tgait[leg].t_x = tgait[leg].t_x * 2.0f / 3.0f;
            tgait[leg].t_y = tgait[leg].t_y * 2.0f / 3.0f;
            tgait[leg].t_z = liftHeight * 2.0f / 3.0f;
            tgait[leg].rot_z = tgait[leg].rot_z * 2.0f / 3.0f;
        }
        else if (leg_step == 1) // UP
        {
            tgait[leg].t_x = 0.0f;
            tgait[leg].t_y = 0.0f;
            tgait[leg].t_z = liftHeight;
            tgait[leg].rot_z = 0.0f;
        }
        else if (leg_step == 2) // DOWN 1/3
        {
            tgait[leg].t_x = (Xspeed * cycleTime * pushSteps) / (3.0f * stepsInCycle);
            tgait[leg].t_y = (Yspeed * cycleTime * pushSteps) / (3.0f * stepsInCycle);
            tgait[leg].t_z = liftHeight * 2.0f / 3.0f;
            tgait[leg].rot_z = (Rspeed * cycleTime * pushSteps) / (3.0f * stepsInCycle);
        }
        else if (leg_step == 3) // DOWN 2/3
        {
            tgait[leg].t_x = (Xspeed * cycleTime * pushSteps) * 2.0f / (3.0f * stepsInCycle);
            tgait[leg].t_y = (Yspeed * cycleTime * pushSteps) * 2.0f / (3.0f * stepsInCycle);
            tgait[leg].t_z = liftHeight / 3.0f;
            tgait[leg].rot_z = (Rspeed * cycleTime * pushSteps) * 2.0f / (3.0f * stepsInCycle);
        }
        else if (leg_step == 4) // DOWN
        {
            tgait[leg].t_x = (Xspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
            tgait[leg].t_y = (Yspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
            tgait[leg].t_z = 0.0f;
            tgait[leg].rot_z = (Rspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
        }
        else // MOVE BODY FORWARD
        {
            tgait[leg].t_x -= Xspeed * tranTime_s;
            tgait[leg].t_y -= Yspeed * tranTime_s;
            tgait[leg].t_z = 0.0f;
            tgait[leg].rot_z -= Rspeed * tranTime_s;
        }
    }

	void Gaits::next_step( void )
    {
        gait_step++;

        if ( gait_step > stepsInCycle )
        {
            gait_step = 1;
        }
    }

        float Gaits::map(int16_t value, int16_t in_min, int16_t in_max, float out_min, float out_max)
    {
        return out_min + (static_cast<float>(value - in_min) * (out_max - out_min)) / (in_max - in_min);
    }

    void Gaits::set_xspeed(int16_t xspeed)
    {
        if ( xspeed == 0 )
        {
            Xspeed = 0.0f;
            return;
        }

        Xspeed = map(xspeed, INT16_MIN, INT16_MAX, -Xspeed_max, Xspeed_max);
    }

    void Gaits::set_yspeed(int16_t yspeed)
    {
        if ( yspeed == 0 )
        {
            Yspeed = 0.0f;
            return;
        }

        Yspeed = map(yspeed, INT16_MIN, INT16_MAX, -Yspeed_max, Yspeed_max);
    }

    void Gaits::set_rspeed(int16_t rspeed)
    {
        if ( rspeed == 0 )
        {
            Rspeed = 0.0f;
            return;
        }

        Rspeed = map(rspeed, INT16_MIN, INT16_MAX, -Rspeed_max, Rspeed_max); // Rspeed sigue siendo float
    }

	bool Gaits::isMoving() // refactorizar, lo veo obsoleto, hay otras maneras mejores de hacerlo
	{
		return  ( Xspeed > 1.50f || Xspeed < -1.50f ) || 
				( Yspeed > 1.50f || Yspeed < -1.50f ) || 
				( Rspeed > 0.05f || Rspeed < -0.05f );
	}

}
