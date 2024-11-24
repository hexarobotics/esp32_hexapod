#include "gaits.h"
#include <cmath>
#include <esp_log.h>
#include "hexa_params.h"

namespace hexapod
{
	// Constructor Gaits
	Gaits::Gaits(GaitType gait, uint16_t num_legs) : 
		Xspeed(0.0), Yspeed(0.0), Rspeed(0.0), liftHeight(45),
		num_legs_(num_legs), stepsInCycle(0), pushSteps(0), desfase(0),
		tranTime(0), cycleTime(0.0),
		gaitleg_order{0}, parado(true), moving(false),
		gait_step(1), leg_step(0),
		modo_control(0), current_gait(NUM_MAX_GAITS)
	{
		// Inicializamos el orden de las patas
		gait_select(gait);

		init_tgaits();

		ESP_LOGI(TAG_GAIT,"Gait object created");
	}

	void Gaits::init_tgaits(void)
	{
		for ( int leg = 0; leg < num_legs_-1 ; leg++ ) 
		{
			tgait[leg].reset();
		}

		gait_step = 1;
	}

	// Método principal para seleccionar el gait
	void Gaits::gait_select(GaitType new_gait)
	{
		// Si el gait es el mismo que el actual, no hacemos nada
		if (new_gait == current_gait) return;

		// Actualizamos el gait actual
		current_gait = new_gait;
		liftHeight = 45;

		// Configuramos el orden de las patas en base al gait seleccionado
		setLegOrderByGait(current_gait);

		// Actualizamos los parámetros en base al gait seleccionado
		update_gait_params(current_gait);

		// Recalculamos variables dependientes
		cycleTime = (stepsInCycle * static_cast<float>(tranTime)) / 1000.0f;
		// Reset step
		gait_step = 1;

		// Actualizamos las velocidades
		update_velocities();
	}

	// Nuevo método para configurar el orden de las patas según el gait
	void Gaits::setLegOrderByGait(GaitType GaitType)
	{
		switch (GaitType)
		{
			case RIPPLE_6:
			case RIPPLE_12:
			case RIPPLE_24:
					gaitleg_order[LEFT_FRONT] 	= LEG_FIRST;
					gaitleg_order[LEFT_MIDDLE]  = LEG_FIFTH;
					gaitleg_order[LEFT_REAR] 	= LEG_THIRD;
					gaitleg_order[RIGHT_FRONT] 	= LEG_FOURTH;
					gaitleg_order[RIGHT_MIDDLE] = LEG_SECOND;
					gaitleg_order[RIGHT_REAR] 	= LEG_SIXTH;
				break;
			case TRIPOD_6:
			case TRIPOD_12:
			case TRIPOD_24:
					gaitleg_order[LEFT_FRONT] 	= LEG_FIRST;   // RIGHT_FRONT
					gaitleg_order[LEFT_MIDDLE]  = LEG_SECOND;  // RIGHT_REAR
					gaitleg_order[LEFT_REAR] 	= LEG_FIRST;   // LEFT_FRONT
					gaitleg_order[RIGHT_FRONT] 	= LEG_SECOND;  // LEFT_REAR
					gaitleg_order[RIGHT_MIDDLE] = LEG_FIRST;   // LEFT_MIDDLE
					gaitleg_order[RIGHT_REAR] 	= LEG_SECOND;  // RIGHT_MIDDLE
				break;
			case WAVE_12:
			case WAVE_24:
					gaitleg_order[LEFT_FRONT] 	= LEG_FIRST;   // RIGHT_FRONT
					gaitleg_order[LEFT_MIDDLE]  = LEG_SECOND;  // RIGHT_REAR
					gaitleg_order[LEFT_REAR] 	= LEG_THIRD;   // LEFT_FRONT
					gaitleg_order[RIGHT_FRONT] 	= LEG_FOURTH;  // LEFT_REAR
					gaitleg_order[RIGHT_MIDDLE] = LEG_FIFTH;   // RIGHT_MIDDLE
					gaitleg_order[RIGHT_REAR] 	= LEG_SIXTH;   // LEFT_MIDDLE
				break;
			default:
				// Podríamos agregar un manejo de error aquí si fuera necesario
				break;
		}
	}

	// Método para actualizar los parámetros del gait (pushSteps, stepsInCycle, etc.)
	void Gaits::update_gait_params(GaitType type)
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

