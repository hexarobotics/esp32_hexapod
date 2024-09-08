#include <Hexaik.h>
#include "esp_log.h"

uint8_t control_cuerpo;
uint8_t control_robot;

#define DEG_90  M_PI_2
#define DEG_45  M_PI_4

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

    // Implementación de legIK 3DOF
    Hexaik::ik_angles Hexaik::legIK(int X, int Y, int Z, leg_id leg) 
    {
        ik_angles ans;

        // Resolver el ángulo de la coxa
        ESP_LOGW("IK","COXA orig: %lf\n",atan2(Y, X));
        ans.coxa = radians_to_degrees(atan2(Y, X));

        // Asegurarse de que los cálculos no generen valores inválidos
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
        
        ESP_LOGW("IK","FEMUR orig: %lf\n",(q2 + q1));
        ans.femur = radians_to_degrees(q2 + q1);

        n1 = (im * im) - (L_FEMUR * L_FEMUR) - (L_TIBIA * L_TIBIA);
        d2 = -2 * L_FEMUR * L_TIBIA;

        if (d2 == 0 || fabs(n1 / d2) > 1) {
            ESP_LOGE(TAG, "Error: Valores invalidos para acos (tibia)");
            return ans;
        }

        ESP_LOGW("IK","TIBIA orig: %lf\n",(acos(n1 / d2))); //
        //ans.tibia = radians_to_degrees(acos(n1 / d2) - DEG_90); original
        ans.tibia = radians_to_degrees(acos(n1 / d2));// alfa // - DEG_45);

        // Ajustar los ángulos reales
        ans = real_angle(leg, ans);

        ESP_LOGI(TAG, "coxa: %d, femur: %d, tibia: %d\n", ans.coxa, ans.femur, ans.tibia);

        return ans;
    }

    Hexaik::ik_angles Hexaik::real_angle( leg_id leg, ik_angles angles )
    {
        ik_angles real;

        /*	####################   COXA	  #####################	*/
        if (leg == LEFT_FRONT) // Para pata 1
        {
            if (angles.coxa >= 0)
            {
                real.coxa = angles.coxa - 90;
            }
            else // 3*45 + beta, siendo beta = 180 - abs|coxa|
            {
                real.coxa = 315 - std::abs(angles.coxa); 
            }
        }
        else if (leg == LEFT_MIDDLE)
        {
            if (angles.coxa >= 0)
            {
                real.coxa = angles.coxa - 90;
            }
            else
            {
                real.coxa = 270 - std::abs(angles.coxa);
            }
        }
        else if (leg == LEFT_REAR)
        {
            if (angles.coxa >= 0)
            {
                real.coxa = angles.coxa - 135;
            }
            else
            {
                real.coxa = 225 - std::abs(angles.coxa);
            }
        }
        else if (leg == RIGHT_FRONT)
        {
            real.coxa = angles.coxa + 45;
        }
        else if (leg == RIGHT_MIDDLE)
        {
            real.coxa = angles.coxa + 90;
        }
        else if (leg == RIGHT_REAR)
        {
            real.coxa = angles.coxa + 135;
        }

        /*	####################   FEMUR	  #####################	*/
        real.femur = 90 - angles.femur;  // Ajuste universal para todas las patas

        /*	####################   TIBIA	  #####################	*/
        // No se requiere ajuste adicional para tibia

        if ( angles.tibia <= 45 )
        {
            real.tibia = 0;
        }
        else
        {
            real.tibia = angles.tibia - 45;
        }


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

    int Hexaik::radians_to_degrees(double radians)
    {
        // Convertir radianes a grados
        double degrees = radians * (180.0 / M_PI);
        
        // Redondear al entero más cercano
        int rounded_degrees = static_cast<int>(std::round(degrees));
        // Verificar si el valor está fuera de los límites
        //if (degrees < 0) {
        //    ESP_LOGW("IK", "Angulo fuera de limites: %d grados. Ajustando a 0.", degrees);
        //    degrees = 0;
        //} else if (degrees > 180) {
        //    ESP_LOGW("IK", "Angulo fuera de limites: %d grados. Ajustando a 180.", degrees);
        //    degrees = 180;
        //}

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
