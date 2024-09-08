#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <pca9685.h>
#include <string.h>
#include <esp_log.h>
#include <stdint.h>

#include "servo_manager.h"
#include "hexaik.h"
#include "vectors.h"

using namespace transformations3D;
using namespace hexapod;

#define ADDR PCA9685_ADDR_BASE
#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

static const char *TAG = "pca9685_test";

void pca9685_test_task(void *pvParameters)
{
    //hello task
    Servo::ServoManager servo_mngr;
    servo_mngr.Init();

    Vectors::vector3d leg_end_example[3];

    leg_end_example[0] = {-155, 80, -100}; // x, y, z
    leg_end_example[1] = {-155, 0, -70};
    leg_end_example[2] = {-155, -80, -100};

    Hexaik hexaik;

    while (1)
    {

        //for (int i = 0; i<3;i++)
        //{
        //    Hexaik::ik_angles res = hexaik.do_1_leg_ik( leg_end_example[i], Hexaik::left_middle);
//
        //    servo_mngr.set_angle(0,res.coxa);
        //    servo_mngr.set_angle(1,res.femur);
        //    servo_mngr.set_angle(2,res.tibia);
//
        //    vTaskDelay(pdMS_TO_TICKS(2000));
        //}



        servo_mngr.set_angle(2,120); 

        vTaskDelay(pdMS_TO_TICKS(2000));



        /*

        for (uint16_t angle = 0; angle <= 180; angle+=10 )
        {
            vTaskDelay(pdMS_TO_TICKS(500));
            servo_mngr.set_angle(0,angle);
        }
        
        for (uint16_t angle = 180; angle <= 0; angle-=10 )
        {
            vTaskDelay(pdMS_TO_TICKS(500));
            servo_mngr.set_angle(0,angle);
        }
        
        servo_mngr.set_angle(0,0);
        vTaskDelay(pdMS_TO_TICKS(2000));

        servo_mngr.set_angle(0,180);
        vTaskDelay(pdMS_TO_TICKS(2000));

        */
    }
}

extern "C"  void app_main()
{
    xTaskCreatePinnedToCore(pca9685_test_task, TAG, configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL, APP_CPU_NUM);
}
