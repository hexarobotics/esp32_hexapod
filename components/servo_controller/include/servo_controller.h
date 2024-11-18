#ifndef __SERVO_CONTROLLER_H__
#define __SERVO_CONTROLLER_H__

#include "servo_driver.h"
#include "stdbool.h"

#define POSE_SIZE 18          // Tamaño del arreglo de posiciones
#define DEFAULT_FRAME_TIME_MS 20  // Tiempo por defecto para la interpolación (en milisegundos)

namespace Servo
{
    class ServoController
    {
        public:
            // Constructor
            ServoController();

            // Métodos públicos
            void    setFrameTimeMs     (uint16_t newFrameTimeMs);
            void    setup_servo_init_pose         (const uint16_t* initialPose);
            void    writePosition      ();
            void    interpolate_setup   (uint16_t time);
            void    Interpolate_step    ();
            bool    isInterpolating() const;
            void    save_nextpose       (int id, int pos);
            void    initializeServos   (const uint16_t* servosStart, const uint16_t* group1Up, const uint16_t* group2Up);

        private:
            // Variables de estado
            uint16_t frameTimeMs_;
            uint32_t frameTimeUs_;
            bool     interpolating_;
            uint64_t lastFrame_;
            uint16_t pose_[POSE_SIZE];         // Posiciones actuales
            uint16_t nextPose_[POSE_SIZE];     // Posiciones objetivo
            uint16_t speed_[POSE_SIZE];        // Velocidades de interpolación
            uint8_t  id_[POSE_SIZE];           // Identificadores de los servos
            uint8_t  poseSize_;                // Número de servos a controlar

            // Controladores de servos (estáticos, 2 en este caso)
            Servo::ServoDriver pca1;      // Primer controlador de servo (pca)
            Servo::ServoDriver pca2;      // Segundo controlador de servo (pca)
    };
}

#endif // __SERVO_CONTROLLER_H__