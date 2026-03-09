//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------
#include "drv_led.h"
#include "event_bus.h"
#include "system_monitor.h"

#include "esp_log.h"

#if CONFIG_DRV_LED_ENABLE

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
// Variables
//--------------------------------------------------------------------------------------------

static const char *TAG = "drv_led";
static bool _is_initialized = false;

//--------------------------------------------------------------------------------------------
// Static function prototypes
//--------------------------------------------------------------------------------------------

static void drv_led_configure_timer(void);
static void drv_led_configure_channel(ledc_channel_t channel, gpio_num_t gpio);
static void drv_led_set_status(uint32_t duty);
static uint32_t drv_led_max_duty(void);

static void drv_led_task(void *arg);

//--------------------------------------------------------------------------------------------
// API
//--------------------------------------------------------------------------------------------

esp_err_t drv_led_init(void)
{
    if (_is_initialized)
    {
        ESP_LOGW(TAG, "LED already initialized");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "Initializing LED PWM subsystem");

    drv_led_configure_timer();

    drv_led_configure_channel(LED_CHANNEL_STATUS, CONFIG_LED_GPIO_STATUS);

#if CONFIG_DRV_COLD_LED_ENABLE
    drv_led_configure_channel(LED_CHANNEL_COLD, CONFIG_LED_GPIO_COLD);
#endif

#if CONFIG_DRV_WARM_LED_ENABLE
    drv_led_configure_channel(LED_CHANNEL_WARM, CONFIG_LED_GPIO_WARM);
#endif

    _is_initialized = true;

    ESP_LOGI(TAG, "LED PWM initialized successfully");

    return ESP_OK;
}

esp_err_t drv_led_start_task(void)
{
    BaseType_t ret = xTaskCreate(drv_led_task, TAG, CONFIG_DRV_LED_TASK_STACK_SIZE, 
                                    NULL, CONFIG_DRV_LED_TASK_PRIORITY, NULL);
    
    return (ret == pdPASS) ? ESP_OK : ESP_FAIL;
}

bool drv_led_request(drv_led_event_id_t id)
{
    app_event_t event = {
        .module = APP_EVENT_MODULE_LED,
        .id     = id,
        .param  = 0
    };

    return event_bus_emit(&event);
}

//--------------------------------------------------------------------------------------------
// STATIC
//--------------------------------------------------------------------------------------------

static void drv_led_configure_timer(void)
{
    ledc_timer_config_t timer_conf = {
        .speed_mode      = LED_PWM_MODE,
        .duty_resolution = LED_PWM_RESOLUTION,
        .timer_num       = LED_PWM_TIMER,
        .freq_hz         = LED_PWM_FREQ_HZ,
        .clk_cfg         = LEDC_AUTO_CLK,
    };

    ESP_ERROR_CHECK(ledc_timer_config(&timer_conf));
}

static void drv_led_configure_channel(ledc_channel_t channel, gpio_num_t gpio)
{
    ledc_channel_config_t channel_conf = {
        .speed_mode = LED_PWM_MODE,
        .channel    = channel,
        .timer_sel  = LED_PWM_TIMER,
        .gpio_num   = gpio,
        .duty       = 0,
        .hpoint     = 0,
    };

    ESP_ERROR_CHECK(ledc_channel_config(&channel_conf));
}

static uint32_t drv_led_max_duty(void)
{
    return (1 << LED_PWM_RESOLUTION) - 1;
}

static void drv_led_set_status(uint32_t duty)
{
    if (!_is_initialized)
        return;

    if (duty > drv_led_max_duty())
        duty = drv_led_max_duty();

    ESP_ERROR_CHECK(ledc_set_duty(LED_PWM_MODE, LED_CHANNEL_STATUS, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LED_PWM_MODE, LED_CHANNEL_STATUS));
}

static void drv_led_task(void *arg)
{
    QueueHandle_t queue = event_bus_get_queue();
    configASSERT(queue != NULL);

    app_event_t event;

    ESP_LOGI(TAG, "LED task started");
    // system_monitor_heartbeat(SYSTEM_TASK_LED);
    drv_led_set_status(drv_led_max_duty()/2);

    while (1)
    {
        if (xQueueReceive(queue, &event, pdMS_TO_TICKS(100)) == pdTRUE)
        {
            if (event.module != APP_EVENT_MODULE_LED)
            {
                continue;
            }

            switch ((drv_led_event_id_t)event.id)
            {
                case DRV_LED_EVENT_ON:
                    ESP_LOGI(TAG, "LED ON");
                    drv_led_set_status(0);
                    break;

                case DRV_LED_EVENT_OFF:
                    ESP_LOGI(TAG, "LED OFF");
                    drv_led_set_status(drv_led_max_duty());
                    break;

                case DRV_LED_EVENT_TOGGLE:
                    ESP_LOGI(TAG, "LED TOGGLE");
                    drv_led_set_status(drv_led_max_duty()/2);
                    break;

                default:
                    break;
            }
        }

        // system_monitor_heartbeat(SYSTEM_TASK_LED);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    vTaskDelete(NULL);
}

#else

esp_err_t drv_led_init(void)
{
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t drv_led_start_task(void)
{
    return ESP_ERR_NOT_SUPPORTED;
}

bool drv_led_request(drv_led_event_id_t id)
{
    return false;
}

#endif
