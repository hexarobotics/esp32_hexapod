idf_component_register(SRCS "servo_controller.cpp" "servo_driver.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES pca9685 esp_timer)

