idf_component_register(SRCS "http_server.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_http_server tasks_manager wifi_app)

