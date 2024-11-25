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

/* Servo Legs*/
#define RF_COXA 1
#define RF_FEMUR 2
#define RF_TIBIA 3
//
#define RR_COXA 4
#define RR_FEMUR 5
#define RR_TIBIA 6
//
#define LF_COXA 7
#define LF_FEMUR 8
#define LF_TIBIA 9
//
#define LR_COXA 10
#define LR_FEMUR 11
#define LR_TIBIA 12
//
#define RM_COXA 13
#define RM_FEMUR 14
#define RM_TIBIA 15
//
#define LM_COXA 16
#define LM_FEMUR 17
#define LM_TIBIA 18


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


  static const transformations3D::Vectors::vector2d coxa_pos[NUM_MAX_LEGS];

}

#endif // __HEXA_PARAMS_H__
