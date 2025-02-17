#ifndef KEYBOARD_LAYOUT_H
#define KEYBOARD_LAYOUT_H

#include "driver/gpio.h"

// Keyboard Layout
//
// BLOCKS:
//--------------------------------------------------------
//  0   |  1   |  2   |  3   |  4   |  5   |  6   |  7   |
//--------------------------------------------------------
// KEYS:
//--------------------------------------------------------
//         0       |        1        |     etc...   
//--------------------------------------------------------
// 0,1,2,3,4,5,6,7 | 0,1,2,3,4,5,6,7 |     etc...
//--------------------------------------------------------
// C,C#,D,D# etc...

/// Number of blocks in the keyboard
#define BLOCK_N 8
/// Number of keys in each block
#define KEYS_N  8

/// GPIO pins for each block
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

/// ADC channels for each key
const adc_channel_t key_pins[KEYS_N] = {
    ADC_CHANNEL_0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7
};

/// Keyboard layout (buffer)
unsigned char key_layout[8];

#endif // KEYBOARD_LAYOUT_H