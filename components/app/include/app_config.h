/**
 ******************************************************************************
 * @file    app_config.h
 * @brief
 * @details
 ******************************************************************************
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

//--------------------------------------------------------------------------------------------
// Scheduler configuration
//--------------------------------------------------------------------------------------------

/**
 * @brief Task priorities used in the system.
 *
 * @note Higher number = higher priority.
 */
#define APP_TASK_PRIO_DRV_LED        4
#define APP_TASK_PRIO_DEBUG_CONSOLE  5

/**
 * @brief Task stack sizes (in words, not bytes if FreeRTOS config so defines).
 */
#define APP_TASK_STACK_DRV_LED        4096
#define APP_TASK_STACK_DEBUG_CONSOLE  4096

//--------------------------------------------------------------------------------------------
// Event bus configuration
//--------------------------------------------------------------------------------------------

#define APP_EVENT_BUS_QUEUE_LENGTH 10

#endif /* APP_CONFIG_H */