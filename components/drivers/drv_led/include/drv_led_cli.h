/**
 * @file    drv_led_cli.c
 * @brief   LED control CLI commands registration module
 * 
 * Module providing CLI command registration for LED control
 * in ESP-IDF console environment.
 */

#ifndef DRV_LED_CLI_H
#define DRV_LED_CLI_H

//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "drv_led.h"

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

/**
 * @brief Registers LED control CLI commands
 */
void drv_led_cli_register(void);

#endif /* DRV_LED_CLI_H */
