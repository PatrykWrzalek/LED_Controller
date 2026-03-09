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
 * Advantages:
 *  - loose coupling
 *  - improved scalability
 *  - easier testing
 ******************************************************************************
 */

#ifndef EVENT_BUS_H
#define EVENT_BUS_H

//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "app_event.h"

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

/**
 * @brief   Initialize event bus subsystem.
 * @details Creates internal FreeRTOS queue used for system-wide event exchange.
 *          Must be called once during system startup before any module emits events.
 * @return
 *  - ESP_OK on success
 *  - ESP_ERR_INVALID_STATE if already initialized
 *  - ESP_FAIL if queue allocation failed
 */
esp_err_t event_bus_init(void);

/**
 * @brief       Emit event (non-blocking).
 * @param[in]   event Pointer to event structure.
 * @return
 *  - true if event was queued successfully
 *  - false if queue is full or not initialized
 */
bool event_bus_emit(const app_event_t *event);

/**
 * @brief       Emit event (blocking version).
 * @details     Waits for free space in queue up to specified timeout.
 * @param[in]   event Pointer to event structure.
 * @param[in]   ticks Maximum time to wait for free queue space.
 * @return
 *  - true if event was queued successfully
 *  - false on timeout or if bus not initialized
 */
bool event_bus_emit_blocking(const app_event_t *event, TickType_t ticks);

/**
 * @brief   Get internal queue handle.
 * @details Used by modules that consume events.
 * @return  Queue handle or NULL if bus not initialized.
 */
QueueHandle_t event_bus_get_queue(void);

#endif /* EVENT_BUS_H */
