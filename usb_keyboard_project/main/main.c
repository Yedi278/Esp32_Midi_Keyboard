/*
 * SPDX-FileCopyrightText: 2019 Ha Thach (tinyusb.org)
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2022-2024 Espressif Systems (Shanghai) CO LTD
 */

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#ifndef CONFIG_LOG_MAXIMUM_LEVEL
#define CONFIG_LOG_MAXIMUM_LEVEL 4
#endif

#include "custom_midi.h"
#include "custom_gpio.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    custom_midi_init();
    custom_gpio_init();

    // // Periodically send MIDI packets
    // int const tempo = 286;
    // const esp_timer_create_args_t periodic_midi_args = {
    //     .callback = &periodic_midi_write_example_cb,
    //     /* name is optional, but may help identify the timer when debugging */
    //     .name = "periodic_midi"
    // };

    // ESP_LOGI(TAG, "MIDI write task init");
    // esp_timer_handle_t periodic_midi_timer;
    // ESP_ERROR_CHECK(esp_timer_create(&periodic_midi_args, &periodic_midi_timer));
    // ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_midi_timer, tempo * 1000));

    // // Read received MIDI packets
    // ESP_LOGI(TAG, "MIDI read task init");
    // xTaskCreate(midi_task_read_example, "midi_task_read_example", 4 * 1024, NULL, 5, NULL);


    // gpio_set_level(key_pins[4], 1);
    while (1)
    {
        periodic_midi_func();
        // if(gpio_get_level(block_pins[3]) != 0){
        //     ESP_LOGI(TAG, "Key 0 pressed");
        // }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
