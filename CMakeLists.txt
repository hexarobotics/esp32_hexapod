cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 11)

set(EXTRA_COMPONENT_DIRS
    "esp-idf-lib/components"          # componentes externos
    "components/app_nvs"
    "components/control_modes/body_control_mode"
    "components/control_modes/gaits_control_mode"
    "components/hexa_core"
    "components/hexaik"
    "components/http_server"
    "components/servo_controller"
    "components/tasks_manager"
    "components/transformations"
    "components/wifi_app"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(vscode_hexa_main_project_wifi)

# Incluir archivos binarios
target_add_binary_data(${PROJECT_NAME}.elf "webpage/favicon.ico" BINARY)
target_add_binary_data(${PROJECT_NAME}.elf "webpage/app.js" BINARY)
target_add_binary_data(${PROJECT_NAME}.elf "webpage/app.css" BINARY)
target_add_binary_data(${PROJECT_NAME}.elf "webpage/index.html" BINARY)
target_add_binary_data(${PROJECT_NAME}.elf "webpage/jquery-3.3.1.min.js" BINARY)
#target_add_binary_data(${PROJECT_NAME}.elf "webpage/interact.min.js" BINARY)
target_add_binary_data(${PROJECT_NAME}.elf "webpage/control.html" BINARY)
target_add_binary_data(${PROJECT_NAME}.elf "webpage/control.css" BINARY)
target_add_binary_data(${PROJECT_NAME}.elf "webpage/control.js" BINARY)
