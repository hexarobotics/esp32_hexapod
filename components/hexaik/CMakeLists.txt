# Componente hex que depende de transformations
idf_component_register(SRCS "HexaIK.cpp" "Hexaik.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES transformations hexa_core)

