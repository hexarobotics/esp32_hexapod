


// aquí implementaremos get_parameters()

// 1. Leer parametros que vienen de la app
//

#include "hexa_control.h"

namespace hexapod
{
    static control_mode_e modo_control = C_MODE_GAITS;

    control_mode_e get_control_mode(void)
    {
        return modo_control;
    }


    control_mode_e set_control_mode( control_mode_e new_mode )
    {
        modo_control = new_mode;
    }

}