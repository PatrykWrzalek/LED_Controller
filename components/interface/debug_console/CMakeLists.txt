idf_component_register(SRCS "debug_console.c"
                    INCLUDE_DIRS "include"
                    REQUIRES console nvs_flash drv_led system_monitor
)
