/**
 ******************************************************************************
 * @file    drv_led.h
 * @brief   LED driver abstraction layer (PWM-based).
 *
 * @details
 * This module is responsible ONLY for:
 *  - Configuring LEDC peripheral
 *  - Controlling PWM duty
 *  - Reacting to LED-related events
 *
 * It does NOT:
 *  - Contain business logic
 *  - Know who generated events
 *  - Store application state
 ******************************************************************************
 */

#ifndef DRV_LED_H
#define DRV_LED_H

//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include <stdio.h>

#include "sdkconfig.h"

#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

//--------------------------------------------------------------------------------------------
// Defines
//--------------------------------------------------------------------------------------------

#define LED_PWM_FREQ_HZ     2
#define LED_PWM_RESOLUTION  LEDC_TIMER_13_BIT
#define LED_PWM_MODE        LEDC_LOW_SPEED_MODE
#define LED_PWM_TIMER       LEDC_TIMER_0

#define LED_CHANNEL_STATUS  LEDC_CHANNEL_0
#define LED_CHANNEL_COLD    LEDC_CHANNEL_1
#define LED_CHANNEL_WARM    LEDC_CHANNEL_2

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
 * @brief Initializes LED driver hardware.
 */
void drv_led_init(void);

/**
 * @brief Starts LED event handling task.
 */
void drv_led_start_task(void);

/**
 * @brief         Sends an LED event through the event bus.
 * @param[in] id  LED event identifier (drv_led_event_id_t)
 * @return        true if the event was successfully sent,
 *                false otherwise
 */
bool drv_led_request(drv_led_event_id_t id);

#endif /* DRV_LED_H */