	/**
	 * @brief 
	 * @details
	 *                            Distancia recorrida (mm)
	 *	 Veloc_max =   ----------------------------------------------
	*                      (trantime(ms) / conv_ms_to_s) * pushsteps
	*/
	void Gaits::update_velocities(void) 
	{
		Xspeed_max = 70.0f / ((static_cast<float>(tranTime) / 1000.0f) * pushSteps);
		Yspeed_max = 70.0f / ((static_cast<float>(tranTime) / 1000.0f) * pushSteps);
		Rspeed_max = (M_PI / 7.0f) / ((static_cast<float>(tranTime) / 1000.0f) * pushSteps);

		//ESP_LOGI(TAG_GAIT,"tranTime: %d", tranTime);
		//ESP_LOGI(TAG_GAIT,"pushSteps: %d", pushSteps);

		//ESP_LOGI(TAG_GAIT,"Xspeed max: %f", Xspeed_max);
		//ESP_LOGI(TAG_GAIT,"Yspeed_max max: %f", Yspeed_max);
		//ESP_LOGI(TAG_GAIT,"Rspeed_max max: %f", Rspeed_max);
	}

	void Gaits::configureBody()
	{
		tranTime = 140;
	}

	bool Gaits::isMoving() // refactorizar, lo veo obsoleto, hay otras maneras mejores de hacerlo
	{
		//ESP_LOGI(TAG_GAIT,"Xspeed: %f", Xspeed);
		moving = ( Xspeed > 1.50f || Xspeed < -1.50f ) || 
				 ( Yspeed > 1.50f || Yspeed < -1.50f ) || 
				 ( Rspeed > 0.05f || Rspeed < -0.05f );

		return moving;
	}

	uint16_t Gaits::get_gait_transition_time(void)
	{
		return tranTime;
	}

	// gait_step -> seria gait_step: es el step actual del gait ( por ejemplo de un gait
	// 		   que tiene 12 steps, pues el step por el que vamos ) luego leg_step, es el step
	// 		   especifico de cada pata, para poder reproducir el gait o modo de andar

	/***************************************/

	//	########################################			GAIT GENERATOR CODE			##############################################
	//		*************************************************************************************************************

