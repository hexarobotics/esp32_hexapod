#include <Hexaik.h>

uint8_t control_cuerpo;
uint8_t control_robot;

#define DEG_90  1.57F

namespace hexapod
{
    // Class Hexaik
    // Constructor Hexaik
    Hexaik::Hexaik()
    {
	    // Inicializa todos los elementos del arreglo a {0.0, 0.0, 0.0}
	    for (int i = 0; i < num_legs; ++i)
	    {
	        leg_endpoints[i] = Vectors::vector3d(); // Usa el constructor por defecto que inicializa a 0
	    }
    }

    // Implementación de legIK
    Hexaik::ik_angles Hexaik::legIK(int X, int Y, int Z, leg_id leg)
    {
        ik_angles ans;

        // Resolver el ángulo de la coxa
        ans.coxa = radians_to_degrees(atan2(Y, X));

        long trueX = sqrt((long)X * (long)X + (long)Y * (long)Y) - L_COXA;
        long im = sqrt((long)trueX * (long)trueX + (long)Z * (long)Z);  // Longitud de la pierna imaginaria
        float q1 = atan2(Z, trueX);
        long n1 = (L_TIBIA * L_TIBIA) - (L_FEMUR * L_FEMUR) - (im * im);
        long d2 = -2 * L_FEMUR * im;
        float q2 = acos((float)n1 / (float)d2);

        ans.femur = radians_to_degrees(q2 + q1);

        n1 = (im * im) - (L_FEMUR * L_FEMUR) - (L_TIBIA * L_TIBIA);
        d2 = -2 * L_FEMUR * L_TIBIA;
        ans.tibia = radians_to_degrees(acos((float)n1 / (float)d2) - DEG_90); // que cojones hacia el 1.57?

        // Ajustar los ángulos reales
        ans = real_angle(leg, ans);

        return ans;
    }

    Hexaik::ik_angles Hexaik::real_angle( leg_id leg, ik_angles angles )
    {
        ik_angles real;

        /*	####################   COXA	  #####################	*/
        if ( leg == LEFT_FRONT ) // Para 1, por analizar
        {
            if( angles.coxa >= 0 )
            {
                //angles.coxa =- (Constante_PWM/2)+angles.coxa;/*hecho*/

                real.coxa = angles.coxa - 90;
            }
            else // 3*45 + beta, siendo beta = 180 - abs|coxa|
            {
                //angles.coxa =- angles.coxa;
                angles.coxa = std::abs(angles.coxa);

                real.coxa = 315 - angles.coxa;
            }
        }
        else if ( leg == LEFT_MIDDLE )
        {
            if( angles.coxa >= 0 )
            {
                real.coxa = angles.coxa - 90;	/*hecho*/
            }
            else
            {
                //angles.coxa =- angles.coxa;
                angles.coxa = std::abs(angles.coxa);

                real.coxa = 270 - angles.coxa;
            }
        }
        else if( leg == LEFT_REAR )
        {
            if( angles.coxa >= 0 )
            {
                real.coxa = angles.coxa - 135;
            }
            else
            { 		
                real.coxa = 225 - std::abs(angles.coxa);
            }
        }
        else if(leg==RIGHT_FRONT)
        {
            real.coxa = angles.coxa + 45;
        }
        else if(leg==RIGHT_MIDDLE)
        {
            real.coxa = angles.coxa + 90;
        }
        else if(leg==RIGHT_REAR)
        {
            real.coxa = angles.coxa + 135;
        }

        // 

            //		####################		   FEMUR	 	 #####################		//
        // PARA todas las patas igual
        real.femur = 90 - angles.femur; // GOOD
        //angulo.femur=PATAS[leg].servo_femur.Cero+(Constante_PWM/2)+angulo.femur;
        //angulo.femur=PATAS[leg].servo_femur.Cero+(3*(Constante_PWM/2))-angulo.femur;


        //		####################		   TIBIA			  ######################		//
        // PARA todas las patas igual
        // PARA el angulo tibia se aplica el angulo calculado y ya

        return real;
    }

    void Hexaik::do_ik( void ) // hex.cpp -> hex_compute_step() (quitar de ik.cpp)
    {
        for(uint8_t leg=0; leg<=num_max_legs;leg++)
        {
        //    Gait_generator(n_pata); // gait.cpp file
        //    //body_ik -> ahora llamado Tmatrix_apply o algo asi, la matriz de T homogenea. ik.cpp file
            //legIK(); //ik.cpp file
//
        //    check_angles_range_and_save_to_send_servos(); // servos_and_i2c.cpp file
//
        //    uint16_t gait_step++;
//
        //    if (step>stepsInCycle)step=1;
        }
    }

    Hexaik::ik_angles Hexaik::do_1_leg_ik( Vectors::vector3d vec, leg_id leg  ) // hex.cpp -> hex_compute_step() (quitar de ik.cpp)
    {
        //    Gait_generator(n_pata); // gait.cpp file
        //    //body_ik -> ahora llamado Tmatrix_apply o algo asi, la matriz de T homogenea. ik.cpp file

        return legIK(vec.x,vec.y,vec.z,leg); //ik.cpp file
//
        //    check_angles_range_and_save_to_send_servos(); // servos_and_i2c.cpp file
//
        //    uint16_t gait_step++;
//
        //    if (step>stepsInCycle)step=1;
    }
    
} // namespace hexapod


/// que he hecho
/// TODO: separar las clases en archivos distintos





void get_ik_initial_parameters()
{
    if ( control_cuerpo == 1 )
    {

    }
    else if ( control_robot == 1 )
    {

    }
}
