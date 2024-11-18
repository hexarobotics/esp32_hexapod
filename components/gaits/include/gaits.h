#ifndef GAIT_GENERATOR_H
#define GAIT_GENERATOR_H

#include <stdint.h>

#include "gait_types.h"
#include "tmatrix.h"

namespace hexapod
{
    class Gaits
    {
        private:
            // Velocities and space parameters
            float Xspeed;
            float Yspeed;
            float Rspeed; // radians
            float liftHeight;

            float Xspeed_max;
            float Yspeed_max;
            float Rspeed_max; // radians

            // Steps in gait
            uint8_t stepsInCycle;
            uint8_t pushSteps;
            uint8_t desfase;

            uint16_t tranTime;     // Time between steps in ms
            uint32_t cycleTime;    // Total time. cycleTime =  (stepsInCycle * tranTime(ms)) / 1000.0 (to s) 

            uint8_t gaitleg_order[6];
            transformations3D::Tmatrix gaits[6];
            bool parado;
            uint8_t gait_step;
            int8_t leg_step; //int8_t ok, puede salir negativo -> refactorizar
            bool modo_control;	// 0 movimiento cuerpo; 1 movimiento robot

            GaitType current_gait;

            enum Leg_order
            {
                LEG_FIRST = 1,
                LEG_SECOND,
                LEG_THIRD,
                LEG_FOURTH,
                LEG_FIFTH,
                LEG_SIXTH,
                LEG_MAX_ORD    // Total de patas (6)
            };

        public:
            // Constructor
            Gaits(GaitType gait);

            // Métodos públicos
            bool isMoving(void) const;
            void gait_select(GaitType type);
            void generateGait(uint8_t leg);
            void configureBody();
            uint16_t get_gait_transition_time(void);
            transformations3D::Tmatrix step(uint8_t leg);
            void next_step( void );

            void setLegOrderByGait(GaitType GaitType); // Nuevo método
            void update_gait_params(GaitType type);
            void update_velocities(void);

            // Getters y Setters si es necesario
            //float getXspeed() const;
            float map(int16_t value, int16_t in_min, int16_t in_max, float out_min, float out_max);
            void  set_xspeed( int16_t xspeed );
            void  set_yspeed( int16_t yspeed );
            void  set_rspeed( int16_t rspeed );
    };
}

#endif