	transformations3D::Tmatrix Gaits::step( uint8_t leg )
	{
		//gait_step = (gait_step+1)%stepsInCycle;
		// cambiar (stepsInCycle-pushSteps) por DESFASE: 1, 2, 3, 4, o 6 seg�n: (6, 12, 24 steps), wave 24 steps o tripod 24 steps
		leg_step = gait_step - (gaitleg_order[leg]-1) * (desfase);
		
		if (leg_step<0)	leg_step=stepsInCycle + leg_step;
		if (leg_step==0) leg_step=stepsInCycle;

		//ESP_LOGI(TAG_GAIT, "gait_step: %d", gait_step);
		//ESP_LOGI(TAG_GAIT, "leg_step: %d", leg_step);

		//ESP_LOGI(TAG_GAIT, "Xspeed: %f", Xspeed);
		//ESP_LOGI(TAG_GAIT, "Yspeed: %f", Yspeed);
		//ESP_LOGI(TAG_GAIT, "cycleTime: %f", cycleTime);
		//ESP_LOGI(TAG_GAIT, "pushSteps: %f", pushSteps);
		//ESP_LOGI(TAG_GAIT, "stepsInCycle: %f", stepsInCycle);


		if ( ( current_gait == RIPPLE_6) || ( current_gait == TRIPOD_6 ) || ( current_gait == WAVE_12 ) ) 	// ### 3 ETAPAS ###
		{
			if( leg_step == 1 ) 	  // UP
			{
				tgait[leg].t_x = 0.0f;
				tgait[leg].t_y = 0.0f;
				tgait[leg].t_z = liftHeight;
				tgait[leg].rot_z = 0.0f;
			}
			else if( leg_step == 2 ) 	// DOWN
			{
				tgait[leg].t_x = (Xspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
				tgait[leg].t_y = (Yspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
				tgait[leg].t_z = 0.0f;
				tgait[leg].rot_z = (Rspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);
			}
			else 	  // MOVE BODY FORWARD
			{
				tgait[leg].t_x = tgait[leg].t_x - (Xspeed * cycleTime) / stepsInCycle;
				tgait[leg].t_y = tgait[leg].t_y - (Yspeed * cycleTime) / stepsInCycle;
				tgait[leg].t_z = 0.0f;
				tgait[leg].rot_z = tgait[leg].rot_z - (Rspeed*cycleTime) / stepsInCycle;
			}
		}//6
		else if ( ( current_gait == RIPPLE_12 ) || ( current_gait == WAVE_24 ) || ( current_gait == TRIPOD_12 ) )	//	### 5 ETAPAS ###
		{
			if(leg_step == stepsInCycle)	// UP/2 :::: 12 (stepsInCycle)
			{	
				tgait[leg].t_x = tgait[leg].t_x / 2.0f;
				tgait[leg].t_y = tgait[leg].t_y / 2.0f;
				tgait[leg].t_z = liftHeight / 2.0f;
				tgait[leg].rot_z = tgait[leg].rot_z / 2.0f;
			}
			else if(leg_step == 1)		// UP
			{
				tgait[leg].t_x = 0.0f;
				tgait[leg].t_y = 0.0f;
				tgait[leg].t_z = liftHeight;
				tgait[leg].rot_z = 0.0f;
			}
			else if(leg_step == 2)	// DOWN/2
			{
				tgait[leg].t_x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle);
				tgait[leg].t_y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle);
				tgait[leg].t_z = int(liftHeight/2.0f);
				tgait[leg].rot_z = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
			}
			else if(leg_step == 3)		// DOWN
			{
				tgait[leg].t_x = (Xspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);
				tgait[leg].t_y = (Yspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);
				tgait[leg].t_z = 0.0f;
				tgait[leg].rot_z = (Rspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);
			}
			else	// MOVE BODY FORWARD
			{
				tgait[leg].t_x = tgait[leg].t_x - (Xspeed*cycleTime)/stepsInCycle;
				tgait[leg].t_y = tgait[leg].t_y - (Yspeed*cycleTime)/stepsInCycle;
				tgait[leg].t_z = 0.0f;
				tgait[leg].rot_z = tgait[leg].rot_z - (Rspeed*cycleTime)/stepsInCycle;
			}
		}//12
		else if ( ( current_gait == RIPPLE_24 ) || ( current_gait == TRIPOD_24 ) ) //	### 7 ETAPAS ###
		{
			if(leg_step == stepsInCycle-1)		// UP 1/3
			{
				tgait[leg].t_x = tgait[leg].t_x/3.0f;
				tgait[leg].t_y = tgait[leg].t_y/3.0f;
				tgait[leg].t_z = liftHeight/3.0f;
				tgait[leg].rot_z = tgait[leg].rot_z/3.0f;
			}
			else if(leg_step == stepsInCycle)	// UP 2/3
			{
				tgait[leg].t_x = tgait[leg].t_x*2.0f/3.0f;
				tgait[leg].t_y = tgait[leg].t_y*2.0f/3.0f;
				tgait[leg].t_z = liftHeight*2.0f/3.0f;
				tgait[leg].rot_z = tgait[leg].rot_z*2.0f/3.0f;
			}
			else if(leg_step == 1)	// UP
			{
				tgait[leg].t_x = 0.0f;
				tgait[leg].t_y = 0.0f;
				tgait[leg].t_z = liftHeight;
				tgait[leg].rot_z = 0.0f;
			}
			else if(leg_step == 2)	// DOWN 1/3
			{
				tgait[leg].t_x = (Xspeed*cycleTime*pushSteps)/(3.0f*stepsInCycle);
				tgait[leg].t_y = (Yspeed*cycleTime*pushSteps)/(3.0f*stepsInCycle);
				tgait[leg].t_z = liftHeight*2.0f/3;
				tgait[leg].rot_z = (Rspeed*cycleTime*pushSteps)/(3.0f*stepsInCycle);
			}
			else if(leg_step == 3)	// DOWN 2/3
			{
				tgait[leg].t_x = (Xspeed*cycleTime*pushSteps)*2.0f/(3.0f*stepsInCycle);
				tgait[leg].t_y = (Yspeed*cycleTime*pushSteps)*2.0f/(3.0f*stepsInCycle);
				tgait[leg].t_z = liftHeight/3;
				tgait[leg].rot_z = (Rspeed*cycleTime*pushSteps)*2.0f/(3.0f*stepsInCycle);
			}
			else if(leg_step == 4)	// DOWN
			{
				tgait[leg].t_x = (Xspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);// same as Xspeed*trantime*pushsteps/2
				tgait[leg].t_y = (Yspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);// same as Yspeed*trantime*pushsteps/2
				tgait[leg].t_z = 0;
				tgait[leg].rot_z = (Rspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);// same as Rspeed*trantime*pushsteps/2
			}
			else	// MOVE BODY FORWARD
			{
				tgait[leg].t_x = tgait[leg].t_x - (Xspeed*cycleTime)/stepsInCycle;// same as Xspeed*trantime
				tgait[leg].t_y = tgait[leg].t_y - (Yspeed*cycleTime)/stepsInCycle;// same as Yspeed*trantime
				tgait[leg].t_z = 0.0f;
				tgait[leg].rot_z = tgait[leg].rot_z - (Rspeed*cycleTime)/stepsInCycle;// same as Rspeed*trantime
			}
		}//24

		return tgait[leg];
	}

    void Gaits::next_step( void )
    {
        gait_step++;

        if ( gait_step > stepsInCycle )
        {
            gait_step = 1;
        }
    }



} // namespace
