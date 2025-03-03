| Supported Targets | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | -------- | -------- | -------- |

# ESP MIDI KEYBOARD PROJECT

This project involves converting an old 80's keyboard with no digital output into a usb-c pluggable midi device.

## Prerequisites

* ESP-IDF [link](https://docs.espressif.com/projects/esp-idf/en/v5.4/esp32s3/get-started/index.html#installation)
* C++

Remember to configure for your board if not an esp32-s3

## Building on ESP-IDF

> idy.py build
> idf.py -p port flash monitor

# Working Principle

The esp is directly connected to the keys' pins that are defined in the *custom_gpio.c* file.

The keys 
