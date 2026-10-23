#include "hvac_feedback_processor.h"

// Use the existing ADC handle from main.c
extern ADC_HandleTypeDef hadc1;

// Coarse position detection table (ADC ranges)
typedef struct {
    uint16_t min;
    uint16_t max;
} adc_range_t;

// 0: >4000, 1: 3750–4000, 2: 3480–3750, 3: 2500–3480, 4: 1000–2500, 5: 0–1000
static const adc_range_t coarse_table[6] = {
    {4001, 4095},    // 0
    {3750, 4000},    // 1
    {3480, 3749},    // 2
    {2500, 3479},    // 3
    {1000, 2499},    // 4
    {0,    999},     // 5
};

// Precise stop detection table (ADC min, max)
static const adc_range_t precise_table[6] = {
    {4055, 4065},    // 0
    {3837, 3857},    // 1
    {3563, 3593},    // 2
    {3309, 3329},    // 3
    {2022, 2042},    // 4
    {315,  335},     // 5
};

static uint16_t latest_adc_value = 0;
static uint8_t current_flap_position = 0;
static uint8_t feedback_valid_flag = 0;

static uint8_t map_adc_to_position(uint16_t adc)
{
    for (uint8_t i = 0; i < 6; ++i) {
        if (adc >= coarse_table[i].min && adc <= coarse_table[i].max) {
            return i;
        }
    }
    return 0xFF;
}

static uint8_t is_adc_in_precise_window(uint16_t adc, uint8_t position)
{
    if (position > 5) return 0;
    return (adc >= precise_table[position].min && adc <= precise_table[position].max) ? 1 : 0;
}

void feedback_processor_init(void)
{
    latest_adc_value = 0;
    current_flap_position = 0;
    feedback_valid_flag = 0;
}

void feedback_processor_update(void)
{
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        uint16_t adc = (uint16_t)HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        uint8_t pos = map_adc_to_position(adc);
        if (pos != 0xFF) {
            latest_adc_value = adc;
            current_flap_position = pos;
            feedback_valid_flag = 1;
        } else {
            feedback_valid_flag = 0;
        }
    } else {
        HAL_ADC_Stop(&hadc1);
        feedback_valid_flag = 0;
    }
}

uint8_t feedback_processor_get_position(uint8_t *out_position, uint16_t *out_adc, uint8_t *out_valid)
{
    if (out_position) *out_position = current_flap_position;
    if (out_adc) *out_adc = latest_adc_value;
    if (out_valid) *out_valid = feedback_valid_flag;
    return feedback_valid_flag;
}

uint8_t feedback_processor_is_valid(void)
{
    return feedback_valid_flag;
}

// Exported for use by flap_control
uint8_t feedback_processor_is_in_precise_window(uint8_t position)
{
    return is_adc_in_precise_window(latest_adc_value, position);
}