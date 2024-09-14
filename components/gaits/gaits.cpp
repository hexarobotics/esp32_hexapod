#include "gaits.h"
#include <cmath>

namespace hexapod
{
	// Constructor Gaits
	Gaits::Gaits(GaitType gait) : 
	Xspeed(0.0), Yspeed(0.0), Rspeed(0.0), stepsInCycle(0), 
	tranTime(0.0), parado(true), step(1), pushSteps(0), 
	liftHeight(45), Current_Gait(NUM_MAX_GAITS), cycleTime(0.0)
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

	void Gaits::update_velocities() 
	{
		float Veloc_X_max = 70.0 / ((tranTime / 1000.0) * pushSteps);
		float Veloc_Y_max = 70.0 / ((tranTime / 1000.0) * pushSteps);
		float Veloc_Rot_max = (PI / 7) / ((tranTime / 1000.0) * pushSteps);
	}

	void Gaits::configureBody()
	{
		tranTime = 140;
	}

}


// gaitSelect refactorizado
// TODO: gait_generator, hexa_core, con todo incluido en una funcion apply ( do_ik, gaits, etc... )



























//  CODIGO ANTIGUO ************************************** abajo -->












//	########################################			GAIT GENERATOR CODE			##############################################		\\
//		*************************************************************************************************************	      \\


// como gaits lo que va a hacer es transformar un vect_3d, con traslaciones y rotaciones
// tendra que crear un objeto Tmatrix, con las traslaciones y rotaciones a aplicar
// es decir gaits[leg].loquesea sera un objeto tmatrix




void Gait_generator(uint8_t leg)
{
    if(!MOVING || modo_control==0) 		// NOT MOVING***
    {
        if(parado==false)parado=true;

        gaits[leg].x = 0;
        gaits[leg].y = 0;
        gaits[leg].z = 0;
        gaits[leg].r = 0;
        //step=1;
    }
    else //  MOVING***
    { 		
        if(parado==true)
        {
            parado=false; 
            step=1;
        }
  

		//step = (step+1)%stepsInCycle;
		// cambiar (stepsInCycle-pushSteps) por DESFASE: 1, 2, 3, 4, o 6 seg�n: (6, 12, 24 steps), wave 24 steps o tripod 24 steps
		leg_step = step-(gaitLegNo[leg]-1)*(desfase);		 // vale gial, se supone que est� bien

        if (leg_step<0)	leg_step=stepsInCycle + leg_step;  // parece que funciona para ciclos de 6, 12 y 24 steps en RIPPLE
        if (leg_step==0) leg_step=stepsInCycle;

		if((Current_Gait==ripple_6)||(Current_Gait==tripod_6)||(Current_Gait==wave_12)) 	// ### 3 ETAPAS ###
        {
			if(leg_step == 1) 	  // UP
            {
				gaits[leg].x = 0;
				gaits[leg].y = 0;
				gaits[leg].z = liftHeight;
				gaits[leg].r = 0;
			}
			else if(leg_step == 2) 	// DOWN
            {
				gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(2*stepsInCycle);
				gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(2*stepsInCycle);
				gaits[leg].z = 0;
				gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(2*stepsInCycle);
			}
            else 	  // MOVE BODY FORWARD
            {
                gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/stepsInCycle;
                gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/stepsInCycle;
                gaits[leg].z = 0;
                gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/stepsInCycle;
			}
        }//6

		if((Current_Gait==ripple_12)||(Current_Gait==wave_24)||(Current_Gait==tripod_12)){ //	### 5 ETAPAS ###

			if(leg_step == stepsInCycle){	// UP/2 :::: 12 (stepsInCycle)
				gaits[leg].x = gaits[leg].x/2;
				gaits[leg].y = gaits[leg].y/2;
				gaits[leg].z = liftHeight/2;
				gaits[leg].r = gaits[leg].r/2;
			}
			else if(leg_step == 1){	// UP
				gaits[leg].x = 0;
				gaits[leg].y = 0;
				gaits[leg].z = liftHeight;
				gaits[leg].r = 0;
			}
			else if(leg_step == 2){	// DOWN/2
				gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle);
				gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle);
				gaits[leg].z = int(liftHeight/2);
				gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
			}
			else if(leg_step == 3){	// DOWN
				gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(2*stepsInCycle);
				gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(2*stepsInCycle);
				gaits[leg].z = 0;
				gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(2*stepsInCycle);
			}
				else {	// MOVE BODY FORWARD
					gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/stepsInCycle;
					gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/stepsInCycle;
					gaits[leg].z = 0;
					gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/stepsInCycle;
			}
}//12

		if((Current_Gait==ripple_24)||(Current_Gait==tripod_24)) //	### 7 ETAPAS ###
        {

					if(leg_step == stepsInCycle-1){	// UP 1/3
						gaits[leg].x = gaits[leg].x/3;
						gaits[leg].y = gaits[leg].y/3;
						gaits[leg].z = liftHeight/3;
						gaits[leg].r = gaits[leg].r/3;
					}
					else if(leg_step == stepsInCycle){	// UP 2/3
						gaits[leg].x = gaits[leg].x*2/3;
						gaits[leg].y = gaits[leg].y*2/3;
						gaits[leg].z = liftHeight*2/3;
						gaits[leg].r = gaits[leg].r*2/3;
					}
					else if(leg_step == 1){	// UP
						gaits[leg].x = 0;
						gaits[leg].y = 0;
						gaits[leg].z = liftHeight;
						gaits[leg].r = 0;
					}
					else if(leg_step == 2){	// DOWN 1/3
						gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(3*stepsInCycle);
						gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(3*stepsInCycle);
						gaits[leg].z = liftHeight*2/3;
						gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(3*stepsInCycle);
					}
					else if(leg_step == 3){	// DOWN 2/3
						gaits[leg].x = (Xspeed*cycleTime*pushSteps)*2/(3*stepsInCycle);
						gaits[leg].y = (Yspeed*cycleTime*pushSteps)*2/(3*stepsInCycle);
						gaits[leg].z = liftHeight/3;
						gaits[leg].r = (Rspeed*cycleTime*pushSteps)*2/(3*stepsInCycle);
					}
					else if(leg_step == 4){	// DOWN
						gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(2*stepsInCycle);// same as Xspeed*trantime*pushsteps/2
						gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(2*stepsInCycle);// same as Yspeed*trantime*pushsteps/2
						gaits[leg].z = 0;
						gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(2*stepsInCycle);// same as Rspeed*trantime*pushsteps/2
					}else{	// MOVE BODY FORWARD
						gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/stepsInCycle;// same as Xspeed*trantime
						gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/stepsInCycle;// same as Yspeed*trantime
						gaits[leg].z = 0;
						gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/stepsInCycle;// same as Rspeed*trantime
					}
}//24


}

}



