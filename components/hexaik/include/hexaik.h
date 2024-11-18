#ifndef __HEXAIK_H__
  #define __HEXAIK_H__

#include <cstdint>
#include <cmath>

#include "vectors.h"
#include "hexa_params.h"

/**
 * @brief Hexapod kinematics library
 * 
 */
namespace hexapod
{
    class Hexaik : public transformations3D::Vectors
    {
        public:
            struct ik_angles
            {
                int coxa;
                int femur;
                int tibia;
            };

            static const uint8_t num_legs = 6;
            static const char* TAG;

            Hexaik();

            static ik_angles   legIK         ( leg_id leg, int X, int Y, int Z );
            ik_angles          do_1_leg_ik   ( Vectors::vector3d vec, leg_id leg  );

        private:
            vector3d leg_endpoints[num_legs]; // no usada

            void        do_ik         ( void );
            static int radians_to_degrees(double radians);
            static ik_angles   real_angle    ( leg_id leg, ik_angles angles );
    };
};


#endif // __HEXAIK_H__
