#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <pca9685.h>
#include <string.h>
#include <esp_log.h>
#include <stdint.h>

#include "servo_manager.h"

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

    while (1)
    {
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
    }
}

extern "C"  void app_main()
{
    xTaskCreatePinnedToCore(pca9685_test_task, TAG, configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL, APP_CPU_NUM);
}
