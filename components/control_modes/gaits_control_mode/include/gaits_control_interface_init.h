#ifndef GAITS_CONTROL_INTERFACE_INIT_H
#define GAITS_CONTROL_INTERFACE_INIT_H

#include <stdint.h> // Para los tipos int16_t
#include "gaits.h" // Incluir solo en C++

// Función de inicialización solo disponible en C++
void gaits_control_interface_init(hexapod::Gaits* gaits_obj);

#endif // GAITS_CONTROL_INTERFACE_INIT_H
