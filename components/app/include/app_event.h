/**
 ******************************************************************************
 * @file    app_event.h
 * @brief
 * @details
 ******************************************************************************
 */

#ifndef APP_EVENT_H
#define APP_EVENT_H

//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stddef.h>

//--------------------------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------------------------

/**
 * @brief Global event module identifier.
 */
typedef enum
{
    APP_EVENT_MODULE_LED = 0,
    APP_EVENT_MODULE_SYSTEM,
    APP_EVENT_MODULE_CONSOLE
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

#endif /* APP_EVENT_H */
