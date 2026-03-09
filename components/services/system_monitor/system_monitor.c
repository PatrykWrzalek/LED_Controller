//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "system_monitor.h"
#include "app_event.h"
#include "event_bus.h"

#include <stdlib.h>

#include "esp_log.h"
#include "esp_heap_caps.h"

//--------------------------------------------------------------------------------------------
// Static function prototypes
//--------------------------------------------------------------------------------------------

static void system_monitor_task(void *arg);

//--------------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------------

static const char *TAG = "system_monitor";

static TaskHandle_t s_task_handle = NULL;
static bool s_initialized = false;

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

esp_err_t system_monitor_init(void)
{
    if (s_initialized)
    {
        ESP_LOGW(TAG, "System monitor already initialized");
        return ESP_ERR_INVALID_STATE;
    }

    BaseType_t ret = xTaskCreate(system_monitor_task, "system_monitor", CONFIG_SYSTEM_MONITOR_TASK_STACK_SIZE,
                                    NULL, CONFIG_SYSTEM_MONITOR_TASK_PRIORITY, &s_task_handle);

    if (ret != pdPASS || s_task_handle == NULL)
    {
        ESP_LOGE(TAG, "Failed to create system monitor task");
        return ESP_FAIL;
    }

    s_initialized = true;

    ESP_LOGI(TAG, "System monitor initialized");

    return ESP_OK;
}

bool system_monitor_heartbeat(system_task_id_t task)
{
    if (!s_initialized || task < SYSTEM_TASK_MAX)
    {
        return false;
    }

    app_event_t event = {
        .module = APP_EVENT_MODULE_SYSTEM_MONITOR,
        .id     = SYSTEM_MONITOR_EVENT_HEARTBEAT,
        .param  = 0
    };

    ESP_LOGD(TAG, "Heartbeat from %d", task);
    return event_bus_emit(&event);
}

void system_monitor_print_runtime(void)
{
    char buffer[512];

    vTaskGetRunTimeStats(buffer);

    ESP_LOGI(TAG, "\nTask Runtime Stats:\n%s", buffer);
}

void system_monitor_print_heap(void)
{
    size_t free = esp_get_free_heap_size();
    size_t min  = esp_get_minimum_free_heap_size();

    ESP_LOGI(TAG, "Heap free: %u", free);
    ESP_LOGI(TAG, "Heap minimum: %u", min);
}

void system_monitor_print_stack(void)
{
    UBaseType_t task_count = uxTaskGetNumberOfTasks();

    TaskStatus_t *status = malloc(task_count * sizeof(TaskStatus_t));

    if (!status)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for task status");
        return;
    }

    task_count = uxTaskGetSystemState(status, task_count, NULL);

    for (int i = 0; i < task_count; i++)
    {
        ESP_LOGI(TAG, "Task: %s stack watermark: %u", status[i].pcTaskName, (unsigned)status[i].usStackHighWaterMark);
    }

    free(status);
}

//--------------------------------------------------------------------------------------------
// STATIC
//--------------------------------------------------------------------------------------------

static void system_monitor_task(void *arg)
{
    QueueHandle_t queue = event_bus_get_queue();
    configASSERT(queue != NULL);

    app_event_t event;

    ESP_LOGI(TAG, "System monitor task started");

    while (true)
    {
        if (xQueueReceive(queue, &event, pdMS_TO_TICKS(CONFIG_SYSTEM_MONITOR_PERIOD_MS)) == pdTRUE)
        {
            if (event.module != APP_EVENT_MODULE_SYSTEM_MONITOR)
                continue;

            switch (event.id)
            {
                case SYSTEM_MONITOR_EVENT_DUMP_STATS:
                    system_monitor_print_runtime();
                    system_monitor_print_heap();
                    system_monitor_print_stack();
                    break;

                default:
                    break;
            }
        }
        else
        {
            system_monitor_print_heap();
            vTaskDelay(pdMS_TO_TICKS(CONFIG_SYSTEM_MONITOR_PERIOD_MS));
        }
    }

    vTaskDelete(NULL);
}
