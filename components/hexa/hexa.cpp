#include <hexa.h>
#include <cmath>


uint8_t control_cuerpo;
uint8_t control_robot;

namespace hexapod
{
    // Class Hexa
    // Constructor Hexa
    Hexa::Hexa()
    {
	    // Inicializa todos los elementos del arreglo a {0.0, 0.0, 0.0}
	    for (int i = 0; i < num_legs; ++i)
	    {
	        leg_endpoints[i] = Vectors::vector3d(); // Usa el constructor por defecto que inicializa a 0
	    }
    }
    
} // namespace hexapod


/// que he hecho
/// TODO: separar las clases en archivos distintos





void get_ik_initial_parameters()
{
    if ( control_cuerpo == 1 )
    {

    }
    else if ( control_robot == 1 )
    {

    }
}
