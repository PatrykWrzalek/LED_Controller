/**
 ******************************************************************************
 * @file    system_monitor_cli.h
 * @brief   CLI commands for system monitor module.
 *
 * @details
 * This module registers console commands that allow runtime inspection
 * of system diagnostics provided by the system monitor.
 *
 * The commands are available through the ESP-IDF console component and
 * expose runtime information such as:
 *
 *  - CPU usage per FreeRTOS task
 *  - heap usage statistics
 *  - stack high-water marks
 ******************************************************************************
 */

#ifndef SYSTEM_MONITOR_CLI_H
#define SYSTEM_MONITOR_CLI_H

//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "system_monitor.h"

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

/**
 * @brief   Register CLI commands for the system monitor module.
 * @details Registers console commands that expose system diagnostic information.
 *          This function should be called during application initialization after
 *          the ESP console subsystem has been initialized.
 */
void system_monitor_cli_register(void);

#endif /* SYSTEM_MONITOR_CLI_H */