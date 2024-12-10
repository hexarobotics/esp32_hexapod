


// aquí implementaremos get_parameters()

// 1. Leer parametros que vienen de la app
//

#include "control_mode_interface.h"


static control_mode_e* p_modo_control;

void control_mode_interface_init(control_mode_e* p_mode)
{
    p_modo_control = p_mode;
}


extern "C" {

    void set_control_mode( control_mode_e new_mode )
    {
        *p_modo_control = new_mode;
    }
}
