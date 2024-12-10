#ifndef BODY_CONTROL_MODE_H
    #define BODY_CONTROL_MODE_H

#include "Servo_controller.h"
#include "hexaik.h"

namespace hexapod
{
    void hexa_body_step( hexapod::HexaIK ik_solver, Servo::ServoController& servo_ctr );
    void body_control_init( void );

}

#endif // BODY_CONTROL_MODE_H




