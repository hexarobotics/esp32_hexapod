# Componente Gaits
idf_component_register( SRCS "gaits.cpp" "gait_control.cpp"
                        INCLUDE_DIRS "include"
                        REQUIRES transformations hexa_core )

