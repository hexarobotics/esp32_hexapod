/**
 * @file servo_driver.cpp
 * @brief Controlador de servomotores PCA9685 para ESP32.
 * 
 * Este módulo permite inicializar y controlar varios PCA9685 en el mismo bus I2C.
 * Asegura que la función `i2cdev_init()` se llame **una sola vez** en el programa,
 * independientemente del número de instancias de `ServoDriver` creadas.
 * 
 * ### Ejemplo de uso:
 * ```cpp
 * Servo::ServoDriver pca1(0x40);
 * pca1.Init();
 * Servo::ServoDriver pca2(0x41);
 * pca2.Init();
 * ```
 * 
 * @note La inicialización del I2C se maneja de manera interna en el constructor y solo ocurre una vez.
 * No es necesario llamar manualmente a `i2cdev_init()`.
 * 
 * @date 2024
 * @version 1.0
 * @autor Miguel
 */

#include "servo_driver.h"
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
#define SERVO_MIN	        102 // 102 Default - set 0º
#define SERVO_MAX	        540 // 512 Default - set 180º
#define MIN_ANGLE	        0.0
#define MAX_ANGLE	        180.0
#define SERVO_PWM_FREQ      50
#define ROUND               0.5F
#define NUM_MAX_CHANNELS    16
#define UNKOWN              0

namespace Servo
{
    bool ServoDriver::i2c_initialized_ = false;
    uint8_t ServoDriver::dev_counter_ = 0;

    // Constructor por defecto
    ServoDriver::ServoDriver() :
        pwm_freq(SERVO_PWM_FREQ),
        i2c_address(PCA9685_I2C_ADDRESS),
        servo_cal_(nullptr),
        pose_size_(UNKOWN)
    {
        init_i2c_once();

        Init();

        dev_counter_++;
    }

    // Constructor con dirección I2C específica
    ServoDriver::ServoDriver(uint8_t i2c_addr) :
        pwm_freq(SERVO_PWM_FREQ),
        i2c_address(i2c_addr),
        servo_cal_(nullptr),
        pose_size_(UNKOWN)
    {
        init_i2c_once();

        Init();

        dev_counter_++;
    }

    // Constructor Addr i2c y calibracion servos
    ServoDriver::ServoDriver(uint8_t i2c_addr, const servo_cal_t* servo_calibration, uint16_t pose_size) :
        pwm_freq(SERVO_PWM_FREQ),
        i2c_address(i2c_addr),
        servo_cal_(servo_calibration),
        pose_size_(pose_size)
    {
        if (servo_calibration == nullptr || pose_size == 0)
        {
            ESP_LOGE(TAG, "Invalid servo calibration data or pose size");
            abort();
        }

        init_i2c_once();
        Init();

        dev_counter_++;
    }

    // Método estático para inicializar el I2C solo una vez
    void ServoDriver::init_i2c_once(void)
    {
        if (!i2c_initialized_)
        {
            ESP_ERROR_CHECK(i2cdev_init());
            i2c_initialized_ = true;
        }
    }

    esp_err_t ServoDriver::set_angle(uint8_t ch, float Angle)
    {
        float pwm_val = 0;

        if(Angle < MIN_ANGLE) Angle = MIN_ANGLE;
        if(Angle > MAX_ANGLE) Angle = MAX_ANGLE;

        if ( servo_cal_ == nullptr ) // default -> no calibration
        {
            pwm_val = (Angle - MIN_ANGLE) * ((float)SERVO_MAX - (float)SERVO_MIN) / (MAX_ANGLE - MIN_ANGLE) + (float)SERVO_MIN;
            ESP_LOGE(TAG, "NO ESTARAS ENTRANDO CABRON?");
        }
        else if ( ( ch < pose_size_ ) && ch < NUM_MAX_CHANNELS )
        {
            pwm_val = (Angle - MIN_ANGLE) * ((float)servo_cal_[ch].pwm_max - (float)servo_cal_[ch].pwm_min) / (MAX_ANGLE - MIN_ANGLE) + (float)SERVO_MIN;
        }
        else
        {
            ESP_LOGE(TAG, "Channel higher than pose size: check configuration");
        }


        return set_pwm_value(&pca9685_dev, ch, (uint16_t)(pwm_val + ROUND));
    }

    esp_err_t ServoDriver::set_pwm_value(i2c_dev_t *dev, uint8_t ch, uint16_t pwm)
    {
        if (pwm > 4095) pwm = 4095; 

        if (pwm == 4095)
        {
            return pca9685_set_pwm_value(dev, ch, 4095); // Full on.
        }
        else if (pwm == 0)
        {
            return pca9685_set_pwm_value(dev, ch, 0);    // Full off.
        }
        else
        {
            return pca9685_set_pwm_value(dev, ch, pwm);
        }

        return ESP_OK;
    }

    // PCA_9685 Driver Init
    ServoDriver::servo_driv_ret ServoDriver::Init(void)
    {
        memset(&pca9685_dev, 0, sizeof(i2c_dev_t));

        // Usa la dirección I2C especificada en el constructor
        ESP_ERROR_CHECK(pca9685_init_desc(&pca9685_dev, i2c_address, I2C_MASTER_NUM, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO));
        ESP_ERROR_CHECK(pca9685_init(&pca9685_dev));

        ESP_ERROR_CHECK(pca9685_restart(&pca9685_dev));

        uint16_t freq;

        ESP_ERROR_CHECK(pca9685_set_pwm_frequency(&pca9685_dev, pwm_freq));
        ESP_ERROR_CHECK(pca9685_get_pwm_frequency(&pca9685_dev, &freq));

        ESP_LOGI(TAG, "Driver: %d, Addr: 0x%X, Freq %d Hz, real %d", dev_counter_, i2c_address, 50, freq);

        return s_ret_ok;
    }

} // namespace Servo
