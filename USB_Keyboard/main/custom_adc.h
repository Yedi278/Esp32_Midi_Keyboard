#ifndef CUSTOM_ADC_H
#define CUSTOM_ADC_H

#include "esp_log.h"
#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"
#include "keyboard_layout.h"

#define CONFIG_LOG_MAXIMUM_LEVEL 4

/**
 * @brief ADC Calibration Function
 * @param cali_handle pointer to the calibration handle
 */
static esp_err_t custom_adc_calibrate(adc_cali_handle_t* cali_handle){

    ESP_LOGI("ADC - CALIBRATION", "Calibrating ADC");
    adc_cali_scheme_ver_t scheme;
    ESP_ERROR_CHECK(adc_cali_check_scheme(&scheme));

    ESP_LOGI("ADC - CALIBRATION", "calibration scheme version is %s", "Curve Fitting");
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_curve_fitting(&cali_config, cali_handle));

    ESP_LOGI("ADC - CALIBRATION", "ADC Calibrated");
    return ESP_OK;
}

/**
 * @brief ADC Initialization Function
 * @param adc1_handle pointer to the adc handle
 * @param cali_handle pointer to the calibration handle
 */
void custom_adc_init(adc_oneshot_unit_handle_t *adc1_handle, adc_cali_handle_t *cali_handle){
    
    ESP_LOGI("ADC", "Initializing ADC");
    
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_12,
    };

    for(int i=0; i<8; i++){
        ESP_ERROR_CHECK(adc_oneshot_config_channel(*adc1_handle, key_pins[i] , &config));
    }

    custom_adc_calibrate(cali_handle);

    ESP_LOGI("ADC", "ADC Initialized");
}

void custom_adc_read(unsigned char* key_layout, adc_oneshot_unit_handle_t adc1_handle, adc_cali_handle_t cali_handle){

    extern const adc_channel_t key_pins[8];
    extern const gpio_num_t block_pins[8];

    int i=0, j=0, val=0;

    for(; i<8; i++){

        gpio_set_level(block_pins[i], 1);

        for(; j<8; j++)
        {
             
            if (adc_oneshot_get_calibrated_result(adc1_handle, cali_handle, key_pins[j], &val) != ESP_OK) {
                ESP_LOGE("ADC", "Failed to get ADC value");
                continue;
            }
            if(val > 2000)
            {
                if(key_layout[i] & (1 << j))
                {
                    key_layout[i] &= ~(1 << j);
                }else
                {
                    key_layout[i] |= (1 << j);
                }
            }
        }
        
        gpio_set_level(block_pins[i], 0);
    }
}

/**
 * @brief ADC Calibration Test Function
 * @details This function checks the calibration value of the ADC by reading the HIGH value and LOW value on the GPIO_PIN
the pin should be directly connect to each other physically.
 * @param ADC_CHANN ADC Channel
 * @param adc1_handle pointer to the adc handle
 * @param cali_handle pointer to the calibration handle
 */
void check_cali(void *arg){

    gpio_num_t GPIO_PIN = GPIO_NUM_37;
    adc_channel_t ADC_CHANN = ADC_CHANNEL_6;
    extern adc_oneshot_unit_handle_t adc1_handle;
    extern adc_cali_handle_t cali_handle;

    ESP_LOGI("Calibration Value", "Checking Calibration Value");

    gpio_set_level(GPIO_PIN, 1);

    int val;
    ESP_ERROR_CHECK(adc_oneshot_get_calibrated_result(adc1_handle, cali_handle, ADC_CHANN, &val));
    ESP_LOGI("Calibration Value", "%d mV on HIGH\n", val);

    gpio_set_level(GPIO_PIN, 0);
    ESP_ERROR_CHECK(adc_oneshot_get_calibrated_result(adc1_handle, cali_handle, ADC_CHANN, &val));
    ESP_LOGI("Calibration Value", "%d mV on LOW\n", val);
}

#endif // CUSTOM_ADC_H
