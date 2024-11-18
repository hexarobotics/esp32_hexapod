#include "servo_controller.h"
#include "esp_timer.h"
#include <freertos/task.h>

namespace Servo
{
    ServoController::ServoController() :
        frameTimeMs_(DEFAULT_FRAME_TIME_MS),
        frameTimeUs_(DEFAULT_FRAME_TIME_MS * 1000),
        interpolating_(0),
        lastFrame_(0),
        poseSize_(POSE_SIZE),
        pca1(0x40),     // Dirección I2C del primer controlador
        pca2(0x41)      // Dirección I2C del segundo controlador
    {
        // Inicialización de las poses
        for (uint8_t i = 0; i < POSE_SIZE; i++)
        {
            id_[i] = i + 1;
            pose_[i] = 0;      // Inicializa a 0
            nextPose_[i] = 0;  // Inicializa a 0
        }
    }

    void ServoController::setFrameTimeMs(uint16_t newFrameTimeMs)
    {
        frameTimeMs_ = newFrameTimeMs;
        frameTimeUs_ = newFrameTimeMs * 1000;
    }

    void ServoController::writePosition()
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
        uint8_t frames = (time / frameTimeMs_) + 1;
        lastFrame_ = esp_timer_get_time();

        for (uint8_t i = 0; i < poseSize_; i++)
        {
            if (nextPose_[i] > pose_[i])
            {
                speed_[i] = (nextPose_[i] - pose_[i]) / frames + 1;
            }
            else
            {
                speed_[i] = (pose_[i] - nextPose_[i]) / frames + 1;
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

        if (frameTimeUs_ > esp_timer_get_time() - lastFrame_)
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

        writePosition();
    }

    void ServoController::save_nextpose(int id, int pos)
    {
        if (id >= 1 && id <= poseSize_)
        {
            nextPose_[id - 1] = pos;  // Accede al índice correspondiente (id - 1)
        }
    }

        void ServoController::setup_servo_init_pose(const uint16_t* initialPose)
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

    void ServoController::initializeServos(const uint16_t* servosStart, const uint16_t* group1Up, const uint16_t* group2Up)
    {
        writePosition();
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