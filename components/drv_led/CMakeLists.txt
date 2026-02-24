idf_component_register(SRCS "drv_led.c"
                    INCLUDE_DIRS "include"
                    REQUIRES event_bus esp_driver_ledc esp_driver_gpio app
)

set(COMPONENT_KCONFIGS "Kconfig")