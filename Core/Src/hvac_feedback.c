#include "hvac_feedback.h"

// ADC boundaries for mapping (example: 12-bit ADC, 0-4095)
#define ADC_MIN_VALID      100
#define ADC_MAX_VALID      4000

// Flap position boundaries (5 positions, 4 ranges)
static const uint16_t adc_position_boundaries[HVAC_FLAP_POS_MAX] = {
    800,   // boundary between 0 and 1
    1600,  // boundary between 1 and 2
    2400,  // boundary between 2 and 3
    3200   // boundary between 3 and 4
};

static ADC_HandleTypeDef *feedback_adc = NULL;
static uint16_t latest_adc_value = 0;
static uint8_t current_flap_position = HVAC_FLAP_POS_INVALID;
static uint8_t feedback_valid_flag = 0;

void hvac_feedback_init(ADC_HandleTypeDef *hadc)
{
    feedback_adc = hadc;
    latest_adc_value = 0;
    current_flap_position = HVAC_FLAP_POS_INVALID;
    feedback_valid_flag = 0;
}

void hvac_feedback_update(void)
{
    if (feedback_adc == NULL) return;

    HAL_ADC_Start(feedback_adc);
    if (HAL_ADC_PollForConversion(feedback_adc, 10) == HAL_OK)
    {
        uint16_t adc_val = (uint16_t)HAL_ADC_GetValue(feedback_adc);
        HAL_ADC_Stop(feedback_adc);

        latest_adc_value = adc_val;

        if (adc_val < ADC_MIN_VALID || adc_val > ADC_MAX_VALID)
        {
            feedback_valid_flag = 0;
            current_flap_position = HVAC_FLAP_POS_INVALID;
            return;
        }

        // Map ADC value to position
        uint8_t pos = 0;
        while (pos < HVAC_FLAP_POS_MAX && adc_val > adc_position_boundaries[pos])
            pos++;

        current_flap_position = pos;
        feedback_valid_flag = 1;
    }
    else
    {
        HAL_ADC_Stop(feedback_adc);
        feedback_valid_flag = 0;
        current_flap_position = HVAC_FLAP_POS_INVALID;
    }
}

uint8_t hvac_feedback_get_position(uint8_t *valid)
{
    if (valid) *valid = feedback_valid_flag;
    return current_flap_position;
}