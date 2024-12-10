# Componente Gaits
idf_component_register( SRCS "gaits_control_interface.cpp" "gaits_control_mode.cpp" "gaits.cpp"
                        INCLUDE_DIRS "include"
                        REQUIRES transformations hexa_core )

