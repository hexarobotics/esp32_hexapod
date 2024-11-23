#include "servo_controller.h"
#include "esp_timer.h"
#include <freertos/task.h>

namespace Servo
{
    


    ServoController::ServoController() :
        frame_length_ms_(DEFAULT_FRAME_TIME_MS),
        frame_length_us_(DEFAULT_FRAME_TIME_MS * 1000),
        interpolating_(0),
        lastFrame_(esp_timer_get_time()),
        poseSize_(POSE_SIZE),
        pca1(0x40, pca1_servo_cal_,poseSize_ / 2 ),     // Dirección I2C del primer controlador
        pca2(0x41, pca2_servo_cal_,poseSize_ / 2 )      // Dirección I2C del segundo controlador
    {
        // Inicialización de las poses
        for (uint8_t i = 0; i < POSE_SIZE; i++)
        {
            //id_[i] = i + 1;
            pose_[i] = initial_pose_[i];      // Inicializa a 90
            nextPose_[i] = initial_pose_[i];  // Inicializa a 90
        }
    }

    void ServoController::setFrameTimeMs(uint16_t newFrameTimeMs)
    {
        frame_length_ms_ = newFrameTimeMs;
        frame_length_us_ = newFrameTimeMs * 1000;
    }

    void ServoController::writePositions()
    {
        for (uint8_t i = 0; i < poseSize_; i++)
        {
            if (i < 9)
            {
                pca1.set_angle( i, pose_[i] );
            }
            else
            {
                pca2.set_angle( i-9, pose_[i] );
            }
        }
    }

    void ServoController::interpolate_setup(uint16_t time)
    {
        uint16_t frames = (time / frame_length_ms_) + 1;
        lastFrame_ = esp_timer_get_time();

        for (uint8_t i = 0; i < poseSize_; i++)
        {
            if (nextPose_[i] > pose_[i])
            {
                int32_t delta = static_cast<int32_t>(nextPose_[i]) - static_cast<int32_t>(pose_[i]);
                speed_[i] = (delta / frames) + 1;
            }
            else
            {
                int32_t delta = static_cast<int32_t>(pose_[i]) - static_cast<int32_t>(nextPose_[i]);
                speed_[i] = (delta / frames) + 1;
            }
        }

        interpolating_ = 1;
    }

    void ServoController::Interpolate_step()
    {
        if (interpolating_ == 0)
        {
            return;
        }

        if (frame_length_us_ > esp_timer_get_time() - lastFrame_)
        {
            return;
        }

        lastFrame_ = esp_timer_get_time();
        int complete = poseSize_;

        for (uint8_t i = 0; i < poseSize_; i++)
        {
            int diff = nextPose_[i] - pose_[i];
            if (diff == 0)
            {
                complete--;
            }
            else
            {
                if (diff > 0)
                {
                    if (diff < speed_[i])
                    {
                        pose_[i] = nextPose_[i];
                        complete--;
                    }
                    else
                    {
                        pose_[i] += speed_[i];
                    }
                }
                else
                {
                    if ((-diff) < speed_[i])
                    {
                        pose_[i] = nextPose_[i];
                        complete--;
                    }
                    else
                    {
                        pose_[i] -= speed_[i];
                    }
                }
            }
        }

        if (complete <= 0)
        {
            interpolating_ = 0;
        }

        writePositions();
    }

    void ServoController::setup_servo_init_pose( const uint16_t* initialPose ) // BORRAR, se hace en el constructor
    {
        for (uint8_t i = 0; i < poseSize_; i++)
        {
            id_[i] = i + 1;
            pose_[i] = initialPose[i];
            nextPose_[i] = initialPose[i];
        }

        interpolating_ = 0;
        lastFrame_ = esp_timer_get_time();
    }

    void ServoController::save_nextpose( uint8_t index, uint8_t pos )
    {
        if ( index < poseSize_ )
        {
            nextPose_[index] = pos;  // Accede al índice correspondiente (id - 1)
        }
    }

    void ServoController::initializeServos(const uint16_t* servosStart, const uint16_t* group1Up, const uint16_t* group2Up)
    {
        writePositions();
        vTaskDelay(pdMS_TO_TICKS(1000));

        for (int i = 1; i < poseSize_ + 1; i++) save_nextpose(i, servosStart[i - 1]);
        interpolate_setup(1000);

        while (interpolating_ > 0)
        {
            Interpolate_step();
        }

        for (int i = 1; i < poseSize_ + 1; i++) save_nextpose(i, group1Up[i - 1]);
        interpolate_setup(500);

        while (interpolating_ > 0)
        {
            Interpolate_step();
        }

        for (int i = 1; i < poseSize_ + 1; i++) save_nextpose(i, group2Up[i - 1]);
        interpolate_setup(500);

        while (interpolating_ > 0)
        {
            Interpolate_step();
        }

        for (int i = 1; i < poseSize_ + 1; i++) save_nextpose(i, servosStart[i - 1]);
        interpolate_setup(500);

        while (interpolating_ > 0)
        {
            Interpolate_step();
        }
    }

    bool ServoController::isInterpolating() const
    {
        return interpolating_;
    }

}