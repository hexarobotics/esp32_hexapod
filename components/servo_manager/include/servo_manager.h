#ifndef __SERVO_MNG__
  #define __SERVO_MNG__

#include <stdint.h>

#include "driver/i2c.h"
#include "esp_log.h"
#include "pca9685.h"

/**
 * @brief Servo library
 * 
 */
namespace Servo
{
    class Servo_mng
    {
        public:
            enum s_mng_ret
            {
                s_ret_ok,
                s_ret_nok
            };

            uint16_t pwm_freq;

            Servo_mng();

            s_mng_ret init (void);

        private:
            i2c_dev_t pca9685_dev;
            const char *TAG = "PCA9685_SERVO";

            esp_err_t i2c_master_init (void);
    };
};

#endif // __SERVO_MNG_H__

