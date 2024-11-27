# Componente Gaits
idf_component_register( SRCS "gaits_control_interface.cpp" "gaits.cpp" "gait_control.cpp"
                        INCLUDE_DIRS "include"
                        REQUIRES transformations hexa_core )

