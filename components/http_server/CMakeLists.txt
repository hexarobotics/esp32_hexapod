idf_component_register(SRCS "http_server.c"
                    INCLUDE_DIRS "include"
                    REQUIRES  esp_http_server app_update esp_timer sntp_time_sync tasks_manager wifi_app )

