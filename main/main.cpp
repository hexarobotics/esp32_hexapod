#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
//#include <pca9685.h>
#include <string.h>
#include <esp_log.h>
#include <stdint.h>

#include "nvs_flash.h"

#include "hexa_params.h"
#include "servo_driver.h"
#include "hexaik.h"
#include "vectors.h"
#include "hexa_core.h"

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

    ESP_LOGI(TAG, "pca9685 Task initialization");

	
    //Servo::ServoDriver servo_mngr;
    //servo_mngr.Init();
//
    //Vectors::vector3d leg_end_example[3];
//
    //leg_end_example[0] = {-155, 80, -120};
    //leg_end_example[1] = {-155, -80, -120}; // x, y, z
    //leg_end_example[2] = {-155, 0, -70};
//
    //Hexaik hexaik;

    while (1)
    {
        //ESP_LOGI(TAG, "PCA Task Alive");

        //for (int i = 0; i<3;i++)
        //{
        //    Hexaik::ik_angles res = hexaik.do_1_leg_ik( leg_end_example[i], LEFT_MIDDLE);
//
        //    servo_mngr.set_angle(0,res.coxa);
        //    servo_mngr.set_angle(1,res.femur);
        //    servo_mngr.set_angle(2,res.tibia);
//
            vTaskDelay(pdMS_TO_TICKS(1000));
        //}
    }
}

extern "C"  void app_main()
{
    xTaskCreatePinnedToCore(pca9685_test_task, TAG, configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL, APP_CPU_NUM);

    if (xTaskCreatePinnedToCore(
            hexa_main_task,                     // Función de la tarea
            HEXA_TASK_TAG,                      // Nombre de la tarea
            configMINIMAL_STACK_SIZE * 3,       // Tamaño de la pila (en palabras)
            NULL,                               // Parámetro de entrada
            6,                                  // Prioridad de la tarea
            NULL,                               // Puntero a manejar la tarea (opcional)
            APP_CPU_NUM)                        // Núcleo al que se asignará
        != pdPASS)
    {
        ESP_LOGE("APP_MAIN", "Failed to create HexapodTask");
    }

}
