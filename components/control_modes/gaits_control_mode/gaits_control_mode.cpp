
#include "hexa_params.h"
#include "gaits_control_mode.h"
//#include <esp_log.h>
//#include "esp_timer.h"
//#include "gaits_control_interface.h"

namespace hexapod
{
    bool init_position = true;

    transformations3D::Vectors::vector3d calculate_total_vector( uint8_t leg )
    {
        transformations3D::Vectors::vector3d v_out;

        v_out.x = coxa_endpoints[leg].x + leg_endpoints[leg].x + leg_offsets[leg].x;
        v_out.y = coxa_endpoints[leg].y + leg_endpoints[leg].y + leg_offsets[leg].y;
        v_out.z = leg_endpoints[leg].z  + leg_offsets[leg].z;
        
        return v_out;
    }

    transformations3D::Vectors::vector3d recalculate_leg_vector( uint8_t leg, transformations3D::Vectors::vector3d in_vect )
    {
        transformations3D::Vectors::vector3d v_out;

        v_out.x = in_vect.x - coxa_endpoints[leg].x;
        v_out.y = in_vect.y - coxa_endpoints[leg].y;
        v_out.z = in_vect.z;
        
        return v_out;
    }

    // Restablecer la posición inicial
    void resetToInitPosition(hexapod::HexaIK ik_solver, Servo::ServoController &servo_ctr)
    {
        HexaIK::ik_ret_e_t ret;
        HexaIK::Angles leg_angles;

        for (uint8_t leg = LEFT_FRONT; leg < NUM_MAX_LEGS; leg++)
        {
            // 1. Vector total y transformaciones
            transformations3D::Vectors::vector3d leg_vect;

            leg_vect.x = leg_endpoints[leg].x + leg_offsets[leg].x;
            leg_vect.y = leg_endpoints[leg].y + leg_offsets[leg].y;
            leg_vect.z = leg_endpoints[leg].z + leg_offsets[leg].z;

            // 2. Resolver cinemática inversa y guardar posiciones
            ret = ik_solver.do_1_leg_ik( leg_angles, static_cast<ik_leg_id>(leg), leg_vect.x, leg_vect.y, leg_vect.z );

            if ( ret != HexaIK::IK_SUCCESS )
            {
                return;
            }

            // 3. Guardar nextpose
            servo_ctr.save_nextpose(leg * 3, leg_angles.coxa);
            servo_ctr.save_nextpose(leg * 3 + 1, leg_angles.femur);
            servo_ctr.save_nextpose(leg * 3 + 2, leg_angles.tibia);
        }

        servo_ctr.interpolate_setup(200);

        while( servo_ctr.isInterpolating() )
        {
            servo_ctr.Interpolate_step();

            vTaskDelay(pdMS_TO_TICKS(20));
        }

        init_position = true;
    }

    // Realizar un paso de marcha (gait step)
    void performGaitStep( hexapod::HexaIK ik_solver, hexapod::Gaits &gait, Servo::ServoController &servo_ctr )
    {
        HexaIK::ik_ret_e_t ret;
        HexaIK::Angles leg_angles;

        for (uint8_t leg = LEFT_FRONT; leg < NUM_MAX_LEGS; leg++)
        {
            // 1. Cálculo del paso de marcha
            transformations3D::Tmatrix tgait = gait.step(leg);

            // 2. Vector total y transformaciones
            transformations3D::Vectors::vector3d leg_vect = calculate_total_vector(leg);
            leg_vect = tgait.apply(leg_vect);
            leg_vect = recalculate_leg_vector(leg, leg_vect);

            // 3. Resolver cinemática inversa y guardar posiciones
            ret = ik_solver.do_1_leg_ik( leg_angles, static_cast<ik_leg_id>(leg), leg_vect.x, leg_vect.y, leg_vect.z );

            if ( ret != HexaIK::IK_SUCCESS )
            {
                return;
            }

            servo_ctr.save_nextpose(leg * 3, leg_angles.coxa);
            servo_ctr.save_nextpose(leg * 3 + 1, leg_angles.femur);
            servo_ctr.save_nextpose(leg * 3 + 2, leg_angles.tibia);
        }

        // Incrementar paso y configurar interpolación
        gait.next_step();

        servo_ctr.interpolate_setup(gait.get_gait_transition_time());
    }


    void hexa_gait_step( hexapod::HexaIK ik_solver, hexapod::Gaits& gait, Servo::ServoController& servo_ctr )
    {
        if ( !gait.isMoving() )
        {
            gait.init_tgaits();

            if ( !init_position )
            {
                resetToInitPosition( ik_solver,servo_ctr );
            }
        }
        else
        {
            init_position = false;

            if ( !servo_ctr.isInterpolating() )
            {
                performGaitStep( ik_solver, gait, servo_ctr);
            }

            servo_ctr.Interpolate_step();
        }
    }
}
