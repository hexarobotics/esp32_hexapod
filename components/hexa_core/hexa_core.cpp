
#include "hexa_core.h"
#include "hexa_params.h"
#include "Hexaik.h"
#include "gaits.h"
#include "Servo_controller.h"
#include <esp_log.h>
#include "esp_timer.h"

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

void hexa_main_task(void *pvParameters)
{
	ESP_LOGI(HEXA_TASK_TAG, "hexa_main_task initialization");

	hexapod::Gaits gait(TRIPOD_24);

	Servo::ServoController servo_ctr;
    servo_ctr.writePositions();

	uint16_t delay_ms = servo_ctr.get_frame_length_ms();
	ESP_LOGI(HEXA_TASK_TAG," delay: %d", delay_ms );
	// more tasks?


	gait.set_xspeed(0);
	gait.set_yspeed(0);
	gait.set_rspeed(30000);

	vTaskDelay(pdMS_TO_TICKS(1000));

	static int64_t last_time = esp_timer_get_time();
	static int64_t new_time;

    while(1)
    {
		//new_time = esp_timer_get_time() / 1000;
		//ESP_LOGI(HEXA_TASK_TAG," tiempo pasado(ms): %lld", ( new_time - last_time ) );

        // 1. get_parameters: leemos de la app los comandos de movimiento
        // 2. is_moving()?
        // 3. calculate_gait_step

        //get_parameters()
		//ESP_LOGI(HEXA_TASK_TAG,"Interpolando: %d", servo_ctr.isInterpolating());

        if ( !servo_ctr.isInterpolating() )//&& gait.isMoving())
        {
            for( uint8_t leg = LEFT_FRONT; leg < NUM_MAX_LEGS; leg++ )
            {
				//ESP_LOGI(HEXA_TASK_TAG, "HEXA iterating legs: START");
				//ESP_LOGI(HEXA_TASK_TAG," --- LEG: %d ---", leg );

				// 1. Gait step calculation
                transformations3D::Tmatrix tgait = gait.step(leg);

				//ESP_LOGI(HEXA_TASK_TAG,"T gait x: %f", tgait.t_x);
				//ESP_LOGI(HEXA_TASK_TAG,"T gait y: %f", tgait.t_y);
				//ESP_LOGI(HEXA_TASK_TAG,"T gait z: %f", tgait.t_z);
				//ESP_LOGI(HEXA_TASK_TAG,"T gait r: %f", tgait.rot_z);

				// 2. Total Vect
                transformations3D::Vectors::vector3d leg_vect = calculate_total_vector( leg );

				//ESP_LOGI(HEXA_TASK_TAG,"endpoint x: %f", leg_endpoints[leg].x);
				//ESP_LOGI(HEXA_TASK_TAG,"endpoint y: %f", leg_endpoints[leg].y);
				//ESP_LOGI(HEXA_TASK_TAG,"endpoint z: %f", leg_endpoints[leg].z);
				//ESP_LOGI(HEXA_TASK_TAG,"endpoint r: %f", tgait.rot_z);
				//ESP_LOGI(HEXA_TASK_TAG,"coxa x: %f", coxa_endpoints[leg].x);
				//ESP_LOGI(HEXA_TASK_TAG,"coxa y: %f", coxa_endpoints[leg].y);

				//ESP_LOGI(HEXA_TASK_TAG,"TOTAL Vect x: %f", leg_vect.x);
				//ESP_LOGI(HEXA_TASK_TAG,"TOTAL Vect y: %f", leg_vect.y);
				//ESP_LOGI(HEXA_TASK_TAG,"TOTAL Vect z: %f", leg_vect.z);

				// 3. Apply Transformation matrix to leg_vector
                leg_vect = tgait.apply(leg_vect); // vector modificado

				//ESP_LOGI(HEXA_TASK_TAG,"TOTAL Vect x trans: %f", leg_vect.x);
				//ESP_LOGI(HEXA_TASK_TAG,"TOTAL Vect y trans: %f", leg_vect.y);
				//ESP_LOGI(HEXA_TASK_TAG,"TOTAL Vect z trans: %f", leg_vect.z);

				// 4. Calculate leg vector
                leg_vect = recalculate_leg_vector( leg, leg_vect );

				//ESP_LOGI(HEXA_TASK_TAG,"Vect x trans: %f", leg_vect.x);
				//ESP_LOGI(HEXA_TASK_TAG,"Vect y trans: %f", leg_vect.y);
				//ESP_LOGI(HEXA_TASK_TAG,"Vect z trans: %f", leg_vect.z);

				// 5. Solve IK
                hexapod::Hexaik::ik_angles ik_angles = hexapod::Hexaik::legIK( (leg_id)leg, leg_vect.x, leg_vect.y, leg_vect.z );

				// 6. Save nextpose
				servo_ctr.save_nextpose(leg * 3, 	 ik_angles.coxa );
				servo_ctr.save_nextpose(leg * 3 + 1, ik_angles.femur );
				servo_ctr.save_nextpose(leg * 3 + 2, ik_angles.tibia );

				//ESP_LOGI(HEXA_TASK_TAG, "HEXA iterating legs: END");
            }

            // 7. Set Next Gait step
            gait.next_step(); // Increment gait step

			servo_ctr.interpolate_setup(gait.get_gait_transition_time());
        }

		// update joints
		servo_ctr.Interpolate_step();

		last_time = esp_timer_get_time() / 1000;
		vTaskDelay(pdMS_TO_TICKS(delay_ms));

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