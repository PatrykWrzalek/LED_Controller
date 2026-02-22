idf_component_register(SRCS "debug_console.c"
                    INCLUDE_DIRS "include"
                    REQUIRES event_bus console nvs_flash
)
