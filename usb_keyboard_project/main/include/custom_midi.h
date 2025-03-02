#ifndef CUSTOM_MIDI_H
#define CUSTOM_MIDI_H

#include <stdint.h>
#include "tinyusb.h"


#define TUSB_DESCRIPTOR_TOTAL_LEN (TUD_CONFIG_DESC_LEN + CFG_TUD_MIDI * TUD_MIDI_DESC_LEN)
// Basic MIDI Messages
#define NOTE_OFF 0x80
#define NOTE_ON  0x90


#define NOTE_START 3*12

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

extern int data[64];

void custom_midi_init(void);
void midi_task_read_example(void *arg);
void periodic_midi_write_example_cb(void *arg);

void periodic_midi_func(void);

#endif // CUSTOM_MIDI_H