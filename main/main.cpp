#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
//#include <pca9685.h>
#include <string.h>
#include <esp_log.h>
#include <stdint.h>

#include "nvs_flash.h"


#include "servo_manager.h"
#include "hexaik.h"
#include "vectors.h"

extern "C" {
	#include "wifi_app.h"
}

using namespace transformations3D;
using namespace hexapod;

#define ADDR PCA9685_ADDR_BASE
#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

static const char *TAG = "pca9685_test";

void pca9685_test_task(void *pvParameters)
{
	
    // Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
	
	// Start Wifi
	wifi_app_start();
	
    Servo::ServoManager servo_mngr;
    servo_mngr.Init();

    Vectors::vector3d leg_end_example[3];

    leg_end_example[0] = {-155, 80, -120};
    leg_end_example[1] = {-155, -80, -120}; // x, y, z
    leg_end_example[2] = {-155, 0, -70};

    Hexaik hexaik;

    while (1)
    {

        for (int i = 0; i<3;i++)
        {
            Hexaik::ik_angles res = hexaik.do_1_leg_ik( leg_end_example[i], Hexaik::left_middle);

            servo_mngr.set_angle(0,res.coxa);
            servo_mngr.set_angle(1,res.femur);
            servo_mngr.set_angle(2,res.tibia);

            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

extern "C"  void app_main()
{
    xTaskCreatePinnedToCore(pca9685_test_task, TAG, configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL, APP_CPU_NUM);
}
