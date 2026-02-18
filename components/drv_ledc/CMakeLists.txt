idf_component_register(
    SRCS "drv_ledc.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_driver_ledc esp_driver_gpio esp_pm
)
