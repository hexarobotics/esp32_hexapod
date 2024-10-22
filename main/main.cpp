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

Vectors::vector3d g_leg_pos;

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
    
    static const uint8_t frame_len = 8;
	
    while (1)
    {
        // Limpiar los buffers
		memset(rx_buffer, 0, BUFFER_SIZE); 
		memset(tx_buffer, 0, BUFFER_SIZE); 
        
        // Leer datos de la UART
        int len = uart_read_bytes(UART_NUM_UART0, rx_buffer, BUFFER_SIZE, pdMS_TO_TICKS(100));

        // Si se recibieron datos
        if (len >= frame_len) 
        {
            // Extraer el header (primeros 2 bytes) y las coordenadas
            uint16_t header = (rx_buffer[0] << 8) | rx_buffer[1];
                
            if (header == 0xABCD) 
            {
                // Extraer las coordenadas X, Y, Z (en formato int16_t)
                int16_t x = (int16_t)((rx_buffer[2] << 8) | rx_buffer[3]);
                int16_t y = (int16_t)((rx_buffer[4] << 8) | rx_buffer[5]);
                int16_t z = (int16_t)((rx_buffer[6] << 8) | rx_buffer[7]);
                
                g_leg_pos.x = (double)x;
                g_leg_pos.y = (double)y;
                g_leg_pos.z = (double)z;
                
                // Log para mostrar los valores recibidos
                ESP_LOGI(UART_TAG, "Received coordinates: X = %.2f, Y = %.2f, Z = %.2f", g_leg_pos.x, g_leg_pos.y, g_leg_pos.z);
                
                // Preparar la respuesta
                //int tx_len = snprintf((char*)tx_buffer, BUFFER_SIZE,"Received: X = %.2f, Y = %.2f, Z = %.2f\n", g_leg_pos.x, g_leg_pos.y, g_leg_pos.z);
                
                //uart_write_bytes(UART_NUM_UART0, (const char*)tx_buffer, tx_len);
            } 
            else
            {
                // Header no coincide
                ESP_LOGI(UART_TAG, "Invalid header: 0x%04X", header);
                //const char *invalid_header_msg = "Invalid header\n";
                //uart_write_bytes(UART_NUM_UART0, invalid_header_msg, strlen(invalid_header_msg));
            }
        }

        vTaskDelay(pdMS_TO_TICKS(500)); // Pausa de 50ms
    }
}





void pca9685_test_task(void *pvParameters)
{
    ESP_LOGI(TAG, "pca9685 Task initialization");

    Servo::ServoManager servo_mngr;
    servo_mngr.Init();

    Hexaik hexaik;

    while (1)
    {
            Hexaik::ik_angles res = hexaik.do_1_leg_ik( g_leg_pos, Hexaik::left_middle);

            servo_mngr.set_angle(0,res.coxa);
            servo_mngr.set_angle(1,res.femur);
            servo_mngr.set_angle(2,res.tibia);
            
            vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C"  void app_main()
{
	g_leg_pos.x = -155.0;
	g_leg_pos.y = 0.0;
	g_leg_pos.z = -70.0;
	
	ESP_LOGI(TAG, "Free heap size before pca9685_test_task: %lu", esp_get_free_heap_size());
	
	if(xTaskCreatePinnedToCore(pca9685_test_task, TAG, 4096, NULL, 6, NULL, APP_CPU_NUM) != pdPASS)
	{
		ESP_LOGI(TAG, "Free heap size after pca9685_test_task: %lu", esp_get_free_heap_size());
	    ESP_LOGE(TAG, "Failed to create pca9685_test_task");
	}
    
    if(xTaskCreate(uart_task, UART_TAG, 4096, NULL, 5, NULL) != pdPASS )
    {
	    ESP_LOGE(UART_TAG, "Failed to create uart_task");
	}

	ESP_LOGI(TAG, "Free heap size after creating tasks: %lu", esp_get_free_heap_size());
}
