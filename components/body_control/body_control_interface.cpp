
#include "tmatrix.h"
#include "body_control_interface.h"
#include <esp_log.h>

static transformations3D::Tmatrix* tbody;

void body_control_interface_init(transformations3D::Tmatrix* tbody_obj)
{
    // Asignar el puntero recibido al controlador global
    tbody = tbody_obj;
}

void update_traslation_parameters( float x, float y, float z )
{
    if (!tbody)
    {
        ESP_LOGE("BODY_CONTROL_INTERFACE", "Body controller not initialized!");
        return; // Evita llamadas si no está inicializado
    }

    tbody->t_x = x;
    tbody->t_y = y;
    tbody->t_z = z;
}

void update_rotation_parameters( float rot_x, float rot_y, float rot_z )
{
    if (!tbody)
    {
        ESP_LOGE("BODY_CONTROL_INTERFACE", "Body controller not initialized!");
        return; // Evita llamadas si no está inicializado
    }

    tbody->rot_x = rot_x;
    tbody->rot_y = rot_y;
    tbody->rot_z = rot_z;
}