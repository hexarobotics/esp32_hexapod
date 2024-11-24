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
            void    setFrameTimeMs              (uint16_t newFrameTimeMs);
            void    setup_servo_init_pose       (const uint16_t* initialPose);
            void    writePositions              (void);
            void    interpolate_setup           (uint16_t time);
            void    Interpolate_step            (void);
            bool    isInterpolating             (void) const;
            void    save_nextpose               (uint8_t id, uint8_t pos);
            void    back_to_init_position       (void);
            void    initializeServos            (const uint16_t* servosStart, const uint16_t* group1Up, const uint16_t* group2Up);
            // Getter para frame_length_us_
            uint16_t get_frame_length_ms        (void) const { return frame_length_ms_; }
        private:
            // Variables de estado
            int32_t frame_length_ms_;
            int32_t frame_length_us_;
            bool     interpolating_;
            int64_t  lastFrame_;
            uint16_t initial_pose_[POSE_SIZE] = {
                95, 50, 15, 95, 50, 15, 95, 50, 15, 
                95, 50, 15, 95, 50, 15, 95, 50, 15}; // pose inicial
            uint16_t pose_[POSE_SIZE];         // Posiciones actuales
            uint16_t nextPose_[POSE_SIZE];     // Posiciones objetivo
            int16_t  speed_[POSE_SIZE];        // Velocidades de interpolación
            uint8_t  id_[POSE_SIZE];           // Identificadores de los servos  ****** BORRAR ******
            uint8_t  poseSize_;                // Número de servos a controlar
            servo_cal_t pca1_servo_cal_[POSE_SIZE/2] = {
                {100,540},{102,510},{100,540}, // LF: coxa, femur, tibia
                {98,540},{102,540},{102,540}, // LM: coxa, femur, tibia
                {98,540},{90,520},{102,540}, // LR: coxa, femur, tibia
            };
            servo_cal_t pca2_servo_cal_[POSE_SIZE/2] = {
                {102,545},{90,540},{102,540}, // RF: coxa, femur, tibia
                {102,540},{90,540},{102,540}, // RM: coxa, femur, tibia
                {102,545},{90,540},{102,540}  // RR: coxa, femur, tibia
            };
            // Controladores de servos (estáticos, 2 en este caso)
            Servo::ServoDriver pca1;      // Primer controlador de servo (pca)
            Servo::ServoDriver pca2;      // Segundo controlador de servo (pca)
    };
}

#endif // __SERVO_CONTROLLER_H__