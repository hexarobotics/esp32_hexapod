#include "gaits.h"
#include <cmath>
#include <esp_log.h>
#include "hexa_params.h"

namespace hexapod
{
	// Constructor Gaits
	Gaits::Gaits(GaitType gait) : 
		Xspeed(0.0), Yspeed(0.0), Rspeed(0.0), liftHeight(45),
		stepsInCycle(0), pushSteps(0), desfase(0),
		tranTime(0), cycleTime(0.0),
		gaitleg_order{0}, parado(true), gait_step(1), leg_step(0),
		modo_control(0), current_gait(NUM_MAX_GAITS)
	{
		// Inicializamos el orden de las patas
		gait_select(gait);

		for (int i = 0; i < 6; ++i)
        {
            gaits[i].setTranslationX(0);
            gaits[i].setTranslationY(0);
            gaits[i].setTranslationZ(0);
            gaits[i].setAlpha(0.0);
            gaits[i].setTheta(0.0);
            gaits[i].setPhi(0.0);
        }

		ESP_LOGI(TAG_GAIT,"Gait object created");
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
					gaitleg_order[LEFT_FRONT] 	= LEG_FIRST;  // RIGHT_FRONT
					gaitleg_order[LEFT_MIDDLE]  = LEG_SECOND;  // RIGHT_REAR
					gaitleg_order[LEFT_REAR] 	= LEG_FIRST;   // LEFT_FRONT
					gaitleg_order[RIGHT_FRONT] 	= LEG_SECOND;   // LEFT_REAR
					gaitleg_order[RIGHT_MIDDLE] = LEG_FIRST;  // LEFT_MIDDLE
					gaitleg_order[RIGHT_REAR] 	= LEG_SECOND;   // RIGHT_MIDDLE
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
				pushSteps = 4.0f;
				stepsInCycle = 6.0f;
				desfase = 1;
				tranTime = 140;
				break;

			case RIPPLE_12:
				pushSteps = 8.0f;
				stepsInCycle = 12.0f;
				desfase = 2;
				tranTime = 120;
				break;

			case RIPPLE_24:
				pushSteps = 12.0f;
				stepsInCycle = 18.0f;
				desfase = 3;
				tranTime = 120;
				break;

			case TRIPOD_6:
				pushSteps = 2.0f;
				stepsInCycle = 4.0f;
				desfase = 2;
				tranTime = 140;
				break;

			case TRIPOD_12:
				pushSteps = 4.0f;
				stepsInCycle = 8.0f;
				desfase = 4;
				tranTime = 120;
				break;

			case TRIPOD_24:
				pushSteps = 6.0f;
				stepsInCycle = 12.0f;
				desfase = 6;
				tranTime = 120;
				break;

			case WAVE_12:
				pushSteps = 10.0f;
				stepsInCycle = 12.0f;
				desfase = 2;
				tranTime = 80;
				break;

			case WAVE_24:
				pushSteps = 20.0f;
				stepsInCycle = 24.0f;
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
//
		//ESP_LOGI(TAG_GAIT,"Xspeed max: %f", Xspeed_max);
		//ESP_LOGI(TAG_GAIT,"Yspeed_max max: %f", Yspeed_max);
		//ESP_LOGI(TAG_GAIT,"Rspeed_max max: %f", Rspeed_max);
	}

	void Gaits::configureBody()
	{
		tranTime = 140;
	}

