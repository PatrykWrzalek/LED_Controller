idf_component_register(SRCS "app.c"
                    INCLUDE_DIRS "include"
                     REQUIRES spi_flash event_bus drv_led debug_console system_monitor
)
