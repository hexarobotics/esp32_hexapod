#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
//#include <pca9685.h>
#include <string.h>
#include <esp_log.h>
#include <stdint.h>

//#include "esp_system.h"
//#include "esp_private/esp_clk.h"
//#include "esp_cpu.h"
#include "nvs_flash.h"

#include "hexa_params.h"
#include "servo_driver.h"
#include "hexaik.h"
#include "vectors.h"
#include "hexa_task.h"

extern "C" {
	#include "wifi_app.h"
}

using namespace transformations3D;
using namespace hexapod;

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

extern "C"  void app_main()
{
    //ESP_LOGI(HEXA_TASK_TAG, "CPU Frequency: %d MHz", esp_clk_cpu_freq());
    //ESP_LOGI("CPU_INFO", "Máxima frecuencia soportada: %d MHz", rtc_clk_cpu_freq_get_max() / 1000000);

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

    //xTaskCreatePinnedToCore(pca9685_test_task, TAG, configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL, APP_CPU_NUM);

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
