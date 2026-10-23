#include "hvac_feedback_processor.h"
#include "main.h"

// ADC handle from main.c
extern ADC_HandleTypeDef hadc1;

// Coarse position table: [min, max] for each position (inclusive)
typedef struct {
    uint16_t min;
    uint16_t max;
} adc_range_t;

// Position 0: >4000, 1: 3750–4000, 2: 3480–3750, 3: 2500–3480, 4: 1000–2500, 5: 0–1000
static const adc_range_t coarse_table[6] = {
    {4001, 4095},    // 0
    {3750, 4000},    // 1
    {3480, 3749},    // 2
    {2500, 3479},    // 3
    {1000, 2499},    // 4
    {0,    999},     // 5
};

// Precise stop detection table: [min, max] for each position
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

void feedback_processor_init(void)
{
    latest_adc_value = 0;
    current_flap_position = 0;
    feedback_valid_flag = 0;
}

static uint8_t map_adc_to_position(uint16_t adc)
{
    for (uint8_t i = 0; i < 6; ++i)
    {
        if (adc >= coarse_table[i].min && adc <= coarse_table[i].max)
            return i;
    }
    return 0xFF;
}

void feedback_processor_update(void)
{
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        latest_adc_value = (uint16_t)HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);

        uint8_t pos = map_adc_to_position(latest_adc_value);
        if (pos != 0xFF)
        {
            current_flap_position = pos;
            feedback_valid_flag = 1;
        }
        else
        {
            feedback_valid_flag = 0;
        }
    }
    else
    {
        HAL_ADC_Stop(&hadc1);
        feedback_valid_flag = 0;
    }
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

uint8_t feedback_processor_in_precise_window(uint8_t target_pos)
{
    if (target_pos > 5) return 0;
    uint16_t adc = latest_adc_value;
    if (adc >= precise_table[target_pos].min && adc <= precise_table[target_pos].max)
        return 1;
    return 0;
}