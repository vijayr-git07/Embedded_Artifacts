#include "hvac_feedback.h"

// ADC boundaries for 5 positions (example values, 12-bit ADC)
#define ADC_MIN_VALID      100
#define ADC_MAX_VALID      4000

static const uint16_t adc_pos_boundaries[6] = { 100, 900, 1700, 2500, 3300, 4000 }; // 5 positions: 0-4

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

        if (adc_val < ADC_MIN_VALID || adc_val > ADC_MAX_VALID) {
            feedback_valid_flag = 0;
            current_flap_position = HVAC_FLAP_POS_INVALID;
            return;
        }

        // Map ADC value to position
        for (uint8_t i = 0; i < 5; ++i) {
            if (adc_val >= adc_pos_boundaries[i] && adc_val < adc_pos_boundaries[i+1]) {
                current_flap_position = i;
                feedback_valid_flag = 1;
                return;
            }
        }
        // Out of mapped range
        feedback_valid_flag = 0;
        current_flap_position = HVAC_FLAP_POS_INVALID;
    } else {
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