//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "system_monitor.h"
#include "esp_log.h"

//--------------------------------------------------------------------------------------------
// Static function prototypes
//--------------------------------------------------------------------------------------------

static void system_monitor_task(void *arg);

//--------------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------------

static const char *TAG = "system_monitor";
QueueHandle_t monitor_queue;
TaskHandle_t monitor_task;

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

void system_monitor_init(void)
{
    xTaskCreate(system_monitor_task, "system_monitor", 4096, NULL, 3, NULL);
}

void system_monitor_heartbeat(const char *task_name)
{
    ESP_LOGD(TAG, "Heartbeat from %s", task_name);
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
    UBaseType_t tasks = uxTaskGetNumberOfTasks();

    TaskStatus_t *status = malloc(tasks * sizeof(TaskStatus_t));

    tasks = uxTaskGetSystemState(status, tasks, NULL);

    for (int i = 0; i < tasks; i++)
    {
        ESP_LOGI(TAG, "Task: %s stack watermark: %u", status[i].pcTaskName, status[i].usStackHighWaterMark);
    }

    free(status);
}

//--------------------------------------------------------------------------------------------
// STATIC
//--------------------------------------------------------------------------------------------

static void system_monitor_task(void *arg)
{
    while (true)
    {
        system_monitor_print_runtime();
        system_monitor_print_heap();
        system_monitor_print_stack();

        vTaskDelay(pdMS_TO_TICKS(CONFIG_SYSTEM_MONITOR_PERIOD_MS));
    }
}
