#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <stdint.h>
#include <stdio.h>

#include "keyboard_layout.h"
#include "custom_gpio.h"
#include "custom_adc.h"

void displayKeysLayout(){
    extern unsigned char key_layout[8];

    for(int i=0; i<BLOCK_N; i++){
        for(int j=0; j<KEYS_N; j++){
            printf("%d ", key_layout[i] & (1 << j) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n");
}

void resetKeysLayout(){
    extern unsigned char key_layout[8];

    for(int i=0; i<BLOCK_N; i++){
        key_layout[i] = 0;
    }
}

void readKeys(){

    resetKeysLayout();

    for(int i=0; i<BLOCK_N; i++){
        gpio_set_level(block_pins[i], 1);
        // vTaskDelay(1 / portTICK_PERIOD_MS);

        for(int j=0; j<KEYS_N; j++){
            int raw = adc1_get_raw(key_pins[j]);
            if(esp_adc_cal_raw_to_voltage(raw, &adc1) > 3000){
                key_layout[i] |= (1 << j);
            }
        }

        gpio_set_level(block_pins[i], 0);
    }
}



#endif // KEYBOARD_DRIVER_H