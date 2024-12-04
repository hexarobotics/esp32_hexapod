#ifndef __HEXA_PARAMS_H__
  #define __HEXA_PARAMS_H__

#include "vectors.h"

/* Body
* We assume 4 legs are on the corners of a box defined by X_COXA x Y_COXA
* Middle legs for a hexapod can be different Y, but should be halfway in X
*/
#define X_COXA  60  // MM between front and back legs /2
#define Y_COXA  60  // MM between front/back legs /2
#define MX_COXA  100  // MM between two middle legs /2

/* Legs */
#define L_COXA_OLD      48  // MM distance from coxa servo to femur servo
#define L_FEMUR     75 // MM distance from femur servo to tibia servo
#define L_TIBIA     135 // MM distance from tibia servo to foot
#define L_COXA      44  // MM distance from coxa servo to femur servo

namespace hexapod
{
  enum leg_id
  {
      LEFT_FRONT = 0,
      LEFT_MIDDLE, 
      LEFT_REAR,
      RIGHT_FRONT, 
      RIGHT_MIDDLE,
      RIGHT_REAR,
      NUM_MAX_LEGS  
  };

  // Inicialización estática del array en el .cpp
  const transformations3D::Vectors::vector3d coxa_endpoints[6] =
  {
    {-X_COXA, Y_COXA, 0},     // PATA 1: LEFT_FRONT
    {-MX_COXA, 0, 0},         // PATA 2: LEFT_MIDDLE
    {-X_COXA, -Y_COXA, 0},    // PATA 3: LEFT_REAR
    {X_COXA, Y_COXA, 0},      // PATA 4: RIGHT_FRONT
    {MX_COXA, 0, 0},          // PATA 5: RIGHT_MIDDLE
    {X_COXA, -Y_COXA, 0}      // PATA 6: RIGHT_REAR
  };

  const transformations3D::Vectors::vector3d leg_endpoints[6] =
  {
    {-105, 100, -100},     // PATA 1: LEFT_FRONT
    {-145, 0, -100},       // PATA 2: LEFT_MIDDLE
    {-105, -100,-100},     // PATA 3: LEFT_REAR
    {105, 100, -100},      // PATA 4: RIGHT_FRONT
    {145, 0, -100},        // PATA 5: RIGHT_MIDDLE
    {105, -100, -100}      // PATA 6: RIGHT_REAR
  };

  const transformations3D::Vectors::vector3d leg_offsets[6] =
  {
    {0, 0, 10},     // PATA 1: LEFT_FRONT
    {0, 0, 0},     // PATA 2: LEFT_MIDDLE
    {0, 0, 0},     // PATA 3: LEFT_REAR
    {0, 0, 2},     // PATA 4: RIGHT_FRONT
    {0, 0, 0},     // PATA 5: RIGHT_MIDDLE
    {0, 0, -8}      // PATA 6: RIGHT_REAR
  };

}

#endif // __HEXA_PARAMS_H__
