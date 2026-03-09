#include "app.h"

void app_main(void)
{
    app_start();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}