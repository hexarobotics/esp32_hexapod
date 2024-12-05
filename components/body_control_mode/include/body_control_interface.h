#ifndef _BODY_CONTROL_INTERFACE_H_
#define _BODY_CONTROL_INTERFACE_H_

#include "tmatrix.h"


void body_control_interface_init(transformations3D::Tmatrix* tbody_obj);


void update_traslation_parameters( float x, float y, float z );

#endif // _BODY_CONTROL_INTERFACE_H_
