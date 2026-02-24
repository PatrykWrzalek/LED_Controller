//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "event_bus.h"
#include "esp_log.h"

//--------------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------------

static const char *TAG = "event_bus";
static QueueHandle_t s_event_queue = NULL;

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

void event_bus_init(void)
{
    configASSERT(s_event_queue == NULL);

    s_event_queue = xQueueCreate(10, sizeof(app_event_t));
    configASSERT(s_event_queue != NULL);
}

bool event_bus_emit(const app_event_t *event)
{
    if (s_event_queue == NULL || event == NULL)
        return false;

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
    if (s_event_queue == NULL || event == NULL)
        return false;

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
