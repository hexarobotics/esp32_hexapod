

#include "cmath"
#include "stdint-gcc.h"

#include "code_linares.h"

void do_ik( void ) // hex.cpp -> hex_compute_step() (quitar de ik.cpp)
{
    for(uint8_t n_pata=0; n_pata<6;n_pata++)
    {
        Gait_generator(n_pata); // gait.cpp file
        //body_ik -> ahora llamado Tmatrix_apply o algo asi, la matriz de T homogenea. ik.cpp file
        legIK(); //ik.cpp file
        check_angles_range_and_save_to_send_servos(); // servos_and_i2c.cpp file

        uint16_t gait_step++;
        if (step>stepsInCycle)step=1;
    }
}


void real_angle( void )
{
    // CODIGO COXA ANGLE

    if ( leg == LEFT_FRONT ) // Para 1, por analizar
    {
        if( angulo.coxa >= 0 )
        {
            angulo.coxa =- (Constante_PWM/2)+angulo.coxa;/*hecho*/
        }
        else // 3*45 + beta, siendo beta = 180 - abs|teta|
        {
            angulo.coxa =- angulo.coxa;
            angulo.coxa = 315 - angulo.coxa;
        }
    }
    else if ( leg == LEFT_MIDDLE )
    {
        if( angulo.coxa >= 0 )
        {
            angulo.coxa = angulo.coxa - 90;	/*hecho*/
        }
        else
        {
            angulo.coxa =- angulo.coxa;
            angulo.coxa = 270 - angulo.coxa;
        }
    }
    else if( leg == LEFT_REAR )
    {
        if( angulo.coxa >= 0 )
        {
            angulo.coxa = angulo.coxa - 135;
        }
        else
        { 		
            angulo.coxa = 225 - angulo.coxa;
        }
    }
    else if(leg==RIGHT_FRONT)
    {
        angulo.coxa = angulo.coxa + 45;
	}
    else if(leg==RIGHT_MIDDLE)
    {
		angulo.coxa = angulo.coxa + 90;
    }
    else if(leg==RIGHT_REAR)
    {
		angulo.coxa = angulo.coxa + 135;
	}

    // 

    	//		####################		   FEMUR	 	 #####################		//
	// PARA todas las patas igual
    angulo.femur = 90 - angulo.femur; // GOOD
	//angulo.femur=PATAS[leg].servo_femur.Cero+(Constante_PWM/2)+angulo.femur;
	//angulo.femur=PATAS[leg].servo_femur.Cero+(3*(Constante_PWM/2))-angulo.femur;
	//		####################		   TIBIA			  ######################		//
	// PARA todas las patas igual
    // PARA el angulo tibia se aplica el angulo calculado y ya
}
