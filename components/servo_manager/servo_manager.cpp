#include "servo_manager.h"

#include <string.h>
#include "esp_log.h"

#define I2C_MASTER_SDA_IO           gpio_num_t(21)          /*!< GPIO para SDA */
#define I2C_MASTER_NUM              I2C_NUM_0               /*!< I2C port número */
#define I2C_MASTER_SCL_IO           gpio_num_t(22)          /*!< GPIO para SCL */
#define I2C_MASTER_FREQ_HZ          200000                  /*!< Frecuencia I2C (1MHz) */
#define I2C_MASTER_TX_BUF_DISABLE   0                       /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                       /*!< I2C master doesn't need buffer */
#define PCA9685_I2C_ADDRESS         PCA9685_ADDR_BASE       /*!< Dirección I2C del PCA9685 por defecto*/

// VALORES LDX-218
// us20000 -> 50 hz
//500 us (0 grados):
//PWM Value=(500 us / 20000 us)×4096≈102
//2500 us (180 grados):
//PWM Value=(2500 us20000 us)×4096≈512
#define SERVO_MIN	        102 // 102 Default
#define SERVO_MAX	        512 // 512 Default
#define MIN_ANGLE	        0.0
#define MAX_ANGLE	        180.0
#define SERVO_PWM_FREQ      50
#define ROUND               0.5F

namespace Servo
{
    // Class ServoManager
    // Constructor ServoManager
    ServoManager::ServoManager() : pwm_freq(SERVO_PWM_FREQ)
    {
    }

    esp_err_t ServoManager::set_angle(uint8_t Channel, float Angle)
    {
        float pwm_val;

        if(Angle < MIN_ANGLE) Angle = MIN_ANGLE;
        if(Angle > MAX_ANGLE) Angle = MAX_ANGLE;

        pwm_val = (Angle - MIN_ANGLE) * ((float)SERVO_MAX - (float)SERVO_MIN) / (MAX_ANGLE - MIN_ANGLE) + (float)SERVO_MIN;

        return set_pwm_value(&pca9685_dev,Channel, (uint16_t)(pwm_val+ROUND));
    }

    esp_err_t ServoManager::set_pwm_value(i2c_dev_t *dev, uint8_t Channel, uint16_t pwm)
    {
        if (pwm > 4095) pwm = 4095; 

        if (pwm == 4095)
        {
            return pca9685_set_pwm_value(dev, Channel, 4095); // Full on.
        }
        else if (pwm == 0)
        {
            return pca9685_set_pwm_value(dev, Channel, 0);    // Full off.
        }
        else
        {
            return pca9685_set_pwm_value(dev, Channel, pwm);
        }

        return ESP_OK;
    }

    // ########## DRIVER CONFIG ##########

    // PCA_9685 Driver Init
    ServoManager::servo_mng_ret ServoManager::Init( void )
    {
        //pca9685_dev

       // i2c_dev_t dev;
        memset(&pca9685_dev, 0, sizeof(i2c_dev_t));

        // Init i2cdev library
        ESP_ERROR_CHECK(i2cdev_init());

        vTaskDelay(pdMS_TO_TICKS(20));

        ESP_ERROR_CHECK(pca9685_init_desc(&pca9685_dev, PCA9685_I2C_ADDRESS, I2C_MASTER_NUM, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO));
        ESP_ERROR_CHECK(pca9685_init(&pca9685_dev));

        ESP_ERROR_CHECK(pca9685_restart(&pca9685_dev));

        uint16_t freq;

        ESP_ERROR_CHECK(pca9685_set_pwm_frequency(&pca9685_dev, pwm_freq));
        ESP_ERROR_CHECK(pca9685_get_pwm_frequency(&pca9685_dev, &freq));

        ESP_LOGI(TAG, "Freq %dHz, real %d", 50, freq);


        return s_ret_ok;
    }

} // namespace servo