#include "custom_gpio.h"
#include "esp_log.h"

const char *TAG = "GPIO";

// GPIO pins for each block
const gpio_num_t block_pins[BLOCKS_N] = {
    GPIO_NUM_13,
    GPIO_NUM_12,
    GPIO_NUM_18,
    GPIO_NUM_16,
    GPIO_NUM_15,
    GPIO_NUM_17,
    GPIO_NUM_11,
    GPIO_NUM_14
};

// GPIO pins for each key
const gpio_num_t key_pins[KEYS_N] = {
    GPIO_NUM_1,
    GPIO_NUM_2,
    GPIO_NUM_9,
    GPIO_NUM_7,
    GPIO_NUM_5,
    GPIO_NUM_4,
    GPIO_NUM_6,
    GPIO_NUM_10
};

uint64_t key_state = 0ULL;
uint64_t key_state_old = 0ULL;
uint64_t key_state_buff[KEY_STATE_BUFF_SIZE] = {0};

void custom_gpio_init(void){

    ESP_LOGI(TAG, "Initializing GPIO");

    // BLOCKS

    // Configure the GPIOs
    gpio_config_t block_io_conf;
    //disable interrupt
    block_io_conf.intr_type = GPIO_INTR_DISABLE;

    //bit mask of the pins
    block_io_conf.pin_bit_mask = 0;
    for(int i=0; i<BLOCKS_N; ++i){
        block_io_conf.pin_bit_mask |= (1ULL << block_pins[i]);
    }
    
    //set as input mode
    block_io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-down mode
    block_io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    block_io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&block_io_conf);


    // KEYS

    gpio_config_t keys_io_conf;
    //disable interrupt
    keys_io_conf.intr_type = GPIO_INTR_DISABLE;

    //bit mask of the pins
    keys_io_conf.pin_bit_mask = 0;
    for(int i=0; i<KEYS_N; ++i){
        keys_io_conf.pin_bit_mask |= (1ULL << key_pins[i]);
    }

    //set as input mode
    keys_io_conf.mode = GPIO_MODE_OUTPUT;
    //enable pull-down mode
    keys_io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    keys_io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;

    gpio_config(&keys_io_conf);

    //dump gpio conf
    // gpio_dump_io_configuration(stdout, GPIO_NUM_MAX);

    for(int i=0; i<BLOCKS_N; ++i){
        gpio_set_level(block_pins[i], 0);
        gpio_set_level(key_pins[i], 0);
    }
    ESP_LOGI(TAG, "GPIO initialized");
}

int custom_gpio_convert(int block, int key){

    int note;

    switch (block)
    {
    case 0:
        note = key;
        break;
    case 1:
        note = key;
        if(key < 5) note= key + 8;
        break;
    case 2:
        note = key + 8;
        if(key < 5) note= key + 16;
        break;
    case 3:
        note = key + 16;
        if(key < 5) note= key + 24;
        break;
    case 4:
        note = key + 24;
        if(key < 5) note= key + 32;
        break;
    case 5:
        note = key + 32;
        if(key < 5) note= key + 40;
        break;
    case 6:
        note = key + 40;
        if(key < 5) note= key + 48;
        break;
    case 7:
        note = key + 48;
        if(key < 5) note= key + 56;
        break;
    
    default: // error sends -1
        note = 0;
        break;
    }

    return note;
}