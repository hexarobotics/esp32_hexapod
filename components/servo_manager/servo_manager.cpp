#include "servo_manager.h"


#define I2C_MASTER_SDA_IO    (gpio_num_t)21     /*!< GPIO para SDA */
#define I2C_MASTER_SCL_IO    (gpio_num_t)22     /*!< GPIO para SCL */
#define I2C_MASTER_NUM       I2C_NUM_0          /*!< I2C port número */
#define I2C_MASTER_FREQ_HZ   1000000            /*!< Frecuencia I2C (1MHz) */
#define I2C_MASTER_TX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */

#define PCA9685_I2C_ADDRESS  0x40  /*!< Dirección I2C del PCA9685 por defecto*/
#define SERVO_MIN_PULSEWIDTH 1000  /*!< Ancho de pulso mínimo en microsegundos */
#define SERVO_MAX_PULSEWIDTH 2000  /*!< Ancho de pulso máximo en microsegundos */
#define SERVO_MAX_DEGREE     180   /*!< Máximo grado de rotación de servomotor */

namespace Servo
{
    // Class Servo_mng
    // Constructor Servo_mng
    Servo_mng::Servo_mng() :
    pwm_freq(50) 
    {
    }

    // PCA_9685 Driver Init
    Servo_mng::s_mng_ret Servo_mng::init( void )
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

        // Configurar frecuencia PWM (50Hz para servomotores)
        if ( ESP_OK != pca9685_set_pwm_frequency(&pca9685_dev, pwm_freq) ) // 50Hz
        {
            ESP_LOGE(TAG, "Error al configurar frecuencia PWM: %s", esp_err_to_name(ret));

            return s_ret_nok;
        }

        return s_ret_ok;
    }

    // Función para inicializar I2C
    esp_err_t Servo_mng::i2c_master_init()
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

        if ( ESP_OK != i2c_param_config(I2C_MASTER_NUM, &i2c_conf) )
        {
            ESP_LOGE(TAG, "Error al configurar el i2c: %s", esp_err_to_name(ret));

            return ret;
        }
        
        return i2c_driver_install(I2C_MASTER_NUM, i2c_conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    }


} // namespace servo