#ifndef HEXA_GAITS_MODE_H
    #define HEXA_GAITS_MODE_H

#include "gaits.h"
#include "Servo_controller.h"

namespace hexapod
{

    void hexa_gait_step( hexapod::Gaits& gait, Servo::ServoController& servo_ctr );


}

#endif // HEXA_GAITS_MODE_H