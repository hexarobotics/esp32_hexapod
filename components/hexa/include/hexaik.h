#ifndef __HEXAIK_H__
  #define __HEXAIK_H__

#include <cstdint>
#include <cmath>

#include "vectors.h"

//	###			KINEMATIC VARIABLES		###		//
//	********************************		//

/* Body
* We assume 4 legs are on the corners of a box defined by X_COXA x Y_COXA
* Middle legs for a hexapod can be different Y, but should be halfway in X
*/
#define X_COXA  60  // MM between front and back legs /2
#define Y_COXA  60  // MM between front/back legs /2
#define MX_COXA  100  // MM between two middle legs /2

/* Legs */
#define L_COXA      48  // MM distance from coxa servo to femur servo
#define L_FEMUR     75 // MM distance from femur servo to tibia servo
#define L_TIBIA     135 // MM distance from tibia servo to foot

/**
 * @brief Hexapod kinematics library
 * 
 */
namespace hexapod
{
    class Hexaik : public transformations3D::Vectors
    {
        public:
            struct ik_sol_t
            {
                int coxa;
                int femur;
                int tibia;
            };

            static const uint8_t num_legs = 6;

            Hexaik();

        private:
            vector3d leg_endpoints[num_legs];

            ik_sol_t legIK(int X, int Y, int Z, int leg);

            // Función para convertir radianes a grados
            inline float radians_to_degrees(float radians)
            {
                return radians * (180.0f / M_PI);
            }
    };
};


#endif // __HEXAIK_H__
