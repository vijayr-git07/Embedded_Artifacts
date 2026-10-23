#include "hvac_feedback_processor.h"
#include "main.h"

// ADC handle from main.c
extern ADC_HandleTypeDef hadc1;

// Coarse position table (ADC ranges)
typedef struct {
    uint16_t min;
    uint16_t max;
} adc_range_t;

#define FLAP_POSITIONS 6

static const adc_range_t coarse_table[FLAP_POSITIONS] = {
    {4001, 4095},    // 0: >4000
    {3750, 4000},    // 1: 3750–4000
    {3480, 3749},    // 2: 3480–3750
    {2500, 3479},    // 3: 2500–3480
    {1000, 2499},    // 4: 1000–2500
    {0,    999}      // 5: 0–1000
};

// Precise stop detection table
typedef struct {
    uint16_t min;
    uint16_t max;
} adc_stop_window_t;

static const adc_stop_window_t stop_table[FLAP_POSITIONS] = {
    {4055, 4065}, // 0
    {3837, 3857}, // 1
    {3563, 3593}, // 2
    {3309, 3329}, // 3
    {2022, 2042}, // 4
    {315,  335}   // 5
};

static uint16_t latest_adc_value = 0;
static uint8_t current_flap_position = 0;
static uint8_t feedback_valid_flag = 0;

void feedback_processor_init(void)
{
    latest_adc_value = 0;
    current_flap_position = 0;
    feedback_valid_flag = 0;
}

void feedback_processor_update(void)
{
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        uint16_t adc = (uint16_t)HAL_ADC_GetValue(&hadc1);
        latest_adc_value = adc;
        feedback_valid_flag = 0;
        for (uint8_t i = 0; i < FLAP_POSITIONS; ++i)
        {
            if (adc >= coarse_table[i].min && adc <= coarse_table[i].max)
            {
                current_flap_position = i;
                feedback_valid_flag = 1;
                break;
            }
        }
    }
    HAL_ADC_Stop(&hadc1);
}

uint8_t feedback_processor_get_position(uint8_t *out_pos)
{
    if (feedback_valid_flag)
    {
        if (out_pos) *out_pos = current_flap_position;
        return 1;
    }
    return 0;
}

uint8_t feedback_processor_is_valid(void)
{
    return feedback_valid_flag;
}

uint16_t feedback_processor_get_adc(void)
{
    return latest_adc_value;
}

uint8_t feedback_processor_in_stop_window(uint8_t target_pos)
{
    if (target_pos >= FLAP_POSITIONS)
        return 0;
    uint16_t adc = latest_adc_value;
    const adc_stop_window_t *win = &stop_table[target_pos];
    return (adc >= win->min && adc <= win->max);
}