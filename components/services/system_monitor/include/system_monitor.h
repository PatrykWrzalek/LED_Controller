/**
 ******************************************************************************
 * @file    system_monitor.h
 * @brief   System monitoring utilities for FreeRTOS tasks and ESP heap.
 *
 * @details
 * Provides functions to:
 *  - initialize a system monitor task
 *  - report task heartbeats
 *  - dump runtime statistics, heap usage, and stack high-water marks
 *
 * Useful for debugging and monitoring embedded ESP32 applications.
 ******************************************************************************
 */

#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "sdkconfig.h"

#include "esp_err.h"

//--------------------------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------------------------

typedef enum
{
    SYSTEM_TASK_LED = 0,
    SYSTEM_TASK_MAX
} system_task_id_t;

/**
 * @brief Generic service event container.
 */
typedef enum
{
    SYSTEM_MONITOR_EVENT_HEARTBEAT,
    SYSTEM_MONITOR_EVENT_DUMP_STATS
} system_monitor_event_t;

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

/**
 * @brief   Initialize the system monitor task.
 * @details Creates a FreeRTOS task that periodically prints runtime stats, heap usage,
 *          and stack high-water marks. Must be called once during system startup.
 * @return
 *  - ESP_OK on success
 *  - ESP_ERR_INVALID_STATE if already initialized
 *  - ESP_FAIL if task creation failed
 */
esp_err_t system_monitor_init(void);

/**
 * @brief     Send heartbeat event from a task.
 * @param[in] task_name Null-terminated task name.
 * @return
 *  - true if event was queued successfully
 *  - false if queue is full or not initialized
 */
bool system_monitor_heartbeat(system_task_id_t task);   // TODO: zrobić działające system_monitor_heartbeat

/**
 * @brief Print FreeRTOS task runtime statistics.
 */
void system_monitor_print_runtime(void);

/**
 * @brief Print current and minimum heap usage.
 */
void system_monitor_print_heap(void);

/**
 * @brief Print stack high-water marks for all tasks.
 */
void system_monitor_print_stack(void);

#endif /* SYSTEM_MONITOR_H */
