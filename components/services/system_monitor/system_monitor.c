//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "system_monitor.h"
#include "app_event.h"
#include "event_bus.h"

#include <stdlib.h>
#include <string.h>

#include "esp_log.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"

//--------------------------------------------------------------------------------------------
// Defines
//--------------------------------------------------------------------------------------------

#define SYSTEM_MONITOR_MAX_TASKS 16
#define SYSTEM_MONITOR_TLS_SLOT  0
#define SYSTEM_MONITOR_TIMEOUT_MS 2000

//--------------------------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------------------------

typedef struct
{
    TaskHandle_t handle;
    const char *name;
    int64_t last_heartbeat;
    bool alive;
} system_monitor_task_entry_t;

//--------------------------------------------------------------------------------------------
// Static function prototypes
//--------------------------------------------------------------------------------------------

static void system_monitor_task(void *arg);
static void system_monitor_check_tasks(void);

//--------------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------------

static const char *TAG = "system_monitor";

static TaskHandle_t s_task_handle = NULL;
static bool s_initialized = false;

static system_monitor_task_entry_t s_tasks[SYSTEM_MONITOR_MAX_TASKS];
static size_t s_task_count = 0;

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

    memset(s_tasks, 0, sizeof(s_tasks));

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

void system_monitor_register_current_task(void)
{
    if (s_task_count >= SYSTEM_MONITOR_MAX_TASKS)
    {
        ESP_LOGW(TAG, "Task registry full");
        return;
    }

    TaskHandle_t handle = xTaskGetCurrentTaskHandle();

    system_monitor_task_entry_t *entry = &s_tasks[s_task_count];

    entry->handle = handle;
    entry->name = pcTaskGetName(handle);
    entry->last_heartbeat = esp_timer_get_time();
    entry->alive = true;

    vTaskSetThreadLocalStoragePointer(handle, SYSTEM_MONITOR_TLS_SLOT, entry);

    s_task_count++;

    ESP_LOGI(TAG, "Registered task: %s", entry->name);
}

void system_monitor_heartbeat(void)
{
    system_monitor_task_entry_t *entry =
        pvTaskGetThreadLocalStoragePointer(NULL, SYSTEM_MONITOR_TLS_SLOT);

    if (!entry)
    {
        return;
    }

    entry->last_heartbeat = esp_timer_get_time();
}

void system_monitor_print_tasks(void)
{
    int64_t now = esp_timer_get_time();

    ESP_LOGI(TAG, "----------- Task health -----------");
    ESP_LOGI(TAG, "%-16s %-8s %-10s", "Task", "Alive", "Last(ms)");

    for (size_t i = 0; i < s_task_count; i++)
    {
        int64_t diff = now - s_tasks[i].last_heartbeat;

        ESP_LOGI(TAG, "%-16s %-8s %-10lld",
                 s_tasks[i].name, s_tasks[i].alive ? "YES" : "NO",
                 diff / 1000);
    }

    ESP_LOGI(TAG, "-----------------------------------");
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

//--------------------------------------------------------------------------------------------
// STATIC
//--------------------------------------------------------------------------------------------

static void system_monitor_task(void *arg)
{
    ESP_LOGI(TAG, "System monitor task started");

    system_monitor_register_current_task();

    while (true)
    {
        system_monitor_heartbeat();

        system_monitor_check_tasks();

        vTaskDelay(pdMS_TO_TICKS(CONFIG_SYSTEM_MONITOR_PERIOD_MS));
    }

    vTaskDelete(NULL);
}

static void system_monitor_check_tasks(void)
{
    int64_t now = esp_timer_get_time();

    for (size_t i = 0; i < s_task_count; i++)
    {
        int64_t diff = now - s_tasks[i].last_heartbeat;

        if (diff > (SYSTEM_MONITOR_TIMEOUT_MS * 1000))
        {
            if (s_tasks[i].alive)
            {
                ESP_LOGE(TAG, "Task %s stalled (%lld ms)",
                         s_tasks[i].name, diff / 1000);
            }

            s_tasks[i].alive = false;
        }
        else
        {
            s_tasks[i].alive = true;
        }
    }
}
