#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "custom_midi.h"
#include "esp_timer.h"

#include "custom_adc.h"
#include "custom_gpio.h"
#include "keyboard_driver.h"

#define CONFIG_LOG_MAXIMUM_LEVEL 4

adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t cali_handle;

void app_main(void)
{
    custom_midi_init();
    custom_adc_init(&adc1_handle, &cali_handle);
    custom_gpio_init();

    // Periodically send MIDI packets
    int const tempo = 10000; // 10 milliseconds
    const esp_timer_create_args_t periodic_midi_args = {
        .callback = &periodic_midi_write_example_cb,
        /* name is optional, but may help identify the timer when debugging */
        .name = "periodic_midi"
    };

    ESP_LOGI(TAG, "MIDI write task init");
    esp_timer_handle_t periodic_midi_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_midi_args, &periodic_midi_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_midi_timer, tempo));
    
    while(1){

    }
    // // Read received MIDI packets
    // ESP_LOGI(TAG, "MIDI read task init");
    // xTaskCreate(midi_task_read_example, "midi_task_read_example", 4 * 1024, NULL, 5, NULL);
}