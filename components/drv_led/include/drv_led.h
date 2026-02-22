/**
 ******************************************************************************
 * @file    drv_led.h
 * @brief
 * @details
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
// API
//--------------------------------------------------------------------------------------------
void drv_led_init(void);
void drv_led_start_task(void);

#endif /* DRV_LED_H */
