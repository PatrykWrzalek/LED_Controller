//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "debug_console.h"
#include "event_bus.h"
#include "drv_led.h"

#include "esp_log.h"
#include "esp_console.h"
#include "nvs_flash.h"

//--------------------------------------------------------------------------------------------
// Defines
//--------------------------------------------------------------------------------------------

#define PROMPT_STR "CMD>"

//--------------------------------------------------------------------------------------------
// Static function prototypes
//--------------------------------------------------------------------------------------------

static void debug_console_task(void *arg);

#if CONFIG_DRV_LED_ENABLE
static void debug_console_register_commands(void);

static int cmd_led_on(int argc, char **argv);
static int cmd_led_off(int argc, char **argv);
static int cmd_led_toggle(int argc, char **argv);
#endif

//--------------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------------

static const char *TAG = "debug_console";

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

void debug_console_init(void)
{
    // NVS init
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }

    esp_console_register_help_command();

#if CONFIG_DRV_LED_ENABLE
    debug_console_register_commands();
#endif

    BaseType_t ret = xTaskCreate(debug_console_task, "debug_console", CONFIG_DEBUG_CONSOLE_TASK_STACK_SIZE, 
                                    NULL, CONFIG_DEBUG_CONSOLE_TASK_PRIORITY, NULL);

    configASSERT(ret == pdPASS);
}
//--------------------------------------------------------------------------------------------
// STATIC
//--------------------------------------------------------------------------------------------

static void debug_console_task(void *arg)
{
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.prompt = PROMPT_STR;

#if CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
    esp_console_dev_usb_serial_jtag_config_t usb_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&usb_config, &repl_config, &repl));
#else
    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl));
#endif

    ESP_ERROR_CHECK(esp_console_start_repl(repl));
    vTaskDelete(NULL);
}

#if CONFIG_DRV_LED_ENABLE
static void debug_console_register_commands(void)
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

static int cmd_led_on(int argc, char **argv)
{
    drv_led_request(DRV_LED_EVENT_ON);

    ESP_LOGI(TAG, "EVENT_LED_ON emitted");
    return 0;
}

static int cmd_led_off(int argc, char **argv)
{
    drv_led_request(DRV_LED_EVENT_OFF);

    ESP_LOGI(TAG, "EVENT_LED_OFF emitted");
    return 0;
}

static int cmd_led_toggle(int argc, char **argv)
{
    drv_led_request(DRV_LED_EVENT_TOGGLE);

    ESP_LOGI(TAG, "EVENT_LED_TOGGLE emitted");
    return 0;
}
#endif
