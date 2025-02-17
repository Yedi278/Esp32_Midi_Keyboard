#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <stdint.h>
#include <stdio.h>

#include "keyboard_layout.h"
#include "custom_gpio.h"
#include "custom_adc.h"

void displayKeysLayout(const unsigned char* key_layout){

    for(int i=0; i<BLOCK_N; i++){
        printf("Block %d: ", i);
        for(int j=0; j<KEYS_N; j++){
            printf("%d ", key_layout[i] & (1 << j) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n");
}

void resetKeysLayout(unsigned char* key_layout){
    for(int i=0; i<BLOCK_N; i++){
        key_layout[i] = 0;
    }
}

void readKeysLayout(adc_oneshot_unit_handle_t *adc1_handle, 
                    adc_cali_handle_t *cali_handle, 
                    unsigned char* key_layout){
    extern const adc_channel_t key_pins[KEYS_N];
    extern const gpio_num_t block_pins[BLOCK_N];

    int i=0;
    int j=0;
    int val = 0;

    for(i=0; i<BLOCK_N; i++){

        gpio_set_level(block_pins[i], 1);
        
        for(j=0; j<KEYS_N; j++){

            if(adc_oneshot_get_calibrated_result(*adc1_handle, *cali_handle, key_pins[j], &val) != ESP_OK){
                ESP_LOGE("ADC", "Error reading key %d", j);
                continue;
            }
            if(val > 2000){
                key_layout[i] |= (1 << j);
            }

        }

        gpio_set_level(block_pins[i], 0);
    }
}

void periodicKeyboardTask( void *args ){
    extern adc_oneshot_unit_handle_t adc1_handle;
    extern adc_cali_handle_t cali_handle;
    extern unsigned char key_layout[8];

    readKeysLayout(&adc1_handle,
                   &cali_handle,
                   &key_layout[0]);

    displayKeysLayout(&key_layout[0]);
    resetKeysLayout(&key_layout[0]);
}

#endif // KEYBOARD_DRIVER_H