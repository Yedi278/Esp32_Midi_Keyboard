#include "custom_midi.h"
#include "custom_gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <stdint.h>

static const char *TAG = "MIDI";

/** TinyUSB descriptors **/
/**
 * @brief String descriptor
 */
static const char* s_str_desc[5] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "Yehan Edirisinghe",             // 1: Manufacturer
    "USB Keyboard",      // 2: Product
    "420690",              // 3: Serials, should use chip ID
    "Edi's Keyboard", // 4: MIDI
};

/**
 * @brief Configuration descriptor
 *
 * This is a simple configuration descriptor that defines 1 configuration and a MIDI interface
 */
static const uint8_t s_midi_cfg_desc[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_COUNT, 0, TUSB_DESCRIPTOR_TOTAL_LEN, 0, 100),

    // Interface number, string index, EP Out & EP In address, EP size
    TUD_MIDI_DESCRIPTOR(ITF_NUM_MIDI, 4, EPNUM_MIDI, (0x80 | EPNUM_MIDI), 64),
};

#if (TUD_OPT_HIGH_SPEED)
/**
 * @brief High Speed configuration descriptor
 *
 * This is a simple configuration descriptor that defines 1 configuration and a MIDI interface
 */
static const uint8_t s_midi_hs_cfg_desc[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_COUNT, 0, TUSB_DESCRIPTOR_TOTAL_LEN, 0, 100),

    // Interface number, string index, EP Out & EP In address, EP size
    TUD_MIDI_DESCRIPTOR(ITF_NUM_MIDI, 4, EPNUM_MIDI, (0x80 | EPNUM_MIDI), 512),
};
#endif // TUD_OPT_HIGH_SPEED


// ------------------------------------------------------------------------------------ //

/**
 * @brief Initialize the MIDI device
 * 
 */
void custom_midi_init(){
    ESP_LOGI(TAG, "USB initialization");

    tinyusb_config_t const tusb_cfg = {
        .device_descriptor = NULL, // If device_descriptor is NULL, tinyusb_driver_install() will use Kconfig
        .string_descriptor = s_str_desc,
        .string_descriptor_count = sizeof(s_str_desc) / sizeof(s_str_desc[0]),
        .external_phy = false,
#if (TUD_OPT_HIGH_SPEED)
        .fs_configuration_descriptor = s_midi_cfg_desc, // HID configuration descriptor for full-speed and high-speed are the same
        .hs_configuration_descriptor = s_midi_hs_cfg_desc,
        .qualifier_descriptor = NULL,
#else
        .configuration_descriptor = s_midi_cfg_desc,
#endif // TUD_OPT_HIGH_SPEED
    };
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    ESP_LOGI(TAG, "USB initialization DONE");
}

/**
 * @brief Read input MIDI data from USB
 * 
 * @param arg 
 */
void midi_task_read_example(void *arg)
{
    // The MIDI interface always creates input and output port/jack descriptors
    // regardless of these being used or not. Therefore incoming traffic should be read
    // (possibly just discarded) to avoid the sender blocking in IO
    uint8_t packet[4];
    bool read = false;
    for (;;) {
        vTaskDelay(1);
        while (tud_midi_available()) {
            read = tud_midi_packet_read(packet);
            if (read) {
                ESP_LOGI(TAG, "Read - Time (ms since boot): %lld, Data: %02hhX %02hhX %02hhX %02hhX",
                         esp_timer_get_time(), packet[0], packet[1], packet[2], packet[3]);
            }
        }
    }
}

void print_state(uint64_t key_state){
    for(int i=0; i<64; i++){
        printf("%d", key_state & (1ULL << i) ? 1 : 0);
    }
    printf("\n");
}

int key_state_buff_pos = 0;

void periodic_midi_func_new(void){

    for(int i=0; i<BLOCKS_N; i++){
        for(int j=0; j<KEYS_N; j++){

            for(int k=0; k<BLOCKS_N; k++){
                gpio_set_level(block_pins[k], 0);
                gpio_set_level(key_pins[k], 0);
            }

            gpio_set_level(key_pins[j], 1);

            key_state_buff[key_state_buff_pos] |= ((uint64_t)gpio_get_level(block_pins[i]) << custom_gpio_convert(i,j));
            
            gpio_set_level(key_pins[j], 0);
        }
    }
    
    if(tud_midi_mounted()){
        uint8_t note_on[3] = {NOTE_ON, 62, 127};
        uint8_t note_off[3] = {NOTE_OFF, 62, 0};

        uint64_t diff = 0xFFFFFFFFFFFFFFFF;

        for(int i=0; i<KEY_STATE_BUFF_SIZE; i++){
            diff &= key_state_buff[i];
        }

        if(key_state != key_state_old){
            
            uint64_t diff = (key_state & ~key_state_old) | (key_state_old & ~key_state);
            uint64_t on = diff & key_state;
            uint64_t off = diff & ~key_state;

            for(int i=0; i<64; i++){
                if(on & (1ULL << i)){

                    note_on[1] = i+12;
                    tud_midi_stream_write(0, note_on, 3);

                }else if(off & (1ULL << i)){

                    note_off[1] = i+12;
                    tud_midi_stream_write(0, note_off, 3);

                }
            }
        }
    }

    key_state_buff_pos = (key_state_buff_pos + 1) % KEY_STATE_BUFF_SIZE;
}

int key_state_arr[64];

void periodic_midi_func(void){

    key_state_old = key_state;
    key_state = 0;

    for(int i=0; i<BLOCKS_N; i++){
        for(int j=0; j<KEYS_N; j++){

            gpio_set_level(key_pins[j], 1);
            vTaskDelay(1 / portTICK_PERIOD_MS);

            key_state |= ((uint64_t)gpio_get_level(block_pins[i]) << custom_gpio_convert(i,j));
            vTaskDelay(1 / portTICK_PERIOD_MS);

            gpio_set_level(key_pins[j], 0);
            vTaskDelay(1 / portTICK_PERIOD_MS);
            
        }
    }

    if(tud_midi_mounted() || 1){

        uint8_t note_on[3] = {NOTE_ON, 62, 127};
        uint8_t note_off[3] = {NOTE_OFF, 62, 0};
        
        if(key_state != key_state_old){
            
            uint64_t diff = (key_state & ~key_state_old) | (key_state_old & ~key_state);
            uint64_t on = diff & key_state;
            uint64_t off = diff & ~key_state;

            for(int i=0; i<64; i++){
                if(on & (1ULL << i)){

                    note_on[1] = NOTE_START + i;
                    tud_midi_stream_write(0, note_on, 3);
                    ESP_LOGI(TAG, "Note on: %d", i+NOTE_START);

                }else if(off & (1ULL << i)){

                    note_off[1] = NOTE_START + i;
                    tud_midi_stream_write(0, note_off, 3);
                    ESP_LOGI(TAG, "Note off: %d", i+NOTE_START);

                }
            }
        }
    }
}