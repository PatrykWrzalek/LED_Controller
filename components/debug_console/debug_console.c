//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "debug_console.h"
#include "event_bus.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"

#include "nvs.h"
#include "nvs_flash.h"

//--------------------------------------------------------------------------------------------
// Defines
//--------------------------------------------------------------------------------------------

#define PROMPT_STR "CMD>"

//--------------------------------------------------------------------------------------------
// Static function prototypes
//--------------------------------------------------------------------------------------------

static void debug_console_task(void *arg);
static void register_commands(void);

static int cmd_event1(int argc, char **argv);
static int cmd_event2(int argc, char **argv);
static int cmd_event3(int argc, char **argv);

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
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }

    esp_console_register_help_command();

    register_commands();

    xTaskCreate(debug_console_task, "debug_console", 4096, NULL, 5, NULL);
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

static void register_commands(void)
{
    const esp_console_cmd_t cmd1 = {
        .command = "event1",
        .help = "Wywołaj event1",
        .hint = NULL,
        .func = &cmd_event1,
        .argtable = NULL
    };

    const esp_console_cmd_t cmd2 = {
        .command = "event2",
        .help = "Wywołaj event2",
        .hint = NULL,
        .func = &cmd_event2,
        .argtable = NULL
    };

    const esp_console_cmd_t cmd3 = {
        .command = "event3",
        .help = "Wywołaj event3",
        .hint = NULL,
        .func = &cmd_event3,
        .argtable = NULL
    };

    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd1));
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd2));
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd3));
}

static int cmd_event1(int argc, char **argv)
{
    ESP_LOGI(TAG, "Wywołano event1");
    // event_bus_emit(EVENT1);
    return 0;
}

static int cmd_event2(int argc, char **argv)
{
    ESP_LOGI(TAG, "Wywołano event2");
    // event_bus_emit(EVENT2);
    return 0;
}

static int cmd_event3(int argc, char **argv)
{
    ESP_LOGI(TAG, "Wywołano event3");
    // event_bus_emit(EVENT3);
    return 0;
}