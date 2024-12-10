idf_component_register(SRCS "http_server.c" "sntp_time_sync.c"
                    INCLUDE_DIRS "include"
                    REQUIRES  gaits_control_mode body_control_mode esp_http_server app_update esp_timer tasks_manager wifi_app )

