idf_component_register(SRCS "debug_console.c"
                    INCLUDE_DIRS "include"
                    REQUIRES event_bus drv_led console nvs_flash app
)
