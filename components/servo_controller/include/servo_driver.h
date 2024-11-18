#ifndef __ServoDriver__
#define __ServoDriver__

#include <stdint.h>
#include "pca9685.h"

/**
 * @brief Namespace para la gestión de servos
 */
namespace Servo
{
    /**
     * @brief Clase para gestionar servomotores a través del controlador PCA9685
     */
    class ServoDriver
    {
    public:
        enum servo_driv_ret
        {
            s_ret_ok,
            s_ret_nok
        };

        // Constructor por defecto, usa la dirección I2C por defecto
        ServoDriver();

        // Constructor que permite especificar una dirección I2C
        ServoDriver(uint8_t i2c_addr);

        esp_err_t       set_angle     (uint8_t Channel, float Angle);
        servo_driv_ret   Init          (void);

    private:
        i2c_dev_t pca9685_dev;
        const char *TAG = "PCA9685";
        static bool i2c_initialized;
        static uint8_t dev_counter;

        uint16_t pwm_freq;    // Freq pwm pca9685
        uint8_t  i2c_address; // Dirección I2C especificada

		static void init_i2c_once	(void);
        esp_err_t 	set_pwm_value   (i2c_dev_t *dev, uint8_t Channel, uint16_t pwm);
    };
};

#endif // __ServoDriver_H__
