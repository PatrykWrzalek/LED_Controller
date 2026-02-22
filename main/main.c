#include <stdio.h>
#include "drv_led.h"
#include "event_bus.h"

#include "esp_log.h"

void app_main(void)
{
    ESP_LOGI("MAIN", "Init event bus");
    event_bus_init();

    ESP_LOGI("MAIN", "Event bus get queue");
    QueueHandle_t queue = event_bus_get_queue();

    ESP_LOGI("MAIN", "Init LED");
    drv_led_init();

    ESP_LOGI("MAIN", "Start LED task");
    drv_led_start_task(queue);
    
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}