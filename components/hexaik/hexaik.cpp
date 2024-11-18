#include "Hexaik.h"
#include "esp_log.h"

uint8_t control_cuerpo;
uint8_t control_robot;

#define DEG_90  M_PI_2
#define DEG_45  M_PI_4

namespace hexapod
{
    const char* Hexaik::TAG = "HEXA_IK";

    // Class Hexaik
    // Constructor Hexaik
    Hexaik::Hexaik()
    {
	    for (int i = 0; i < num_legs; ++i)
	    {
	        leg_endpoints[i] = Vectors::vector3d(); // inicializa a 0
	    }
    }

    // Implementación de legIK 3DOF
    Hexaik::ik_angles Hexaik::legIK( leg_id leg, int X, int Y, int Z )
    {
        ik_angles ans;

        // Resolver el ángulo de la coxa
        //ESP_LOGW("IK","COXA orig: %lf\n",atan2(Y, X));
        ans.coxa = radians_to_degrees(atan2(Y, X));

        double trueX = sqrt((double)X * X + (double)Y * Y) - L_COXA;
        double im = sqrt(trueX * trueX + Z * Z);  // Longitud de la pierna imaginaria
        double q1 = atan2(Z, trueX);

        double n1 = (L_TIBIA * L_TIBIA) - (L_FEMUR * L_FEMUR) - (im * im);
        double d2 = -2 * L_FEMUR * im;

        if (d2 == 0 || fabs(n1 / d2) > 1) {
            ESP_LOGE(TAG, "Error: Valores invalidos para acos");
            return ans;  // Devuelve un valor por defecto o un error
        }

        double q2 = acos(n1 / d2);
        
        //ESP_LOGW("IK","FEMUR orig: %lf\n",(q2 + q1));
        ans.femur = radians_to_degrees(q2 + q1);

        n1 = (im * im) - (L_FEMUR * L_FEMUR) - (L_TIBIA * L_TIBIA);
        d2 = -2 * L_FEMUR * L_TIBIA;

        if (d2 == 0 || fabs(n1 / d2) > 1) {
            ESP_LOGE(TAG, "Error: Valores invalidos para acos (tibia)");
            return ans;
        }

        //ESP_LOGW("IK","TIBIA orig: %lf\n",(acos(n1 / d2))); //
        //ans.tibia = radians_to_degrees(acos(n1 / d2) - DEG_90); original
        ans.tibia = radians_to_degrees(acos(n1 / d2));// alfa // - DEG_45);

        // Ajustar los ángulos reales
        ans = real_angle(leg, ans);

        //ESP_LOGI(TAG, "coxa: %d, femur: %d, tibia: %d\n", ans.coxa, ans.femur, ans.tibia);

        return ans;
    }

    Hexaik::ik_angles Hexaik::real_angle( leg_id leg, ik_angles angles )
    {
        ik_angles real;

        /*	####################   COXA	  #####################	*/
        switch (leg)
        {
            case LEFT_FRONT:
                real.coxa = (angles.coxa >= 0) ? angles.coxa - 90 : 315 - std::abs(angles.coxa);
                break;

            case LEFT_MIDDLE:
                real.coxa = (angles.coxa >= 0) ? angles.coxa - 90 : 270 - std::abs(angles.coxa);
                break;

            case LEFT_REAR:
                real.coxa = (angles.coxa >= 0) ? angles.coxa - 135 : 225 - std::abs(angles.coxa);
                break;

            case RIGHT_FRONT:
                real.coxa = angles.coxa + 45;
                break;

            case RIGHT_MIDDLE:
                real.coxa = angles.coxa + 90;
                break;

            case RIGHT_REAR:
                real.coxa = angles.coxa + 135;
                break;

            default:
                break;
        }

        /*      femur y tibia igual para todas las patas     */
        /*	####################   FEMUR	  #####################	*/
        real.femur = 90 - angles.femur;  // Ajuste universal para todas las patas

        /*	####################   TIBIA	  #####################	*/
        real.tibia = (angles.tibia <= 45) ? 0 : angles.tibia - 45;  // se ha ganado fisicamente un angulo de 45

        return real;
    }

    void Hexaik::do_ik( void ) // hex.cpp -> hex_compute_step() (quitar de ik.cpp)
    {
        for( uint8_t leg = 0; leg <= NUM_MAX_LEGS; leg++ )
        {
        //    gait_step(n_pata); // gait.cpp file
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
        //    gait_step(n_pata); // gait.cpp file
        //    //body_ik -> ahora llamado Tmatrix_apply o algo asi, la matriz de T homogenea. ik.cpp file

        return legIK( leg, vec.x, vec.y, vec.z ); //ik.cpp file
//
        //    check_angles_range_and_save_to_send_servos(); // servos_and_i2c.cpp file
//
        //    uint16_t gait_step++;
//
        //    if (step>stepsInCycle)step=1;
    }

    int Hexaik::radians_to_degrees(double radians)
    {
        double degrees = radians * (180.0 / M_PI);
        
        int rounded_degrees = static_cast<int>(std::round(degrees));

        return rounded_degrees;
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
