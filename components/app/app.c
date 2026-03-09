//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "app.h"

#include "drv_led.h"
#include "event_bus.h"
#include "debug_console.h"

#include "esp_log.h"

//--------------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------------

static const char *TAG = "app";

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

void app_start(void)
{
    ESP_LOGI(TAG, "Application start");

    //---------------------------------------------------------------------------
    // Event bus
    //---------------------------------------------------------------------------

    ESP_LOGI(TAG, "Initializing event bus");

    if (event_bus_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "Event bus initialization failed");
        return;
    }

    //---------------------------------------------------------------------------
    // Debug console
    //---------------------------------------------------------------------------

    ESP_LOGI(TAG, "Initializing debug console");

    if (debug_console_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "Debug console initialization failed");
    }

    //---------------------------------------------------------------------------
    // LED driver
    //---------------------------------------------------------------------------

    ESP_LOGI(TAG, "Initializing LED driver");

    if (drv_led_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "LED driver initialization failed");
        return;
    }

    //---------------------------------------------------------------------------
    // LED task
    //---------------------------------------------------------------------------

    ESP_LOGI(TAG, "Starting LED task");

    if (drv_led_start_task() == ESP_OK)
    {
        ESP_LOGI(TAG, "LED task started successfully");
    }
    else
    {
        ESP_LOGE(TAG, "LED task start failed");
    }
}