void gaitSelect(gait GaitType)  //ripple_6, ripple_12,	ripple_24,	wave_12,	wave_24,	tripod_6,	tripod_12,	tripod_24
{
	if(GaitType == Current_Gait)
    {
	    return;
    }

	Current_Gait = GaitType;
	liftHeight=45;
	// RIPPLE
	if((Current_Gait == ripple_6)||(Current_Gait == ripple_12)||(Current_Gait == ripple_24))
    {
		//ORDEN PATAS
        gaitLegNo[RIGHT_FRONT] = 4;//5;// actualizado
        gaitLegNo[RIGHT_REAR] =2; //1;// actualizado
        gaitLegNo[LEFT_FRONT] =1; //2;// actualizado
        gaitLegNo[LEFT_REAR] = 5;//4;// actualizado
        gaitLegNo[RIGHT_MIDDLE] =6;// 3;// actualizado
        gaitLegNo[LEFT_MIDDLE] =3; //6;// actualizado

		switch(Current_Gait)
        {
            case ripple_6 :
                pushSteps=4;
                stepsInCycle=6;
                desfase=1;
                tranTime=140;
                break;
            case ripple_12 :
                pushSteps=8;
                stepsInCycle=12;
                desfase=2;
                tranTime=120;
                break;
            case ripple_24 :
                pushSteps=12;
                stepsInCycle=18;
                desfase=3;
                tranTime=120;
                break;
            default:
                break;
        }
	}
    else if((Current_Gait == tripod_6)||(Current_Gait == tripod_12)||(Current_Gait == tripod_24))
    {

		gaitLegNo[RIGHT_FRONT] = 2;	// actualizado
		gaitLegNo[RIGHT_REAR] = 2;	// actualizado
		gaitLegNo[LEFT_FRONT] = 1;	// actualizado
		gaitLegNo[LEFT_MIDDLE] = 2;	// actualizado
		gaitLegNo[LEFT_REAR] = 1;	// actualizado
		gaitLegNo[RIGHT_MIDDLE] = 1;	// actualizado

		switch(Current_Gait) 
        {
            case tripod_6 : 	//	parece que hay que agregarle algun step entre elevaciones alternas de las 3 patas
                pushSteps=2;	//6 	ya que justo cuando una toca el suelo, la otra se levanta y se tambalea un poco
                stepsInCycle=4;//7
                desfase=2;//3
                tranTime=140;
                break;
            case tripod_12 :
                pushSteps=4;
                stepsInCycle=8;
                desfase=4;	// antes era 4 (para hacer que transicione mejor)
                tranTime=120;
                break;
            case tripod_24 :
                pushSteps=6;//19
                stepsInCycle=12;//24
                desfase=6;
                tranTime=120;
                break;
            default:
                break;
		}
//		tranTime = 65;???????????????????????????
		}else if((Current_Gait == wave_12)||(Current_Gait == wave_24)) //wave_12,	wave_24
        {		
			gaitLegNo[RIGHT_REAR] = 6;// actualizado
			gaitLegNo[RIGHT_MIDDLE] = 5;// actualizado
			gaitLegNo[RIGHT_FRONT] = 4;// actualizado
			gaitLegNo[LEFT_REAR] = 3;// actualizado
			gaitLegNo[LEFT_MIDDLE] = 2;// actualizado
			gaitLegNo[LEFT_FRONT] = 1;// actualizado

		switch(Current_Gait)
        {
            case wave_12 :
                pushSteps=10;
                stepsInCycle=12;
                desfase=2;
                tranTime=80;
                break;
            case wave_24 :
                pushSteps=20;
                stepsInCycle=24;
                desfase=4;
                tranTime=80;
                break;
            default:
            break;
        }
}
	// 	RECALCULAMOS VARIABLES		******************************************

	cycleTime = (stepsInCycle*tranTime)/1000.0;
	step = 1;	//	pero hay que poner la step por la que empezamos
	update_velocities ();

	//	Serial.print("Step actual: ");
	//	Serial.println(step);
}

/**
 * @brief 
 * @details
 *                            Distancia recorrida (mm)
 *	 Veloc_max =   ----------------------------------------------
 *                      (trantime(ms) / conv_ms_to_s) * pushsteps
 */
void update_velocities ()
{
	Veloc_X_max=(70.0)/((tranTime/1000.0)*pushSteps);
	Veloc_Y_max=(70.0)/((tranTime/1000.0)*pushSteps);
	Veloc_Rot_max=(PI/7)/((tranTime/1000.0)*pushSteps);
}