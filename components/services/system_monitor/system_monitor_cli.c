//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "system_monitor_cli.h"

#include "esp_console.h"

//--------------------------------------------------------------------------------------------
// Static function prototypes
//--------------------------------------------------------------------------------------------

static int cmd_monitor_runtime(int argc, char **argv);
static int cmd_monitor_heap(int argc, char **argv);
static int cmd_monitor_stack(int argc, char **argv);

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

void system_monitor_cli_register(void)
{
    esp_console_cmd_t runtime_cmd = {
        .command = "monitor_runtime",
        .help    = "Print FreeRTOS task runtime statistics",
        .hint    = NULL,
        .func    = &cmd_monitor_runtime,
        .argtable = NULL
    };

    esp_console_cmd_t heap_cmd = {
        .command = "monitor_heap",
        .help    = "Print current and minimum heap usage",
        .hint    = NULL,
        .func    = &cmd_monitor_heap,
        .argtable = NULL
    };

    esp_console_cmd_t stack_cmd = {
        .command = "monitor_stack",
        .help    = "Print stack high-water marks for all tasks",
        .hint    = NULL,
        .func    = &cmd_monitor_stack,
        .argtable = NULL
    };

    ESP_ERROR_CHECK(esp_console_cmd_register(&runtime_cmd));
    ESP_ERROR_CHECK(esp_console_cmd_register(&heap_cmd));
    ESP_ERROR_CHECK(esp_console_cmd_register(&stack_cmd));
}

//--------------------------------------------------------------------------------------------
// STATIC
//--------------------------------------------------------------------------------------------

static int cmd_monitor_runtime(int argc, char **argv)
{
    system_monitor_print_runtime();
    return 0;
}

static int cmd_monitor_heap(int argc, char **argv)
{
    system_monitor_print_heap();
    return 0;
}

static int cmd_monitor_stack(int argc, char **argv)
{
    system_monitor_print_stack();
    return 0;
}
