//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "event_bus.h"

#include "esp_log.h"
#include "esp_err.h"

//--------------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------------

static const char *TAG = "event_bus";

static QueueHandle_t s_event_queue = NULL;
static bool s_initialized = false;

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

esp_err_t event_bus_init(void)
{
    if (s_initialized)
    {
        ESP_LOGW(TAG, "Event bus already initialized");
        return ESP_ERR_INVALID_STATE;
    }

    s_event_queue = xQueueCreate(CONFIG_EVENT_BUS_QUEUE_LENGTH, sizeof(app_event_t));

    if (s_event_queue == NULL)
    {
        ESP_LOGE(TAG, "Failed to create event queue");
        return ESP_FAIL;
    }

    s_initialized = true;

    ESP_LOGI(TAG, "Event bus initialized (queue length=%d)", CONFIG_EVENT_BUS_QUEUE_LENGTH);

    return ESP_OK;
}

bool event_bus_emit(const app_event_t *event)
{
    if (!s_initialized || event == NULL)
    {
        return false;
    }

    BaseType_t ret = xQueueSend(s_event_queue, event, 0);

    if (ret != pdPASS)
    {
        ESP_LOGW(TAG, "Event queue full: module=%u id=%u", (unsigned)event->module, (unsigned)event->id);
        return false;
    }

    return true;
}

bool event_bus_emit_blocking(const app_event_t *event, TickType_t ticks)
{
    if (!s_initialized || event == NULL)
    {
        return false;
    }

    BaseType_t ret = xQueueSend(s_event_queue, event, ticks);

    if (ret != pdPASS)
    {
        ESP_LOGW(TAG, "Event queue full: module=%u id=%u", (unsigned)event->module, (unsigned)event->id);
        return false;
    }

    return true;
}

QueueHandle_t event_bus_get_queue(void)
{
    return s_event_queue;
}
