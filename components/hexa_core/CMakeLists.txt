#Componente hex que depende de transformations
idf_component_register(SRCS "hexa_task.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES control_modes gaits_control_mode body_control_mode servo_controller hexaik)

