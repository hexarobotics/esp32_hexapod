#include "servo_manager.h"


#define I2C_MASTER_SDA_IO    (gpio_num_t)21     /*!< GPIO para SDA */
#define I2C_MASTER_SCL_IO    (gpio_num_t)22     /*!< GPIO para SCL */
#define I2C_MASTER_NUM       I2C_NUM_0          /*!< I2C port número */
#define I2C_MASTER_FREQ_HZ   200000             /*!< Frecuencia I2C (1MHz) */
#define I2C_MASTER_TX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */
#define PCA9685_I2C_ADDRESS  0x40               /*!< Dirección I2C del PCA9685 por defecto*/

// VALORES LDX-218
//500 us (0 grados):
//PWM Value=(500 us / 20000 us)×4096≈102
//2500 us (180 grados):
//PWM Value=(2500 us20000 us)×4096≈512
#define SERVO_MIN	102
#define SERVO_MAX	512
#define MIN_ANGLE	0.0
#define MAX_ANGLE	180.0

namespace Servo
{
    // Class ServoManager
    // Constructor ServoManager
    ServoManager::ServoManager() : pwm_freq(50) 
    {
    }

    esp_err_t ServoManager::set_angle(uint8_t Channel, float Angle)
    {
        float pwm_val;

        if(Angle < MIN_ANGLE) Angle = MIN_ANGLE;
        if(Angle > MAX_ANGLE) Angle = MAX_ANGLE;

        pwm_val = (Angle - MIN_ANGLE) * ((float)SERVO_MAX - (float)SERVO_MIN) / (MAX_ANGLE - MIN_ANGLE) + (float)SERVO_MIN;

        return set_pwm_value(&pca9685_dev,Channel, (uint16_t)pwm_val);
    }

    esp_err_t ServoManager::set_pwm_value(i2c_dev_t *dev, uint8_t Channel, uint16_t pwm)
    {
        if (pwm > 4095) pwm = 4095; 

        if (pwm == 4095)
        {
            return pca9685_set_pwm_value(dev, Channel, 4096); // Especial para señal completamente encendida
        }
        else if (pwm == 0)
        {
            return pca9685_set_pwm_value(dev, Channel, 0);    // Especial para señal completamente apagada
        }
        else
        {
            return pca9685_set_pwm_value(dev, Channel, pwm);
        }

        return ESP_OK;
    }

    // ########## DRIVER CONFIG ##########

    // PCA_9685 Driver Init
    ServoManager::servo_mng_ret ServoManager::init_pca( void )
    {
        esp_err_t ret = ESP_FAIL;
        
        // Inicializar I2C
        if ( ESP_OK != i2c_master_init() )
        {
            ESP_LOGE(TAG, "Error al inicializar I2C: %s", esp_err_to_name(ret));

            return s_ret_nok;
        }

        // Inicializar descriptor PCA9685
        if ( ESP_OK != pca9685_init_desc(&pca9685_dev, PCA9685_I2C_ADDRESS, I2C_MASTER_NUM, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO))
        {
            ESP_LOGE(TAG, "Error al inicializar descriptor PCA9685: %s", esp_err_to_name(ret));

            return s_ret_nok;
        }

        // Inicializar PCA9685
        if ( ESP_OK != pca9685_init(&pca9685_dev) )
        {
            ESP_LOGE(TAG, "Error al inicializar PCA9685: %s", esp_err_to_name(ret));

            return s_ret_nok;
        }

        // Configurar frecuencia PWM (50Hz para servomotores)
        if ( ESP_OK != pca9685_set_pwm_frequency(&pca9685_dev, pwm_freq) ) // 50Hz
        {
            ESP_LOGE(TAG, "Error al configurar frecuencia PWM: %s", esp_err_to_name(ret));

            return s_ret_nok;
        }

        return s_ret_ok;
    }

    // Función para inicializar I2C
    esp_err_t ServoManager::i2c_master_init()
    {
        esp_err_t ret = ESP_FAIL;

        i2c_config_t i2c_conf;

        // Configurar I2C
        i2c_conf.mode = I2C_MODE_MASTER;
        i2c_conf.sda_io_num = I2C_MASTER_SDA_IO;
        i2c_conf.scl_io_num = I2C_MASTER_SCL_IO;
        i2c_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
        i2c_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
        i2c_conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
        i2c_conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

        ESP_LOGE(TAG, "SIGO VIVO 111111111111111: %s", esp_err_to_name(ret));


        if ( ESP_OK != i2c_param_config(I2C_MASTER_NUM, &i2c_conf) )
        {
            ESP_LOGE(TAG, "Error al configurar el i2c: %s", esp_err_to_name(ret));

            return ret;
        }

        ESP_LOGE(TAG, "SIGO VIVO 222222222222222: %s", esp_err_to_name(ret));

        
        return i2c_driver_install(I2C_MASTER_NUM, i2c_conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    }


} // namespace servo