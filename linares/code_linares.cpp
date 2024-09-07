

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

// Función para convertir radianes a grados
inline float radians_to_degrees(float radians)
{
    return radians * (180.0f / M_PI);
}

/* Simple 3dof leg solver. X,Y,Z are the length from the Coxa rotate to the endpoint. */
ik_sol_t legIK(int X, int Y, int Z, int leg)
{
    ik_sol_t ans; // angulo en servo pwm.

    // primero, resolver el angulo coxa para conseguir que nuestro problema se reduzca a un problema 2D
    ans.coxa = radians_to_degrees(atan2(Y,X)); //pata ya es una direccion de memoria

    long trueX = sqrt(sq((long)X)+sq((long)Y)) - L_COXA;
    long im = sqrt(sq((long)trueX)+sq((long)Z));    // length of imaginary leg
    // get femur angle above horizon...
    float q1  = atan2(Z,trueX);// Mientras que Z sea negativo, el angulo saldra negativo, cuando la pata sobrepase el eje horizontal, el angulo sera positivo
	long  n1  = sq(L_TIBIA)-sq(L_FEMUR)-sq(im);//numerador
    long  d2  = -2*L_FEMUR*im;//denominador
    float q2  = acos((float)n1/(float)d2);

    ans.femur = radians_to_degrees((q2+q1)); // lo pongo positivo pero ahora la z es negativa ya que el centro esta en coxa y endpoint esta por debajo
	//float intermedio=q2-q1;

    // and tibia angle from femur...
    n1 = sq(im)-sq(L_FEMUR)-sq(L_TIBIA);
    d2 = -2*L_FEMUR*L_TIBIA;
    //alfa=n1/d2 //teta3=alfa-90º
    ans.tibia = radians_to_degrees((acos((float)n1/(float)d2))-1.57);

    //  ##### adaptar los angulos reales ##### 	//
    ans = real_angle( leg, ans);//		pasamos la soluci�n de los angulos y los adaptamos a cada servomotor

    return ans;
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
