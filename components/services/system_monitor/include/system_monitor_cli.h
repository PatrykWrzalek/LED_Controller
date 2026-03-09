/**
 ******************************************************************************
 * @file    system_monitor_cli.h
 * @brief   CLI commands for system monitor module.
 *
 * @details
 * Registers debug console commands to interact with the system monitor module.
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
 * @brief Register CLI commands for system monitoring.
 */
void system_monitor_cli_register(void);

#endif /* SYSTEM_MONITOR_CLI_H */