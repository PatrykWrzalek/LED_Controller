idf_component_register(SRCS "drv_led_cli.c" "drv_led.c"
                    INCLUDE_DIRS "include"
                    REQUIRES debug_console event_bus esp_driver_ledc esp_driver_gpio
)

set(COMPONENT_KCONFIGS "Kconfig")