idf_component_register(SRCS "debug_console.c"
                    INCLUDE_DIRS "include"
                    REQUIRES drv_led console nvs_flash system_monitor
)
