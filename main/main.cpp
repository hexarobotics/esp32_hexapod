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
#include "driver/uart.h"
#include <stdio.h>

using namespace transformations3D;
using namespace hexapod;

#define ADDR PCA9685_ADDR_BASE
#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

#define UART_NUM_UART0 UART_NUM_0
#define UART_BAUD_RATE 115200
#define UART_DATA_BITS UART_DATA_8_BITS
#define UART_PARITY UART_PARITY_DISABLE
#define UART_STOP_BITS UART_STOP_BITS_1

#define TXD_PIN 1 // 16
#define RXD_PIN 3 // 17

static const char *TAG = "pca9685_test";
static const char *UART_TAG = "uart_test";


// Tamaño máximo de los buffers de recepción y envío
#define BUFFER_SIZE 256

// Buffers estáticos para recibir y enviar datos
static uint8_t rx_buffer[BUFFER_SIZE];
static uint8_t tx_buffer[BUFFER_SIZE];

void uart_init() {
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_BITS,
        .parity = UART_PARITY,
        .stop_bits = UART_STOP_BITS,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    	.rx_flow_ctrl_thresh = 0, // Initialize rx_flow_ctrl_thresh to 0
        .source_clk = UART_SCLK_APB,
    };
    
    uart_param_config(UART_NUM_UART0, &uart_config);
    uart_driver_install(UART_NUM_UART0, BUFFER_SIZE, BUFFER_SIZE, 0, NULL, 0);
    uart_set_pin(UART_NUM_UART0, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

// Tarea para manejar la comunicación UART
void uart_task(void *pvParameters) 
{
    uart_init();
    
    ESP_LOGI(UART_TAG, "UART Task initialization");
	
    while (1)
    {
        //ESP_LOGI(UART_TAG, "UART Task execution 100ms");
		
		memset(rx_buffer, 0, BUFFER_SIZE); // clean first
		memset(tx_buffer, 0, BUFFER_SIZE); // clean first
        // Leer datos de la UART
        int len = uart_read_bytes(UART_NUM_UART0, rx_buffer, BUFFER_SIZE,pdMS_TO_TICKS(100));
        if (len > 0) 
        {
            // Procesar los datos recibidos
            // ... (aquí colocarás tu lógica para procesar los comandos)
            //printf("Received: %.*s\n", len, rx_buffer);
            //ESP_LOGI(UART_TAG, "Bytes read: %d", len);
            
	        //ESP_LOGI(UART_TAG, "len: %d, msg: %s",len,rx_buffer);

            // Preparar la respuesta
            //sprintf((char*)tx_buffer, "Received %d bytes: %.*s\n", len, len, rx_buffer);
            
            // Preparar la respuesta
			int tx_len = snprintf((char*)tx_buffer, BUFFER_SIZE, "Received %d bytes: %.*s\n", len, len, rx_buffer);
            // Enviar la respuesta
			uart_write_bytes(UART_NUM_UART0, (const char*)tx_buffer, tx_len);

            //uart_write_bytes(UART_NUM_UART0, (const char*)tx_buffer, strlen((const char *)tx_buffer));
        }
        


        
    	vTaskDelay(pdMS_TO_TICKS(50));
    }
}





void pca9685_test_task(void *pvParameters)
{
    ESP_LOGI(TAG, "pca9685 Task initialization");

    Servo::ServoManager servo_mngr;
    servo_mngr.Init();

    Vectors::vector3d leg_end_example[3];

    leg_end_example[0] = {-155, 80, -120};
    leg_end_example[1] = {-155, -80, -120}; // x, y, z
    leg_end_example[2] = {-155, 0, -70};

    Hexaik hexaik;

    while (1)
    {
        //ESP_LOGI(TAG, "Test task execution");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C"  void app_main()
{
	ESP_LOGI(TAG, "Free heap size before pca9685_test_task: %lu", esp_get_free_heap_size());
	
	if(xTaskCreatePinnedToCore(pca9685_test_task, TAG, 4096, NULL, 5, NULL, APP_CPU_NUM) != pdPASS)
	{
		ESP_LOGI(TAG, "Free heap size after pca9685_test_task: %lu", esp_get_free_heap_size());
	    ESP_LOGE(TAG, "Failed to create pca9685_test_task");
	}
    
    if(xTaskCreate(uart_task, UART_TAG, 4096, NULL, 6, NULL) != pdPASS )
    {
	    ESP_LOGE(UART_TAG, "Failed to create uart_task");
	}

	ESP_LOGI(TAG, "Free heap size after creating tasks: %lu", esp_get_free_heap_size());
}
