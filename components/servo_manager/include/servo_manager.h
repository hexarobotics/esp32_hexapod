#ifndef __ServoManager__
  #define __ServoManager__

#include <stdint.h>

//#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "pca9685.h"

/**
 * @brief Namespace para la gestión de servos
 */
namespace Servo
{
    /**
     * @brief Clase para gestionar servomotores a través del controlador PCA9685
     */
    class ServoManager
    {
        public:
            enum servo_mng_ret
            {
                s_ret_ok,
                s_ret_nok
            };

            uint16_t pwm_freq;

            ServoManager();

            esp_err_t       set_angle       (uint8_t Channel, float Angle);
            servo_mng_ret   init_pca        (void);

        private:
            i2c_dev_t pca9685_dev;
            const char *TAG = "PCA9685_SERVO";

            esp_err_t set_pwm_value   (i2c_dev_t *dev, uint8_t Channel, uint16_t pwm);
            esp_err_t i2c_master_init (void);
    };
};

#endif // __ServoManager_H__

