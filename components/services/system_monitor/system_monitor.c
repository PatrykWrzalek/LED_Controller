//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "system_monitor.h"
#include "app_event.h"
#include "event_bus.h"

#include <stdlib.h>
#include <string.h>

#include "esp_console.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"

//--------------------------------------------------------------------------------------------
// Static function prototypes
//--------------------------------------------------------------------------------------------

static void system_monitor_task(void *arg);

static int cmd_monitor_runtime(int argc, char **argv);
static int cmd_monitor_heap(int argc, char **argv);
static int cmd_monitor_stack(int argc, char **argv);

//--------------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------------

static const char *TAG = "system_monitor";

static TaskHandle_t s_task_handle = NULL;
static bool s_initialized = false;

static int64_t s_last_heartbeat[SYSTEM_TASK_MAX];

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

    memset(s_last_heartbeat, 0, sizeof(s_last_heartbeat));

    BaseType_t ret = xTaskCreate(system_monitor_task, TAG, CONFIG_SYSTEM_MONITOR_TASK_STACK_SIZE,
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
    if (!s_initialized || task >= SYSTEM_TASK_MAX)
    {
        return false;
    }

    s_last_heartbeat[task] = esp_timer_get_time();

    app_event_t event = {
        .module = APP_EVENT_MODULE_SYSTEM_MONITOR,
        .id     = SYSTEM_MONITOR_EVENT_HEARTBEAT,
        .param  = task
    };

    ESP_LOGD(TAG, "Heartbeat from %d", task);
    return event_bus_emit(&event);
}

void system_monitor_print_runtime(void)
{
    UBaseType_t task_count = uxTaskGetNumberOfTasks();

    TaskStatus_t *task_status = malloc(task_count * sizeof(TaskStatus_t));

    if (!task_status)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for task status");
        return;
    }
    uint32_t total_runtime = 0;

    task_count = uxTaskGetSystemState(task_status, task_count, &total_runtime);

    if (total_runtime == 0)
    {
        ESP_LOGW(TAG, "Total runtime is zero");
        free(task_status);
        return;
    }

    ESP_LOGI(TAG, "---------------- Runtime stats ----------------");
    ESP_LOGI(TAG, "%-16s %10s %8s", "Task", "Runtime", "CPU %");

    for (UBaseType_t i = 0; i < task_count; i++)
    {
        uint32_t runtime = task_status[i].ulRunTimeCounter;
        uint32_t percent = (runtime * 100UL) / total_runtime;

        ESP_LOGI(TAG, "%-16s %10lu %7lu%%",
                 task_status[i].pcTaskName, runtime, percent);
    }

    ESP_LOGI(TAG, "----------------------------------------------");

    free(task_status);
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

void system_monitor_cli_register(void)
{
    esp_console_cmd_t runtime_cmd = {
        .command = "monitor_runtime",
        .help    = "Print FreeRTOS task runtime statistics",
        .hint    = NULL,
        .func    = &cmd_monitor_runtime,
        .argtable = NULL
    };

    esp_console_cmd_t heap_cmd = {
        .command = "monitor_heap",
        .help    = "Print current and minimum heap usage",
        .hint    = NULL,
        .func    = &cmd_monitor_heap,
        .argtable = NULL
    };

    esp_console_cmd_t stack_cmd = {
        .command = "monitor_stack",
        .help    = "Print stack high-water marks for all tasks",
        .hint    = NULL,
        .func    = &cmd_monitor_stack,
        .argtable = NULL
    };

    ESP_ERROR_CHECK(esp_console_cmd_register(&runtime_cmd));
    ESP_ERROR_CHECK(esp_console_cmd_register(&heap_cmd));
    ESP_ERROR_CHECK(esp_console_cmd_register(&stack_cmd));
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
                case SYSTEM_MONITOR_EVENT_HEARTBEAT:
                    system_task_id_t task = event.param;

                    if (task < SYSTEM_TASK_MAX)
                    {
                        int64_t now = esp_timer_get_time();
                        int64_t diff = now - s_last_heartbeat[task];

                        ESP_LOGD(TAG, "Heartbeat task %d (%lld ms ago)",
                                 task, diff / 1000);
                    }
                    break;

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
            // system_monitor_print_heap();
        }
    }

    vTaskDelete(NULL);
}

static int cmd_monitor_runtime(int argc, char **argv)
{
    system_monitor_print_runtime();
    return 0;
}

static int cmd_monitor_heap(int argc, char **argv)
{
    system_monitor_print_heap();
    return 0;
}

static int cmd_monitor_stack(int argc, char **argv)
{
    system_monitor_print_stack();
    return 0;
}
