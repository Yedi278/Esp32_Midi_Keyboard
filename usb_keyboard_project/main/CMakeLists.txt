idf_component_register(SRCS "main.c" "custom_midi.c" "custom_gpio.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp_timer
                    REQUIRES esp_driver_gpio
                    
                    )
