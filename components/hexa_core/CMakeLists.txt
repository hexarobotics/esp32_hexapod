#Componente hex que depende de transformations
idf_component_register(SRCS "hexa_control.cpp" "hexa_core.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES gaits servo_controller hexaik)

