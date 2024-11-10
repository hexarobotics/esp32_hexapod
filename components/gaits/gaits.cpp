#include "gaits.h"
#include <cmath>

//#define MOVING   ((Xspeed > 15 || Xspeed < -15) || (Yspeed > 15 || Yspeed < -15) || (Rspeed > 0.05 || Rspeed < -0.05))

namespace hexapod
{
	// Constructor Gaits
	Gaits::Gaits(GaitType gait) : 
		Xspeed(0.0), Yspeed(0.0), Rspeed(0.0), liftHeight(45),
		stepsInCycle(0), pushSteps(0), desfase(0),
		tranTime(0.0), cycleTime(0.0),
		gaitleg_order{0}, parado(true), step(1), leg_step(0),
		modo_control(0), Current_Gait(NUM_MAX_GAITS)
	{
		// Inicializamos el orden de las patas
		gait_select(gait);
	}

	// Método principal para seleccionar el gait
	void Gaits::gait_select(GaitType new_gait)
	{
		// Si el gait es el mismo que el actual, no hacemos nada
		if (new_gait == Current_Gait) return;

		// Actualizamos el gait actual
		Current_Gait = new_gait;
		liftHeight = 45;

		// Configuramos el orden de las patas en base al gait seleccionado
		setLegOrderByGait(Current_Gait);

		// Actualizamos los parámetros en base al gait seleccionado
		update_gait_params(Current_Gait);

		// Recalculamos variables dependientes
		cycleTime = (stepsInCycle * tranTime) / 1000.0;
		// Reset step
		step = 1;

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
					gaitleg_order[0] = 4;  // RIGHT_FRONT
					gaitleg_order[1] = 2;  // RIGHT_REAR
					gaitleg_order[2] = 1;  // LEFT_FRONT
					gaitleg_order[3] = 5;  // LEFT_REAR
					gaitleg_order[4] = 6;  // RIGHT_MIDDLE
					gaitleg_order[5] = 3;  // LEFT_MIDDLE
				break;
			case TRIPOD_6:
			case TRIPOD_12:
			case TRIPOD_24:
					gaitleg_order[0] = 2;  // RIGHT_FRONT
					gaitleg_order[1] = 2;  // RIGHT_REAR
					gaitleg_order[2] = 1;  // LEFT_FRONT
					gaitleg_order[3] = 1;  // LEFT_REAR
					gaitleg_order[4] = 2;  // LEFT_MIDDLE
					gaitleg_order[5] = 1;  // RIGHT_MIDDLE
				break;
			case WAVE_12:
			case WAVE_24:
					gaitleg_order[0] = 4;  // RIGHT_FRONT
					gaitleg_order[1] = 6;  // RIGHT_REAR
					gaitleg_order[2] = 1;  // LEFT_FRONT
					gaitleg_order[3] = 3;  // LEFT_REAR
					gaitleg_order[4] = 5;  // RIGHT_MIDDLE
					gaitleg_order[5] = 2;  // LEFT_MIDDLE
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
		float Veloc_X_max = 70.0 / ((tranTime / 1000.0) * pushSteps);
		float Veloc_Y_max = 70.0 / ((tranTime / 1000.0) * pushSteps);
		float Veloc_Rot_max = (M_PI / 7) / ((tranTime / 1000.0) * pushSteps);
	}

	void Gaits::configureBody()
	{
		tranTime = 140;
	}

	bool Gaits::isMoving() const
	{
		return (Xspeed > 15 || Xspeed < -15) || (Yspeed > 15 || Yspeed < -15) || (Rspeed > 0.05 || Rspeed < -0.05);
	}

	// step -> seria gait_step: es el step actual del gait ( por ejemplo de un gait
	// 		   que tiene 12 steps, pues el step por el que vamos ) luego leg_step, es el step
	// 		   especifico de cada pata, para poder reproducir el gait o modo de andar

	/***************************************/

	//	########################################			GAIT GENERATOR CODE			##############################################
	//		*************************************************************************************************************

