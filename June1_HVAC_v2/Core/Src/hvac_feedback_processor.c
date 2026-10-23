#include "hvac_feedback_processor.h"

// Use the existing ADC handle from main.c
extern ADC_HandleTypeDef hadc1;

// Coarse position table (ADC ranges)
typedef struct {
    uint16_t min;
    uint16_t max;
} adc_range_t;

// Positions: 0 (closed) to 5 (fully open)
static const adc_range_t coarse_table[6] = {
    {4001, 4095},    // 0
    {3750, 4000},    // 1
    {3480, 3749},    // 2
    {2500, 3479},    // 3
    {1000, 2499},    // 4
    {0,    999},     // 5
};

// Precise stop detection table
typedef struct {
    uint16_t min;
    uint16_t max;
} precise_stop_t;

static const precise_stop_t precise_stop_table[6] = {
    {4055, 4065}, // 0
    {3837, 3857}, // 1
    {3563, 3593}, // 2
    {3309, 3329}, // 3
    {2022, 2042}, // 4
    {315,  335},  // 5
};

static uint16_t latest_adc_value = 0;
static uint8_t current_flap_position = 0xFF; // 0xFF = invalid
static uint8_t feedback_valid_flag = 0;

void feedback_processor_init(void)
{
    latest_adc_value = 0;
    current_flap_position = 0xFF;
    feedback_valid_flag = 0;
}

void feedback_processor_update(void)
{
    HAL_StatusTypeDef status;
    uint32_t adc_val = 0;

    HAL_ADC_Start(&hadc1);
    status = HAL_ADC_PollForConversion(&hadc1, 10);
    if (status == HAL_OK)
    {
        adc_val = HAL_ADC_GetValue(&hadc1);
        latest_adc_value = (uint16_t)adc_val;
        feedback_valid_flag = 0;
        for (uint8_t i = 0; i < 6; ++i)
        {
            if (latest_adc_value >= coarse_table[i].min && latest_adc_value <= coarse_table[i].max)
            {
                current_flap_position = i;
                feedback_valid_flag = 1;
                break;
            }
        }
        if (!feedback_valid_flag)
        {
            current_flap_position = 0xFF;
        }
    }
    else
    {
        feedback_valid_flag = 0;
        current_flap_position = 0xFF;
    }
    HAL_ADC_Stop(&hadc1);
}

uint8_t feedback_processor_get_position(void)
{
    return current_flap_position;
}

uint8_t feedback_processor_is_valid(void)
{
    return feedback_valid_flag;
}

uint16_t feedback_processor_get_adc(void)
{
    return latest_adc_value;
}

uint8_t feedback_processor_in_precise_stop_window(uint8_t position)
{
    if (position > 5) return 0;
    if (!feedback_valid_flag) return 0;
    if (latest_adc_value >= precise_stop_table[position].min &&
        latest_adc_value <= precise_stop_table[position].max)
    {
        return 1;
    }
    return 0;
}