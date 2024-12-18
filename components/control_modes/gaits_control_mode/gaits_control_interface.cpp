#include "gaits_control_interface.h"
#include "gaits.h" // Incluye la clase Gaits
#include <esp_log.h>


// Instancia global del controlador
static hexapod::Gaits* gaits_controller = nullptr;

// Inicializa el controlador con un objeto Gaits
void gaits_control_interface_init(hexapod::Gaits* gaits_obj)
{
    gaits_controller = gaits_obj;
}

extern "C" {

// Actualiza las velocidades
void gaits_control_interface_set_speeds(int16_t x_speed, int16_t y_speed, int16_t r_speed)
{
    if (!gaits_controller)
    {
        ESP_LOGE("GAITS_CONTROL_INTERFACE", "Gaits controller not initialized!");
        return; // Evita llamadas si no está inicializado
    }
    
    gaits_controller->set_xspeed(x_speed);
    gaits_controller->set_yspeed(y_speed);
    gaits_controller->set_rspeed(r_speed);
}

void gaits_control_interface_set_gait_mode( int new_gait_mode )
{
    gaits_controller->gait_select(static_cast<GaitType>(new_gait_mode));
}


}