	void Gaits::Gait_generator(uint8_t leg)
	{
		if( !isMoving() || modo_control == 0 ) 		// NOT MOVING***
		{
			if( parado == false )
			{
				parado = true;

				gaits[leg].t_x 	 = 0;
				gaits[leg].t_y 	 = 0;
				gaits[leg].t_z 	 = 0;
				gaits[leg].phi_z = 0;
			}
		}
		else //  MOVING***
		{ 		
			if(parado==true)
			{
				parado = false; 
				step = 1;
			}

			//step = (step+1)%stepsInCycle;
			// cambiar (stepsInCycle-pushSteps) por DESFASE: 1, 2, 3, 4, o 6 seg�n: (6, 12, 24 steps), wave 24 steps o tripod 24 steps
			leg_step = step-(gaitleg_order[leg]-1)*(desfase);		 // vale gial, se supone que est� bien

			if (leg_step<0)	leg_step=stepsInCycle + leg_step;  // parece que funciona para ciclos de 6, 12 y 24 steps en RIPPLE
			if (leg_step==0) leg_step=stepsInCycle;

			if ( ( Current_Gait == RIPPLE_6) || ( Current_Gait == TRIPOD_6 ) || ( Current_Gait == WAVE_12 ) ) 	// ### 3 ETAPAS ###
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
					gaits[leg].t_x = (Xspeed*cycleTime*pushSteps)/(2*stepsInCycle);
					gaits[leg].t_y = (Yspeed*cycleTime*pushSteps)/(2*stepsInCycle);
					gaits[leg].t_z = 0;
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)/(2*stepsInCycle);
				}
				else 	  // MOVE BODY FORWARD
				{
					gaits[leg].t_x = gaits[leg].t_x - (Xspeed*cycleTime)/stepsInCycle;
					gaits[leg].t_y = gaits[leg].t_y - (Yspeed*cycleTime)/stepsInCycle;
					gaits[leg].t_z = 0;
					gaits[leg].phi_z = gaits[leg].phi_z - (Rspeed*cycleTime)/stepsInCycle;
				}
			}//6
			else if ( ( Current_Gait == RIPPLE_12 ) || ( Current_Gait == WAVE_24 )|| ( Current_Gait == TRIPOD_12 ) )	//	### 5 ETAPAS ###
			{
				if(leg_step == stepsInCycle)	// UP/2 :::: 12 (stepsInCycle)
				{	
					gaits[leg].t_x = gaits[leg].t_x/2;
					gaits[leg].t_y = gaits[leg].t_y/2;
					gaits[leg].t_z = liftHeight/2;
					gaits[leg].phi_z = gaits[leg].phi_z/2;
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
					gaits[leg].t_z = int(liftHeight/2);
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
				}
				else if(leg_step == 3)		// DOWN
				{
					gaits[leg].t_x = (Xspeed*cycleTime*pushSteps)/(2*stepsInCycle);
					gaits[leg].t_y = (Yspeed*cycleTime*pushSteps)/(2*stepsInCycle);
					gaits[leg].t_z = 0;
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)/(2*stepsInCycle);
				}
				else	// MOVE BODY FORWARD
				{
					gaits[leg].t_x = gaits[leg].t_x - (Xspeed*cycleTime)/stepsInCycle;
					gaits[leg].t_y = gaits[leg].t_y - (Yspeed*cycleTime)/stepsInCycle;
					gaits[leg].t_z = 0;
					gaits[leg].phi_z = gaits[leg].phi_z - (Rspeed*cycleTime)/stepsInCycle;
				}
			}//12
			else if ( ( Current_Gait == RIPPLE_24 ) || ( Current_Gait == TRIPOD_24 ) ) //	### 7 ETAPAS ###
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
					gaits[leg].t_x = gaits[leg].t_x*2/3;
					gaits[leg].t_y = gaits[leg].t_y*2/3;
					gaits[leg].t_z = liftHeight*2/3;
					gaits[leg].phi_z = gaits[leg].phi_z*2/3;
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
					gaits[leg].t_z = liftHeight*2/3;
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)/(3*stepsInCycle);
				}
				else if(leg_step == 3)	// DOWN 2/3
				{
					gaits[leg].t_x = (Xspeed*cycleTime*pushSteps)*2/(3*stepsInCycle);
					gaits[leg].t_y = (Yspeed*cycleTime*pushSteps)*2/(3*stepsInCycle);
					gaits[leg].t_z = liftHeight/3;
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)*2/(3*stepsInCycle);
				}
				else if(leg_step == 4)	// DOWN
				{
					gaits[leg].t_x = (Xspeed*cycleTime*pushSteps)/(2*stepsInCycle);// same as Xspeed*trantime*pushsteps/2
					gaits[leg].t_y = (Yspeed*cycleTime*pushSteps)/(2*stepsInCycle);// same as Yspeed*trantime*pushsteps/2
					gaits[leg].t_z = 0;
					gaits[leg].phi_z = (Rspeed*cycleTime*pushSteps)/(2*stepsInCycle);// same as Rspeed*trantime*pushsteps/2
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
	}

} // namespace
