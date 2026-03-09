/**
 ******************************************************************************
 * @file    debug_console.h
 * @brief   Debug CLI interface using ESP-IDF console subsystem.
 *
 * @details
 * This module initializes interactive console (UART or USB JTAG)
 * and registers development/debug commands.
 *
 * Responsibilities:
 *  - Initialize NVS (required by console)
 *  - Register CLI commands
 *  - Emit system events via event_bus
 *
 * This module contains NO business logic.
 ******************************************************************************
 */

#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "esp_err.h"

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

/**
 * @brief Initialize debug console subsystem.
 */
esp_err_t debug_console_init(void);

#endif /* DEBUG_CONSOLE_H */
