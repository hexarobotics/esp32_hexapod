#ifndef GAIT_GENERATOR_H
#define GAIT_GENERATOR_H

#include <stdint.h>

enum GaitType 
{
    RIPPLE_6, RIPPLE_12, RIPPLE_24,
    TRIPOD_6, TRIPOD_12, TRIPOD_24,
    WAVE_12, WAVE_24,    
    NUM_MAX_GAITS
};

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

        float tranTime;     // Time between steps in ms
        float cycleTime;    // Total time. cycleTime =  (stepsInCycle * tranTime(ms)) / 1000.0 (to s) 

        uint8_t gaitleg_order[6];
        bool parado;
        uint8_t step;

        GaitType Current_Gait;

        void update_velocities();

    public:
        // Constructor
        Gaits();

        // Métodos públicos
        void gait_select(GaitType type);
        void generateGait(uint8_t leg);
        void configureBody();

        void setLegOrderByGait(GaitType GaitType); // Nuevo método
        void update_gait_params(GaitType type);
        void update_velocities();

        // Getters y Setters si es necesario
        float getXspeed() const;
        void setXspeed(float speed);
};

#endif