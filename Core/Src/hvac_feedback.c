#include "hvac_feedback.h"
#include "hvac_bsp.h"

#define HVAC_FLAP_POSITIONS 5

// Example ADC boundaries for 5 positions (0..4)
static const uint16_t adc_position_boundaries[HVAC_FLAP_POSITIONS+1] = {
    0,    // Min ADC value for position 0
    820,  // Min ADC value for position 1
    1640, // Min ADC value for position 2
    2460, // Min ADC value for position 3
    3280, // Min ADC value for position 4
    4096  // Max ADC value + 1
};

static uint16_t latest_adc_value = 0;
static uint8_t current_flap_position = 0;
static uint8_t feedback_valid_flag = 0;

void hvac_feedback_init(void)
{
    latest_adc_value = 0;
    current_flap_position = 0;
    feedback_valid_flag = 0;
}

void hvac_feedback_update(void)
{
    HAL_StatusTypeDef status;
    uint32_t adc_val = 0;

    HAL_ADC_Start(HVAC_BSP_ADC_HANDLE);
    status = HAL_ADC_PollForConversion(HVAC_BSP_ADC_HANDLE, 10);
    if (status == HAL_OK)
    {
        adc_val = HAL_ADC_GetValue(HVAC_BSP_ADC_HANDLE);
        HAL_ADC_Stop(HVAC_BSP_ADC_HANDLE);

        // Validate ADC range
        if (adc_val < 4096)
        {
            latest_adc_value = (uint16_t)adc_val;
            feedback_valid_flag = 1;

            // Map ADC value to position
            for (uint8_t i = 0; i < HVAC_FLAP_POSITIONS; ++i)
            {
                if (latest_adc_value >= adc_position_boundaries[i] &&
                    latest_adc_value < adc_position_boundaries[i+1])
                {
                    current_flap_position = i;
                    break;
                }
            }
        }
        else
        {
            feedback_valid_flag = 0;
        }
    }
    else
    {
        HAL_ADC_Stop(HVAC_BSP_ADC_HANDLE);
        feedback_valid_flag = 0;
    }
}

uint8_t hvac_feedback_get_position(void)
{
    return current_flap_position;
}

uint8_t hvac_feedback_is_valid(void)
{
    return feedback_valid_flag;
}

uint16_t hvac_feedback_get_adc(void)
{
    return latest_adc_value;
}