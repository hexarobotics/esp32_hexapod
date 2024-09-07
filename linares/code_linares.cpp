

#include "cmath"
#include "stdint-gcc.h"

#include "code_linares.h"

void do_ik( void ) // hex.cpp -> hex_compute_step() (quitar de ik.cpp)
{
    for(uint8_t n_pata=0; n_pata<6;n_pata++)
    {
        Gait_generator(n_pata); // gait.cpp file
        //body_ik -> ahora llamado Tmatrix_apply o algo asi, la matriz de T homogenea. ik.cpp file
        legIK(); //ik.cpp file
        check_angles_range_and_save_to_send_servos(); // servos_and_i2c.cpp file

        uint16_t gait_step++;
        if (step>stepsInCycle)step=1;
    }
}



