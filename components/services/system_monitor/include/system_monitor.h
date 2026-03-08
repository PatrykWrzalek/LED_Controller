/**
 ******************************************************************************
 * @file    system_monitor.h
 * @brief   System monitoring utilities for FreeRTOS tasks and ESP heap.
 * @details Provides functions to initialize a system monitor task, report task
 *          heartbeats, and dump runtime statistics, heap usage, and stack
 *          high-water marks. Useful for debugging and monitoring embedded
 *          applications running on ESP32 with FreeRTOS.
 ******************************************************************************
 */

#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>

#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

//--------------------------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------------------------

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
 * @brief Initialize the system monitor task.
 */
void system_monitor_init(void);

/**
 * @brief Report a heartbeat from a task.
 * @param task_name Null-terminated name of the task sending the heartbeat.
 */
void system_monitor_heartbeat(const char *task_name);

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
