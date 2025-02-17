#ifndef CUSTOM_GPIO_H
#define CUSTOM_GPIO_H

#include <stdio.h>
#include "driver/gpio.h"

#include "keyboard_layout.h"

void custom_gpio_init(){

    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;

    //bit mask
    io_conf.pin_bit_mask = 0;
    for(int i=0; i<BLOCK_N; i++){
        io_conf.pin_bit_mask |= (1ULL << block_pins[i]);
    }

    //enable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    //disable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;

    gpio_config(&io_conf);

    for(int i=0; i<BLOCK_N; i++){
        gpio_set_level(block_pins[i], 0);
    }

    // gpio_dump_io_configuration(stdout, io_conf.pin_bit_mask);
    // printf("\n");
}



#endif // CUSTOM_GPIO_H