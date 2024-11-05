idf_component_register(SRCS "wifi_app.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi tasks_manager http_server)

