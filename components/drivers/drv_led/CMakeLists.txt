idf_component_register(SRCS "drv_led_cli.c" "drv_led.c"
                    INCLUDE_DIRS "include"
                    REQUIRES event_bus esp_driver_ledc esp_driver_gpio system_monitor
)

set(COMPONENT_KCONFIGS "Kconfig")
