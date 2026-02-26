/**
 ******************************************************************************
 * @file    event_bus.h
 * @brief   Central event dispatching mechanism for inter-module communication.
 *
 * @details
 * This module implements a lightweight event bus based on a FreeRTOS queue.
 * It allows decoupled communication between system components.
 *
 * Design principles:
 *  - Modules do NOT communicate directly.
 *  - Modules emit events.
 *  - Modules consume events.
 *  - event_bus is only responsible for transport (no logic).
 *
 * This improves scalability, modularity and testability.
 ******************************************************************************
 */

#ifndef EVENT_BUS_H
#define EVENT_BUS_H

//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

//--------------------------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------------------------

/**
 * @brief Global event module identifier.
 */
typedef enum
{
    APP_EVENT_MODULE_LED = 0,
} app_event_module_t;

/**
 * @brief Generic application event container.
 */
typedef struct
{
    app_event_module_t module;   /* Module identifier */
    uint32_t           id;       /* Event ID (module specific) */
    uint32_t           param;    /* Optional parameter (brightness etc.) */
} app_event_t;

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

/**
 * @brief       Initializes the event bus.
 * @details     Creates a FreeRTOS queue used for system-wide event exchange.
 */
void event_bus_init(void);

/**
 * @brief       Sends an event to the system.
 * @param[in]   event Pointer to event structure.
 * @return      true if event was queued successfully
 * @return      false if queue is full or not initialized
 */
bool event_bus_emit(const app_event_t *event);

/**
 * @brief       Emit event (blocking version).
 * @param event Pointer to event structure.
 * @param ticks Maximum time to wait for free space.
 * @return      true if event was queued.
 * @return      false on timeout or error.
 */
bool event_bus_emit_blocking(const app_event_t *event, TickType_t ticks);

/**
 * @brief  Returns internal queue handle.
 * @return QueueHandle_t
 */
QueueHandle_t event_bus_get_queue(void);

#endif /* EVENT_BUS_H */
