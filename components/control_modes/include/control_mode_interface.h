#ifndef HEXA_CONTROL_H
    #define HEXA_CONTROL_H

enum control_mode_e
{
    C_MODE_GAITS = 0,
    C_MODE_BODY,
    MAX_C_MODES,
};

    //control_mode_e get_control_mode( void );

#ifdef __cplusplus

void control_mode_interface_init(control_mode_e* mode);


extern "C" {
#endif

void set_control_mode( control_mode_e new_mode );


//void update_traslation_parameters( float x, float y, float z );

#ifdef __cplusplus
}
#endif





#endif // HEXA_CONTROL_H