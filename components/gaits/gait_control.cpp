

#include "gaits.h"


namespace hexapod
{



// yo envio desde la app Xspeed int16
//int16_t //-32,768 to 32,767
//INT16_MAX

// Implementación de la función map
    float Gaits::map(int16_t value, int16_t in_min, int16_t in_max, float out_min, float out_max)
    {
        return out_min + (static_cast<float>(value - in_min) * (out_max - out_min)) / (in_max - in_min);
    }

    void Gaits::set_xspeed( int16_t xspeed )
    {
        Xspeed = map(xspeed,INT16_MIN,INT16_MAX,-Xspeed_max,Xspeed_max);
    }

    void Gaits::set_yspeed( int16_t yspeed )
    {
        Yspeed = map(yspeed,INT16_MIN,INT16_MAX,-Yspeed_max,Yspeed_max);
    }

    void Gaits::set_rspeed( int16_t rspeed )
    {
        Rspeed = map(rspeed,INT16_MIN,INT16_MAX,-Rspeed_max,Rspeed_max);
    }

/*
    int Gaits::Ajuste_velocidad(uint8_t joystick)
    {
        if ( joystick == 7) //RIGHT
        {
            Xspeed=-map(ps2x.Read_Joystick(PSS_LX),-128,127,-Veloc_X_max,Veloc_X_max);
            return Xspeed;
        }
        else if (joystick==8) // LEFT
        {
            Yspeed=map(ps2x.Read_Joystick(PSS_LY),-128,127,-Veloc_Y_max,Veloc_Y_max);
            return Yspeed;
        }
        else if (joystick==5) // LEFT
        {
            Rspeed=-mapf(ps2x.Read_Joystick(PSS_RX),-128,127,-Veloc_Rot_max,Veloc_Rot_max);
            //Serial.print("Rspeed: ");
            //Serial.println(Rspeed,4);
            return Rspeed;
        }
        else
        {
            return 0;
        }
    }
*/
}