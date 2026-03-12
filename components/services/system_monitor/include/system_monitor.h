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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "sdkconfig.h"

#include "esp_err.h"

//--------------------------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------------------------

/**
 * @brief Generic service event container.
 */
typedef enum
{
    SYSTEM_MONITOR_EVENT_HEARTBEAT,     /*!< Task heartbeat received */
    SYSTEM_MONITOR_EVENT_DUMP_STATS     /*!< Request to print system statistics */
} system_monitor_event_t;

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

/**
 * @brief   Initialize the system monitor task.
 * @details Creates a dedicated FreeRTOS task responsible for system diagnostics.
 *          The monitor listens for events on the event bus and periodically reports
 *          memory statistics.
 * @return
 *  - ESP_OK on success
 *  - ESP_ERR_INVALID_STATE if already initialized
 *  - ESP_FAIL if task creation failed
 */
esp_err_t system_monitor_init(void);

/**
 * @brief Register current FreeRTOS task in system monitor.
 */
void system_monitor_register_current_task(void);

/**
 * @brief Send heartbeat from current task.
 */
void system_monitor_heartbeat(void);

/**
 * @brief Print task health table.
 */
void system_monitor_print_tasks(void);

/**
 * @brief   Print FreeRTOS runtime statistics.
 * @details Displays CPU usage statistics for each FreeRTOS task based on runtime
 *          counters collected by the scheduler.
 */
void system_monitor_print_runtime(void);

/**
 * @brief   Print heap usage statistics.
 * @details Displays the current free heap size and the minimum free heap size
 *          recorded since system boot.
 */
void system_monitor_print_heap(void);

/**
 * @brief   Print stack high-water marks for all tasks.
 * @details Shows the minimum remaining stack for each task since it started
 *          executing. Useful for stack size tuning and overflow diagnostics.
 */
void system_monitor_print_stack(void);

#endif /* SYSTEM_MONITOR_H */
