//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "debug_console.h"
#include "event_bus.h"
#include "drv_led_cli.h"
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

//--------------------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------------------

// static const char *TAG = "debug_console";

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
    drv_led_cli_register();
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
