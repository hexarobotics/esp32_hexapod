#ifndef _BODY_CONTROL_INTERFACE_H_
#define _BODY_CONTROL_INTERFACE_H_


#ifdef __cplusplus
#include "tmatrix.h"

void body_control_interface_init(transformations3D::Tmatrix* tbody_obj);

extern "C" {
#endif

void update_traslation_parameters( float x, float y, float z );

#ifdef __cplusplus
}
#endif

#endif // _BODY_CONTROL_INTERFACE_H_
