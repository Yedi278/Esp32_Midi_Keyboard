#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <stdint.h>
#include <stdio.h>
#include "esp_timer.h"

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

    // create timer
    esp_timer_handle_t timer = NULL;
    esp_timer_create_args_t timer_args = {
        .callback = &periodicKeyboardTask,
        .arg = &adc1_handle,
        .name = "readKeysLayout"
    };

    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer));

    esp_timer_start_periodic(timer, 1000000/10);

    custom_adc_init(&adc1_handle, &cali_handle);
    custom_gpio_init();
    
    while(1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);       
    }
}