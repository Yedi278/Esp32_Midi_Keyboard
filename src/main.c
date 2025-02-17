#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <stdint.h>
#include <stdio.h>

#include "custom_adc.h"
#include "custom_gpio.h"
#include "keyboard_driver.h"

// log level
#define LOG_LEVEL 4
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#define SIZE 100

adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t cali_handle;

void app_main() {

    custom_adc_init(&adc1_handle, &cali_handle);
    custom_gpio_init();
    

    while(1) {

        vTaskDelay(1000 / portTICK_PERIOD_MS);       
    }
}