#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <stdint.h>
#include <stdio.h>

#include "custom_adc.h"
#include "custom_gpio.h"
#include "keyboard_driver.h"

void app_main() {

    custom_adc_init();
    custom_gpio_init();


    while(1) {
        readKeys();
        displayKeysLayout();

        vTaskDelay(1000 / portTICK_PERIOD_MS);       
    }
}