
#include "hexa_params.h"
#include "Hexaik.h"
#include "hexa_gaits_mode.h"
//#include <esp_log.h>
//#include "esp_timer.h"
//#include "gaits_control_interface.h"
#include "body_control_interface.h"

namespace hexapod
{
    bool init_position = true;

    transformations3D::Tmatrix tbody;

    transformations3D::Vectors::vector3d calculate_total_vector( uint8_t leg )
    {
        transformations3D::Vectors::vector3d v_out;

        v_out.x = coxa_endpoints[leg].x + leg_endpoints[leg].x + leg_offsets[leg].x;
        v_out.y = coxa_endpoints[leg].y + leg_endpoints[leg].y + leg_offsets[leg].y;
        v_out.z = leg_endpoints[leg].z  + leg_offsets[leg].z;
        
        return v_out;
    }

    transformations3D::Vectors::vector3d recalculate_leg_vector( transformations3D::Vectors::vector3d coxa_vect, transformations3D::Vectors::vector3d total_vect )
    {
        transformations3D::Vectors::vector3d leg_new;

        leg_new.x = total_vect.x - coxa_vect.x;
        leg_new.y = total_vect.y - coxa_vect.y;
        leg_new.z = total_vect.z - coxa_vect.z;
        
        return leg_new;
    }

    // Restablecer la posición inicial
    void resetToInitPosition(Servo::ServoController &servo_ctr) // llevar a un archivo common con gait_control
    {
        for (uint8_t leg = LEFT_FRONT; leg < NUM_MAX_LEGS; leg++)
        {
            // 1. Vector total y transformaciones
            transformations3D::Vectors::vector3d leg_vect;

            leg_vect.x = leg_endpoints[leg].x + leg_offsets[leg].x;
            leg_vect.y = leg_endpoints[leg].y + leg_offsets[leg].y;
            leg_vect.z = leg_endpoints[leg].z + leg_offsets[leg].z;

            // 2. Resolver cinemática inversa y guardar posiciones
            hexapod::Hexaik::ik_angles ik_angles = hexapod::Hexaik::legIK(
                static_cast<leg_id>(leg), leg_vect.x, leg_vect.y, leg_vect.z);

            // 3. Guardar nextpose
            servo_ctr.save_nextpose(leg * 3, ik_angles.coxa);
            servo_ctr.save_nextpose(leg * 3 + 1, ik_angles.femur);
            servo_ctr.save_nextpose(leg * 3 + 2, ik_angles.tibia);
        }

        servo_ctr.interpolate_setup(200);

        while( servo_ctr.isInterpolating() )
        {
            servo_ctr.Interpolate_step();

            vTaskDelay(pdMS_TO_TICKS(20));
        }

        init_position = true;
    }

    void perform_body_step( Servo::ServoController &servo_ctr )
    {
        for (uint8_t leg = LEFT_FRONT; leg < NUM_MAX_LEGS; leg++)
        {
            // 1. Leg total
            transformations3D::Vectors::vector3d leg_vect = calculate_total_vector(leg);

            // 2. tcoxa
            transformations3D::Vectors::vector3d coxa_vect_t = tbody.apply(coxa_endpoints[leg]);

            // 3. Vector total y transformaciones
            leg_vect = recalculate_leg_vector( coxa_vect_t, leg_vect );

            // 4. Resolver cinemática inversa y guardar posiciones
            hexapod::Hexaik::ik_angles ik_angles = hexapod::Hexaik::legIK(
                static_cast<leg_id>(leg), leg_vect.x, leg_vect.y, leg_vect.z);

            // 5. Send new pose to servos
            servo_ctr.save_nextpose(leg * 3, ik_angles.coxa);
            servo_ctr.save_nextpose(leg * 3 + 1, ik_angles.femur);
            servo_ctr.save_nextpose(leg * 3 + 2, ik_angles.tibia);
        }

        servo_ctr.interpolate_setup(100);
    }

    bool isMoving() // refactorizar, lo veo obsoleto, hay otras maneras mejores de hacerlo
	{
		return  ( tbody.t_x > 1.0f || tbody.t_x < -1.0f ) || 
				( tbody.t_y > 1.0f || tbody.t_y < -1.0f ) ||
                ( tbody.t_z > 1.0f || tbody.t_z < -1.0f ) ||
                ( tbody.rot_x > 0.018f || tbody.rot_x < -0.018f ) || 
                ( tbody.rot_y > 0.018f || tbody.rot_y < -0.018f ) || 
                ( tbody.rot_z > 0.018f || tbody.rot_z < -0.018f ); //0.018f -> 1 grado 
	}

    void init_tbody(void)
    {
        tbody.reset();
    }

    void hexa_body_step( Servo::ServoController& servo_ctr )
    {
        if ( !isMoving() )
        {
            init_tbody();

            if ( !init_position )
            {
                resetToInitPosition(servo_ctr);
            }
        }
        else
        {
            init_position = false;

            if ( !servo_ctr.isInterpolating() )
            {
                perform_body_step( servo_ctr );
            }

            servo_ctr.Interpolate_step();
        }
    }

    void body_control_init( void )
    {
        body_control_interface_init(&tbody);
    }


}
