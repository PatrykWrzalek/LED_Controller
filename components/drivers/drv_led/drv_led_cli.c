//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "drv_led_cli.h"
#include "esp_console.h"

#if CONFIG_DRV_LED_ENABLE

//--------------------------------------------------------------------------------------------
// Static function prototypes
//--------------------------------------------------------------------------------------------

static int cmd_led_on(int argc, char **argv);
static int cmd_led_off(int argc, char **argv);
static int cmd_led_toggle(int argc, char **argv);

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

void drv_led_cli_register(void)
{
    const esp_console_cmd_t led_on_cmd = {
        .command = "led_on",
        .help = "Turn LED ON",
        .hint = NULL,
        .func = cmd_led_on,
        .argtable = NULL
    };

    const esp_console_cmd_t led_off_cmd = {
        .command = "led_off",
        .help = "Turn LED OFF",
        .hint = NULL,
        .func = cmd_led_off,
        .argtable = NULL
    };

    const esp_console_cmd_t led_toggle_cmd = {
        .command = "led_toggle",
        .help = "Toggle LED state",
        .hint = NULL,
        .func = cmd_led_toggle,
        .argtable = NULL
    };

    ESP_ERROR_CHECK(esp_console_cmd_register(&led_on_cmd));
    ESP_ERROR_CHECK(esp_console_cmd_register(&led_off_cmd));
    ESP_ERROR_CHECK(esp_console_cmd_register(&led_toggle_cmd));
}

//--------------------------------------------------------------------------------------------
// STATIC
//--------------------------------------------------------------------------------------------

static int cmd_led_on(int argc, char **argv)
{
    drv_led_request(DRV_LED_EVENT_ON);
    return 0;
}

static int cmd_led_off(int argc, char **argv)
{
    drv_led_request(DRV_LED_EVENT_OFF);
    return 0;
}

static int cmd_led_toggle(int argc, char **argv)
{
    drv_led_request(DRV_LED_EVENT_TOGGLE);
    return 0;
}

#endif
