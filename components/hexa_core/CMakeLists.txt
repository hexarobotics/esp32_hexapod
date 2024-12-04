#Componente hex que depende de transformations
idf_component_register(SRCS  "hexa_control_mode.cpp" "hexa_gaits_mode.cpp" "hexa_task.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES gaits servo_controller hexaik)

