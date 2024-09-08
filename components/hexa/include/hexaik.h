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

            enum leg_id
            {
                left_front,
                left_middle,
                left_rear,
                right_front,
                right_middle,
                right_rear,
                num_max_legs
            };

            static const uint8_t num_legs = 6;
            const char *TAG = "HEXA_IK";

            Hexaik();

            ik_angles   do_1_leg_ik   ( Vectors::vector3d vec, leg_id leg  );

        private:
            vector3d leg_endpoints[num_legs];

            ik_angles   legIK         (int X, int Y, int Z, leg_id leg);
            ik_angles   real_angle    (leg_id leg, ik_angles angles );
            void        do_ik         ( void );

            // Función para convertir radianes a grados
            int radians_to_degrees(double radians);
    };
};


#endif // __HEXAIK_H__
