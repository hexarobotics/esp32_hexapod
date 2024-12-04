#ifndef HEXA_CONTROL_H
    #define HEXA_CONTROL_H


namespace hexapod
{

    enum control_mode_e
    {
        C_MODE_GAITS = 0,
        C_MODE_BODY,
        MAX_C_MODES,
    };

    control_mode_e get_control_mode( void );

    void set_control_mode( control_mode_e new_mode );

}



#endif // HEXA_CONTROL_H