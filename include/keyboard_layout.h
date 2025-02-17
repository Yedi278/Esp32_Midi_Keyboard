#ifndef KEYBOARD_LAYOUT_H
#define KEYBOARD_LAYOUT_H

#include "driver/gpio.h"
#include "driver/adc.h"
#include "stdint.h"

#define BLOCK_N 8
#define KEYS_N  8

const gpio_num_t block_pins[BLOCK_N] = {
    GPIO_NUM_35,
    GPIO_NUM_36,
    GPIO_NUM_37,
    GPIO_NUM_38,
    GPIO_NUM_39,
    GPIO_NUM_40,
    GPIO_NUM_41,
    GPIO_NUM_42
};

const adc1_channel_t key_pins[KEYS_N] = {
    ADC1_CHANNEL_0,
    ADC1_CHANNEL_1,
    ADC1_CHANNEL_2,
    ADC1_CHANNEL_3,
    ADC1_CHANNEL_4,
    ADC1_CHANNEL_5,
    ADC1_CHANNEL_6,
    ADC1_CHANNEL_7
};

unsigned char key_layout[8];

#endif // KEYBOARD_LAYOUT_H