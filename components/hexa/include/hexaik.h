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

            Hexaik();

        private:
            vector3d leg_endpoints[num_legs];

            ik_angles legIK         (int X, int Y, int Z, leg_id leg);
            ik_angles real_angle    (leg_id leg, ik_angles angles );
            void      do_ik         ( void );

            // Función para convertir radianes a grados
            inline float radians_to_degrees(float radians)
            {
                return (uint16_t)(radians * (180.0f / M_PI));
            }
    };
};


#endif // __HEXAIK_H__
