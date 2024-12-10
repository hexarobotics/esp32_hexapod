#ifndef GAITS_CONTROL_MODE_H
    #define GAITS_CONTROL_MODE_H

#include "gaits.h"
#include "Servo_controller.h"
#include "hexaik.h"

namespace hexapod
{
    void hexa_gait_step( hexapod::HexaIK ik_solver, hexapod::Gaits& gait, Servo::ServoController& servo_ctr );
}

#endif // GAITS_CONTROL_MODE_H