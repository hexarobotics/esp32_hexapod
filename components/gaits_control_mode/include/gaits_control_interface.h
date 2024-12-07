#ifndef GAITS_CONTROL_INTERFACE_H
#define GAITS_CONTROL_INTERFACE_H

#include <stdint.h> // Para los tipos int16_t

#ifdef __cplusplus
extern "C" {
#endif

// Inicializa la interfaz con un objeto Gaits
void gaits_control_interface_init(void* gaits_obj);

// Actualiza las velocidades X, Y, Z
void gaits_control_interface_set_speeds(int16_t x, int16_t y, int16_t z);

void gaits_control_interface_set_gait_mode( int gait_mode );

#ifdef __cplusplus
}
#endif

#endif // GAITS_CONTROL_INTERFACE_H
