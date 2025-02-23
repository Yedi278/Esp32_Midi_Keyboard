#ifndef CUSTOM_MIDI_H
#define CUSTOM_MIDI_H

#include <stdint.h>
#include <stdio.h>
#include "esp_log.h"
#include "esp_mac.h"
#include "tinyusb.h"
#include "esp_timer.h"

#include "custom_adc.h"
#include "keyboard_driver.h"

static const char *TAG = "midi";

#ifndef CONFIG_LOG_MAXIMUM_LEVEL
#define CONFIG_LOG_MAXIMUM_LEVEL 4
#endif

/** Helper defines **/

// Interface counter
enum interface_count {
#if CFG_TUD_MIDI
    ITF_NUM_MIDI = 0,
    ITF_NUM_MIDI_STREAMING,
#endif
    ITF_COUNT
};

// USB Endpoint numbers
enum usb_endpoints {
    // Available USB Endpoints: 5 IN/OUT EPs and 1 IN EP
    EP_EMPTY = 0,
#if CFG_TUD_MIDI
    EPNUM_MIDI,
#endif
};

/** TinyUSB descriptors **/

#define TUSB_DESCRIPTOR_TOTAL_LEN (TUD_CONFIG_DESC_LEN + CFG_TUD_MIDI * TUD_MIDI_DESC_LEN)

/**
 * @brief String descriptor
 */
static const char* s_str_desc[5] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "Yehan Edirisinghe",             // 1: Manufacturer
    "Usb Midi Keyboard",      // 2: Product
    "000001",              // 3: Serials, should use chip ID
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

esp_err_t custom_midi_init(void)
{
    ESP_LOGI(TAG, "USB initialization");

    tinyusb_config_t const tusb_cfg = {
        .device_descriptor = NULL, // If device_descriptor is NULL, tinyusb_driver_install() will use Kconfig
        .string_descriptor = s_str_desc,
        .string_descriptor_count = sizeof(s_str_desc) / sizeof(s_str_desc[0]),
        .external_phy = false,
        .configuration_descriptor = s_midi_cfg_desc,
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    ESP_LOGI(TAG, "USB initialization DONE");
    return ESP_OK;
}

static void midi_task_read_example(void *arg)
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

// Basic MIDI Messages
#define NOTE_OFF 0x80
#define NOTE_ON  0x90

void display_layout(){
    extern unsigned char key_layout[8];
    int i=0, j=0;
    ESP_LOGI("LAYOUT", "Displaying layout");

    for(; i<8; i++){
        for(; j>8; j++){
            printf("%d ", (key_layout[i] & (1 << j)) ? 1 : 0);
        }
    }
    printf("\n");
}

static void periodic_midi_write_example_cb(void *arg)
{   
    extern adc_oneshot_unit_handle_t adc1_handle;
    extern adc_cali_handle_t cali_handle;
    extern unsigned char key_layout[8];
    extern unsigned char kay_layout_old[8];

    resetKeysLayout(&key_layout[0], &kay_layout_old[0]);
    readKeysLayout(&adc1_handle,
                   &cali_handle,
                   &key_layout[0]);
    // displayKeysLayout(key_layout);

    static uint8_t const cable_num = 0; // MIDI jack associated with USB endpoint
    static uint8_t const channel = 0; // 0 for channel 1
    
    int i=0, j=0;
    if (tud_midi_mounted()) {

        for(i=0; i<8; i++){
            for(j=0; j<8; j++){
                if((key_layout[i] & (1 << j)) && !(kay_layout_old[i] & (1 << j)))
                {
                    if(key_layout[i] & (1 << j))
                    {
                        uint8_t note_on[3] = {NOTE_ON | channel, i*8+j , 127};
                        tud_midi_stream_write(cable_num, note_on, 3);
                    }else
                    {
                        uint8_t note_off[3] = {NOTE_OFF | channel, i*8+j, 0};
                        tud_midi_stream_write(cable_num, note_off, 3);
                    }
                }
            }
        }
    }
}


#endif // CUSTOM_MIDI_H