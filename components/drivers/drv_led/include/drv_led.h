/**
 ******************************************************************************
 * @file    drv_led.h
 * @brief   LED driver module using ESP32 LEDC (PWM) peripheral.
 *
 * @details
 * This module is responsible for:
 *  - Initializing LEDC timer and channels
 *  - Running a dedicated FreeRTOS task
 *  - Handling LED-related events received from event bus
 *
 * The module can be enabled or disabled via CONFIG_DRV_LED_ENABLE.
 * When disabled, API functions return ESP_ERR_NOT_SUPPORTED
 * or false (depending on return type).
 *
 *******************************************************************************
 */

#ifndef DRV_LED_H
#define DRV_LED_H

//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include <stdio.h>

#include "sdkconfig.h"

#include "esp_err.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

//--------------------------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------------------------

/**
 * @brief LED driver event identifiers.
 */
typedef enum
{
    DRV_LED_EVENT_ON = 0,
    DRV_LED_EVENT_OFF,
    DRV_LED_EVENT_TOGGLE,
} drv_led_event_id_t;

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

/**
 * @brief   Initialize LED driver.
 * @details Configures LEDC timer and channels.
 *          Must be called before starting the driver task.
 * @return  - ESP_OK on success
 *          - ESP_ERR_INVALID_STATE if already initialized
 *          - ESP_ERR_NOT_SUPPORTED if module disabled
 */
esp_err_t drv_led_init(void);


/**
 * @brief   Start LED driver FreeRTOS task.
 * @details Creates internal LED task with parameters defined in Kconfig.
 * @return  - ESP_OK on success
 *          - ESP_FAIL if task creation failed
 *          - ESP_ERR_NOT_SUPPORTED if module disabled
 */
esp_err_t drv_led_start_task(void);


/**
 * @brief   Send LED request to event bus.
 * @param   id LED event identifier.
 * @return  - true if event was successfully queued
 *          - false otherwise or if module disabled
 */
bool drv_led_request(drv_led_event_id_t id);

#endif /* DRV_LED_H */
