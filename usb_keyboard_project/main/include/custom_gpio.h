#ifndef CUSTOM_GPIO_H
#define CUSTOM_GPIO_H

#include "driver/gpio.h"

#define BLOCKS_N 8
#define KEYS_N 8

#define KEY_STATE_BUFF_SIZE 5

//  B  | GPIO
//	B1 | 13
//	B2 | 12
//	B3 | 18
//	B4 | 16
//	B5 | 15
//	B6 | 17
//	B7 | 11
//	B8 | 14

//  K  | GPIO | ADC_CH
//	K1 | 1	  |	  0 
//	K2 | 2	  |	  1
//	K3 | 9	  |	  8
//	K4 | 7	  |	  6
//	K5 | 5	  |	  4
//	K6 | 4	  |	  3
//	K7 | 6	  |	  5
//	K8 | 10	  |	  9

// GPIO pins for each block & key
extern const gpio_num_t block_pins[BLOCKS_N];
extern const gpio_num_t key_pins[KEYS_N];

// Key state
extern uint64_t key_state;
extern uint64_t key_state_old;

extern uint64_t key_state_buff[KEY_STATE_BUFF_SIZE];

void custom_gpio_init(void);

int custom_gpio_convert(int block, int key);

#endif // CUSTOM_GPIO_H