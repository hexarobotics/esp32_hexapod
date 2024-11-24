#include "gaits.h"
#include <cmath>
#include <esp_log.h>
#include "hexa_params.h"

namespace hexapod
{
    // Constructor Gaits
    Gaits::Gaits(GaitType gait, RobotType robot_type) :
        Xspeed(0.0), Yspeed(0.0), Rspeed(0.0), liftHeight(45),
        num_legs_(robot_type == HEXAPOD ? 6 : 4),
        stepsInCycle(0), pushSteps(0), desfase(0),
        tranTime(0), cycleTime(0.0),
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

        cycleTime = (stepsInCycle * static_cast<float>(tranTime)) / 1000.0f;
        gait_step = 1;

        update_velocities();
    }

    void Gaits::setLegOrderByGait(GaitType GaitType)
    {
        static const uint8_t hexapod_orders[][6] = {
            {LEG_FIRST, LEG_FIFTH, LEG_THIRD, LEG_FOURTH, LEG_SECOND, LEG_SIXTH},  // RIPPLE
            {LEG_FIRST, LEG_SECOND, LEG_FIRST, LEG_SECOND, LEG_FIRST, LEG_SECOND}, // TRIPOD
            {LEG_FIRST, LEG_SECOND, LEG_THIRD, LEG_FOURTH, LEG_FIFTH, LEG_SIXTH}   // WAVE
        };

        static const uint8_t quadruped_orders[][4] = {
            {LEG_FIRST, LEG_SECOND, LEG_THIRD, LEG_FOURTH},  // WALK
            {LEG_FIRST, LEG_FIRST, LEG_SECOND, LEG_SECOND}   // TROT
        };

        const uint8_t *order = (robot_type_ == HEXAPOD) 
            ? hexapod_orders[GaitType]
            : quadruped_orders[GaitType];

        for (uint8_t i = 0; i < num_legs_; ++i)
        {
            gaitleg_order[i] = order[i];
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
					tranTime = 140;
					break;

				case RIPPLE_12:
					pushSteps = 8;
					stepsInCycle = 12;
					desfase = 2;
					tranTime = 120;
					break;

				case RIPPLE_24:
					pushSteps = 12;
					stepsInCycle = 18;
					desfase = 3;
					tranTime = 120;
					break;

				case TRIPOD_6:
					pushSteps = 2;
					stepsInCycle = 4;
					desfase = 2;
					tranTime = 140;
					break;

				case TRIPOD_12:
					pushSteps = 4;
					stepsInCycle = 8;
					desfase = 4;
					tranTime = 120;
					break;

				case TRIPOD_24:
					pushSteps = 6;
					stepsInCycle = 12;
					desfase = 6;
					tranTime = 120;
					break;

				case WAVE_12:
					pushSteps = 10;
					stepsInCycle = 12;
					desfase = 2;
					tranTime = 80;
					break;

				case WAVE_24:
					pushSteps = 20;
					stepsInCycle = 24;
					desfase = 4;
					tranTime = 80;
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
	*                      (trantime(ms) / conv_ms_to_s) * pushsteps
	*/
    void Gaits::update_velocities(void)
    {
        float time_factor = static_cast<float>(tranTime) / 1000.0f * pushSteps;

        Xspeed_max = 60.0f / time_factor;
        Yspeed_max = 70.0f / time_factor;
        Rspeed_max = (M_PI / 7.0f) / time_factor;

        ESP_LOGI(TAG_GAIT, "Max speeds updated: X=%.2f, Y=%.2f, R=%.2f", Xspeed_max, Yspeed_max, Rspeed_max);
    }

	void Gaits::configureBody()
	{
		tranTime = 140;
	}

	uint16_t Gaits::get_gait_transition_time(void)
	{
		return tranTime;
	}

	transformations3D::Tmatrix Gaits::step(uint8_t leg)
    {
        leg_step = gait_step - (gaitleg_order[leg] - 1) * desfase;
        if (leg_step < 0) leg_step += stepsInCycle;
        if (leg_step == 0) leg_step = stepsInCycle;

        float x_move = (Xspeed * cycleTime * pushSteps) / stepsInCycle;
        float y_move = (Yspeed * cycleTime * pushSteps) / stepsInCycle;
        float r_move = (Rspeed * cycleTime * pushSteps) / stepsInCycle;

        handle_step_phase(leg, leg_step, x_move, y_move, r_move);

        return tgait[leg];
    }

	void Gaits::handle_step_phase(uint8_t leg, int8_t leg_step, float x_move, float y_move, float r_move)
	{
		if ( ( current_gait == RIPPLE_6) || ( current_gait == TRIPOD_6 ) || ( current_gait == WAVE_12 ) ) 	// ### 3 ETAPAS ###
		{
			handle_step_6(leg, leg_step, x_move, y_move, r_move);
		}
		else if ( ( current_gait == RIPPLE_12 ) || ( current_gait == WAVE_24 ) || ( current_gait == TRIPOD_12 ) )	//	### 5 ETAPAS ###
		{
			handle_step_12(leg, leg_step, x_move, y_move, r_move);
		}
		else if ( ( current_gait == RIPPLE_24 ) || ( current_gait == TRIPOD_24 ) ) //	### 7 ETAPAS ###
		{
			handle_step_24(leg, leg_step, x_move, y_move, r_move);
		}
	}

	void Gaits::handle_step_6(uint8_t leg, int8_t leg_step, float x_move, float y_move, float r_move)
	{
		if (leg_step == 1) // UP
		{
			tgait[leg].t_x = 0.0f;
			tgait[leg].t_y = 0.0f;
			tgait[leg].t_z = liftHeight;
			tgait[leg].rot_z = 0.0f;
		}
		else if (leg_step == 2) // DOWN
		{
			tgait[leg].t_x = x_move / 2.0f;
			tgait[leg].t_y = y_move / 2.0f;
			tgait[leg].t_z = 0.0f;
			tgait[leg].rot_z = r_move / 2.0f;
		}
		else // MOVE BODY FORWARD
		{
			tgait[leg].t_x -= x_move;
			tgait[leg].t_y -= y_move;
			tgait[leg].t_z = 0.0f;
			tgait[leg].rot_z -= r_move;
		}
	}

	void Gaits::handle_step_12(uint8_t leg, int8_t leg_step, float x_move, float y_move, float r_move)
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
			tgait[leg].t_x = x_move / 4.0f;
			tgait[leg].t_y = y_move / 4.0f;
			tgait[leg].t_z = liftHeight / 2.0f;
			tgait[leg].rot_z = r_move / 4.0f;
		}
		else if (leg_step == 3) // DOWN
		{
			tgait[leg].t_x = x_move / 2.0f;
			tgait[leg].t_y = y_move / 2.0f;
			tgait[leg].t_z = 0.0f;
			tgait[leg].rot_z = r_move / 2.0f;
		}
		else // MOVE BODY FORWARD
		{
			tgait[leg].t_x -= x_move;
			tgait[leg].t_y -= y_move;
			tgait[leg].t_z = 0.0f;
			tgait[leg].rot_z -= r_move;
		}
	}

	void Gaits::handle_step_24(uint8_t leg, int8_t leg_step, float x_move, float y_move, float r_move)
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
			tgait[leg].t_x = x_move / 3.0f;
			tgait[leg].t_y = y_move / 3.0f;
			tgait[leg].t_z = liftHeight * 2.0f / 3.0f;
			tgait[leg].rot_z = r_move / 3.0f;
		}
		else if (leg_step == 3) // DOWN 2/3
		{
			tgait[leg].t_x = 2.0f * x_move / 3.0f;
			tgait[leg].t_y = 2.0f * y_move / 3.0f;
			tgait[leg].t_z = liftHeight / 3.0f;
			tgait[leg].rot_z = 2.0f * r_move / 3.0f;
		}
		else if (leg_step == 4) // DOWN
		{
			tgait[leg].t_x = x_move / 2.0f;
			tgait[leg].t_y = y_move / 2.0f;
			tgait[leg].t_z = 0.0f;
			tgait[leg].rot_z = r_move / 2.0f;
		}
		else // MOVE BODY FORWARD
		{
			tgait[leg].t_x -= x_move;
			tgait[leg].t_y -= y_move;
			tgait[leg].t_z = 0.0f;
			tgait[leg].rot_z -= r_move;
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
	
}
