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

            // Steps in gait
            uint8_t stepsInCycle;
            uint8_t pushSteps;
            uint8_t desfase;

            float tranTime;     // Time between steps in ms
            float cycleTime;    // Total time. cycleTime =  (stepsInCycle * tranTime(ms)) / 1000.0 (to s) 

            uint8_t gaitleg_order[6];
            transformations3D::Tmatrix gaits[6];
            bool parado;
            uint8_t step;
            int8_t leg_step;
            bool modo_control;	// 0 movimiento cuerpo; 1 movimiento robot

            GaitType Current_Gait;

        public:
            // Constructor
            Gaits(GaitType gait);

            // Métodos públicos
            bool isMoving(void) const;
            void gait_select(GaitType type);
            void generateGait(uint8_t leg);
            void configureBody();
            void Gait_generator(uint8_t leg);

            void setLegOrderByGait(GaitType GaitType); // Nuevo método
            void update_gait_params(GaitType type);
            void update_velocities(void);

            // Getters y Setters si es necesario
            float getXspeed() const;
            void setXspeed(float speed);
    };
}

#endif