	bool Gaits::isMoving() const // refactorizar, lo veo obsoleto, hay otras maneras mejores de hacerlo
	{
		//ESP_LOGI(TAG_GAIT,"Xspeed: %f", Xspeed);

		return (Xspeed > 1.50f || Xspeed < -1.50f ) || (Yspeed > 1.50f || Yspeed < -1.50f ) || (Rspeed > 0.05f || Rspeed < -0.05f );
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

	static uint16_t cont = 0;

	void Gaits::log_info(const char* msg)
	{
		cont++;

		if (cont == 50)
		{
			ESP_LOGI(TAG_GAIT,"%s", msg);

			cont = 0;
		}
	}

	transformations3D::Tmatrix Gaits::step( uint8_t leg )
	{
		//log_info("entro en step");


        if( !isMoving() )//|| modo_control == 0 )      // NOT MOVING***
        {
			log_info("if not moving");

            if( parado == false )
            {
				log_info("parado - false");

                parado = true;

                gaits[leg].t_x 	 = 0;
                gaits[leg].t_y 	 = 0;
                gaits[leg].t_z 	 = 0;
                gaits[leg].phi_z = 0;
            }
        }
        else //  MOVING***
        {
			//log_info("else - moving");

            if(parado==true)
            {
				ESP_LOGI(TAG_GAIT, "Parado == true");

                parado = false;
                gait_step = 1;
            }

			//ESP_LOGI(TAG_GAIT, "HEXA executing gait step");

			//gait_step = (gait_step+1)%stepsInCycle;
			// cambiar (stepsInCycle-pushSteps) por DESFASE: 1, 2, 3, 4, o 6 seg�n: (6, 12, 24 steps), wave 24 steps o tripod 24 steps
			leg_step = gait_step - (gaitleg_order[leg]-1) * (desfase);
			
			if (leg_step<0)	leg_step=stepsInCycle + leg_step;
			if (leg_step==0) leg_step=stepsInCycle;

			ESP_LOGI(TAG_GAIT, "gait_step: %d", gait_step);
			ESP_LOGI(TAG_GAIT, "leg_step: %d", leg_step);

			ESP_LOGI(TAG_GAIT, "Xspeed: %f", Xspeed);
			ESP_LOGI(TAG_GAIT, "Yspeed: %f", Yspeed);
			ESP_LOGI(TAG_GAIT, "cycleTime: %f", cycleTime);
			ESP_LOGI(TAG_GAIT, "pushSteps: %f", pushSteps);
			ESP_LOGI(TAG_GAIT, "stepsInCycle: %f", stepsInCycle);


			if ( ( current_gait == RIPPLE_6) || ( current_gait == TRIPOD_6 ) || ( current_gait == WAVE_12 ) ) 	// ### 3 ETAPAS ###
			{
				if( leg_step == 1 ) 	  // UP
				{
					gaits[leg].t_x = 0;
					gaits[leg].t_y = 0;
					gaits[leg].t_z = liftHeight;
					gaits[leg].phi_z = 0;
				}
				else if( leg_step == 2 ) 	// DOWN
				{
					gaits[leg].t_x = (Xspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
					gaits[leg].t_y = (Yspeed * cycleTime * pushSteps) / (2.0f * stepsInCycle);
					gaits[leg].t_z = 0;
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);
				}
				else 	  // MOVE BODY FORWARD
				{
					gaits[leg].t_x = gaits[leg].t_x - (Xspeed * cycleTime) / stepsInCycle;
					gaits[leg].t_y = gaits[leg].t_y - (Yspeed * cycleTime) / stepsInCycle;
					gaits[leg].t_z = 0;
					gaits[leg].phi_z = gaits[leg].phi_z - (Rspeed*cycleTime) / stepsInCycle;
				}
			}//6
			else if ( ( current_gait == RIPPLE_12 ) || ( current_gait == WAVE_24 )|| ( current_gait == TRIPOD_12 ) )	//	### 5 ETAPAS ###
			{
				if(leg_step == stepsInCycle)	// UP/2 :::: 12 (stepsInCycle)
				{	
					gaits[leg].t_x = gaits[leg].t_x / 2.0f;
					gaits[leg].t_y = gaits[leg].t_y / 2.0f;
					gaits[leg].t_z = liftHeight / 2.0f;
					gaits[leg].phi_z = gaits[leg].phi_z / 2.0f;
				}
				else if(leg_step == 1)		// UP
				{
					gaits[leg].t_x = 0;
					gaits[leg].t_y = 0;
					gaits[leg].t_z = liftHeight;
					gaits[leg].phi_z = 0;
				}
				else if(leg_step == 2)	// DOWN/2
				{
					gaits[leg].t_x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle);
					gaits[leg].t_y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle);
					gaits[leg].t_z = int(liftHeight/2.0f);
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
				}
				else if(leg_step == 3)		// DOWN
				{
					gaits[leg].t_x = (Xspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);
					gaits[leg].t_y = (Yspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);
					gaits[leg].t_z = 0;
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);
				}
				else	// MOVE BODY FORWARD
				{
					gaits[leg].t_x = gaits[leg].t_x - (Xspeed*cycleTime)/stepsInCycle;
					gaits[leg].t_y = gaits[leg].t_y - (Yspeed*cycleTime)/stepsInCycle;
					gaits[leg].t_z = 0;
					gaits[leg].phi_z = gaits[leg].phi_z - (Rspeed*cycleTime)/stepsInCycle;
				}
			}//12
			else if ( ( current_gait == RIPPLE_24 ) || ( current_gait == TRIPOD_24 ) ) //	### 7 ETAPAS ###
			{
				if(leg_step == stepsInCycle-1)		// UP 1/3
				{
					gaits[leg].t_x = gaits[leg].t_x/3;
					gaits[leg].t_y = gaits[leg].t_y/3;
					gaits[leg].t_z = liftHeight/3;
					gaits[leg].phi_z = gaits[leg].phi_z/3;
				}
				else if(leg_step == stepsInCycle)	// UP 2/3
				{
					gaits[leg].t_x = gaits[leg].t_x*2.0f/3;
					gaits[leg].t_y = gaits[leg].t_y*2.0f/3;
					gaits[leg].t_z = liftHeight*2.0f/3;
					gaits[leg].phi_z = gaits[leg].phi_z*2.0f/3;
				}
				else if(leg_step == 1)	// UP
				{
					gaits[leg].t_x = 0;
					gaits[leg].t_y = 0;
					gaits[leg].t_z = liftHeight;
					gaits[leg].phi_z = 0;
				}
				else if(leg_step == 2)	// DOWN 1/3
				{
					gaits[leg].t_x = (Xspeed*cycleTime*pushSteps)/(3*stepsInCycle);
					gaits[leg].t_y = (Yspeed*cycleTime*pushSteps)/(3*stepsInCycle);
					gaits[leg].t_z = liftHeight*2.0f/3;
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)/(3*stepsInCycle);
				}
				else if(leg_step == 3)	// DOWN 2/3
				{
					gaits[leg].t_x = (Xspeed*cycleTime*pushSteps)*2.0f/(3*stepsInCycle);
					gaits[leg].t_y = (Yspeed*cycleTime*pushSteps)*2.0f/(3*stepsInCycle);
					gaits[leg].t_z = liftHeight/3;
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)*2.0f/(3*stepsInCycle);
				}
				else if(leg_step == 4)	// DOWN
				{
					gaits[leg].t_x = (Xspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);// same as Xspeed*trantime*pushsteps/2
					gaits[leg].t_y = (Yspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);// same as Yspeed*trantime*pushsteps/2
					gaits[leg].t_z = 0;
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)/(2.0f*stepsInCycle);// same as Rspeed*trantime*pushsteps/2
				}
				else	// MOVE BODY FORWARD
				{
					gaits[leg].t_x = gaits[leg].t_x - (Xspeed*cycleTime)/stepsInCycle;// same as Xspeed*trantime
					gaits[leg].t_y = gaits[leg].t_y - (Yspeed*cycleTime)/stepsInCycle;// same as Yspeed*trantime
					gaits[leg].t_z = 0;
					gaits[leg].phi_z = gaits[leg].phi_z - (Rspeed*cycleTime)/stepsInCycle;// same as Rspeed*trantime
				}
			}//24
		} // moving

        return gaits[leg];
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
