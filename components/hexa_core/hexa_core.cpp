
#include "hexa_core.h"
#include "hexa_params.h"
#include "Hexaik.h"
#include "gaits.h"
#include "Servo_controller.h"
#include <esp_log.h>
#include "esp_timer.h"
#include "gaits_control_interface.h"

namespace hexapod
{

// NEWW

const char *HEXA_TASK_TAG = "HEXA Main Task";

// Inicialización estática del array en el .cpp
const transformations3D::Vectors::vector2d coxa_endpoints[6] =
{
    {-X_COXA, Y_COXA},     // PATA 1: LEFT_FRONT
    {-MX_COXA, 0},         // PATA 2: LEFT_MIDDLE
    {-X_COXA, -Y_COXA},    // PATA 3: LEFT_REAR
    {X_COXA, Y_COXA},      // PATA 4: RIGHT_FRONT
    {MX_COXA, 0},          // PATA 5: RIGHT_MIDDLE
    {X_COXA, -Y_COXA}      // PATA 6: RIGHT_REAR
};

const transformations3D::Vectors::vector3d leg_endpoints[6] =
{
    {-115, 100, -100},     // PATA 1: LEFT_FRONT
    {-155, 0, -100},       // PATA 2: LEFT_MIDDLE
    {-115, -100,-100},     // PATA 3: LEFT_REAR
    {115, 100, -100},      // PATA 4: RIGHT_FRONT
    {155, 0, -100},        // PATA 5: RIGHT_MIDDLE
    {115, -100, -100}      // PATA 6: RIGHT_REAR
};

const transformations3D::Vectors::vector3d leg_offsets[6] =
{
    {0, 0, 10},     // PATA 1: LEFT_FRONT
    {0, 0, 0},     // PATA 2: LEFT_MIDDLE
    {0, 0, 0},     // PATA 3: LEFT_REAR
    {0, 0, 2},     // PATA 4: RIGHT_FRONT
    {0, 0, 0},     // PATA 5: RIGHT_MIDDLE
    {0, 0, -8}      // PATA 6: RIGHT_REAR
};

bool init_position;

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

void hexa_core_init(void)
{
	init_position = true;
}

// Restablecer la posición inicial
void resetToInitPosition(Servo::ServoController &servo_ctr)
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

// Realizar un paso de marcha (gait step)
void performGaitStep(hexapod::Gaits &gait, Servo::ServoController &servo_ctr)
{
    for (uint8_t leg = LEFT_FRONT; leg < NUM_MAX_LEGS; leg++)
	{
        // 1. Cálculo del paso de marcha
        transformations3D::Tmatrix tgait = gait.step(leg);

        // 2. Vector total y transformaciones
        transformations3D::Vectors::vector3d leg_vect = calculate_total_vector(leg);
        leg_vect = tgait.apply(leg_vect);
        leg_vect = recalculate_leg_vector(leg, leg_vect);

        // 3. Resolver cinemática inversa y guardar posiciones
        hexapod::Hexaik::ik_angles ik_angles = hexapod::Hexaik::legIK(
            static_cast<leg_id>(leg), leg_vect.x, leg_vect.y, leg_vect.z);

        servo_ctr.save_nextpose(leg * 3, ik_angles.coxa);
        servo_ctr.save_nextpose(leg * 3 + 1, ik_angles.femur);
        servo_ctr.save_nextpose(leg * 3 + 2, ik_angles.tibia);
    }

    // Incrementar paso y configurar interpolación
    gait.next_step();

    servo_ctr.interpolate_setup(gait.get_gait_transition_time());
}

void hexa_main_task(void *pvParameters)
{
    ESP_LOGI(HEXA_TASK_TAG, "hexa_main_task initialization");
    hexa_core_init();

    hexapod::Gaits gait(TRIPOD_6,HEXAPOD);
    // Inicializa la interfaz de control con el objeto Gaits
    gaits_control_interface_init(static_cast<void*>(&gait));

    Servo::ServoController servo_ctr;
    servo_ctr.writePositions();

    uint16_t frame_delay_ms = servo_ctr.get_frame_length_ms();
    ESP_LOGI(HEXA_TASK_TAG, " delay: %d", frame_delay_ms);

    vTaskDelay(pdMS_TO_TICKS(1000));

    while (true)
	{
        if ( !gait.isMoving() )
		{
            gait.init_tgaits();

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
                performGaitStep(gait, servo_ctr);
            }

            servo_ctr.Interpolate_step();
        }

        vTaskDelay(pdMS_TO_TICKS(frame_delay_ms));
    }


	while(0) // TESTING 2 Poses with interpolation process
	{
        for( int8_t leg = LEFT_FRONT; leg < NUM_MAX_LEGS; leg++ )
        {
            ESP_LOGI(HEXA_TASK_TAG," Writing pose 1" );


            servo_ctr.save_nextpose(leg * 3, 	 90 ); // coxa  // 1. Pose all 0
            servo_ctr.save_nextpose(leg * 3 + 1, 90 ); // femur  // 2. Set next_pose_ values
            servo_ctr.save_nextpose(leg * 3 + 2, 45 ); // tibia

        }

        //servo_ctr.save_nextpose(3 * 3 + 1, 	 0 ); // coxa  // 1. Pose all 0


        //servo_ctr.writePositions(); 

        servo_ctr.interpolate_setup(1000);

        while (servo_ctr.isInterpolating())
        {
            servo_ctr.Interpolate_step();
        }

        vTaskDelay(pdMS_TO_TICKS(2000));


        //for( int8_t leg = RIGHT_REAR; leg < NUM_MAX_LEGS; leg++ )
        //{
        //    ESP_LOGI(HEXA_TASK_TAG," Writing pose 2" );
//
//
        //    servo_ctr.save_nextpose(leg * 3, 	 95 );
        //    servo_ctr.save_nextpose(leg * 3 + 1, 90 );
        //    servo_ctr.save_nextpose(leg * 3 + 2, 50 );
//
        //}
//
        ////servo_ctr.writePositions();
//
        //servo_ctr.interpolate_setup(1000);
//
        //while (servo_ctr.isInterpolating())
        //{
        //    servo_ctr.Interpolate_step();
        //}
//
        //vTaskDelay(pdMS_TO_TICKS(2000));

	}


}



}