
#include "hexa_task.h"
#include "hexa_params.h"
#include "Hexaik.h"
#include "gaits.h"
#include "Servo_controller.h"
#include <esp_log.h>
#include "esp_timer.h"
#include "gaits_control_interface.h"
#include "hexa_gaits_mode.h"
#include "hexa_control_mode_interface.h"


namespace hexapod
{
    const char *HEXA_TASK_TAG = "HEXA Main Task";

    void hexa_main_task(void *pvParameters)
    {
        ESP_LOGI(HEXA_TASK_TAG, "hexa_main_task initialization");

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
            control_mode_e control_mode = get_control_mode();

            if ( control_mode == C_MODE_GAITS )
            {
                hexa_gait_step( gait, servo_ctr );

                vTaskDelay(pdMS_TO_TICKS(frame_delay_ms));
            }
            else if ( control_mode == C_MODE_BODY )
            {

            }



        }

    }

}
