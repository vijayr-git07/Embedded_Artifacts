#include "hvac_feedback_processor.h"

// ADC handle provided by CubeMX project
extern ADC_HandleTypeDef hadc1;

// Coarse position detection table
typedef struct {
    flap_position_t pos;
    uint16_t min;
    uint16_t max;
} adc_coarse_range_t;

static const adc_coarse_range_t coarse_table[] = {
    { FLAP_POS_0, 4001, 4095 },
    { FLAP_POS_1, 3750, 4000 },
    { FLAP_POS_2, 3480, 3749 },
    { FLAP_POS_3, 2500, 3479 },
    { FLAP_POS_4, 1000, 2499 },
    { FLAP_POS_5,    0,  999 }
};

// Precise stop detection table
typedef struct {
    flap_position_t pos;
    uint16_t min;
    uint16_t max;
} adc_precise_range_t;

static const adc_precise_range_t precise_table[] = {
    { FLAP_POS_0, 4055, 4065 },
    { FLAP_POS_1, 3837, 3857 },
    { FLAP_POS_2, 3563, 3593 },
    { FLAP_POS_3, 3309, 3329 },
    { FLAP_POS_4, 2022, 2042 },
    { FLAP_POS_5,  315,  335 }
};

static uint16_t latest_adc_value = 0;
static flap_position_t current_flap_position = FLAP_POS_INVALID;
static uint8_t feedback_valid_flag = 0;

static flap_position_t map_adc_to_coarse_position(uint16_t adc)
{
    for (uint8_t i = 0; i < sizeof(coarse_table)/sizeof(coarse_table[0]); ++i) {
        if (adc >= coarse_table[i].min && adc <= coarse_table[i].max) {
            return coarse_table[i].pos;
        }
    }
    return FLAP_POS_INVALID;
}

void feedback_processor_init(void)
{
    latest_adc_value = 0;
    current_flap_position = FLAP_POS_INVALID;
    feedback_valid_flag = 0;
}

void feedback_processor_update(void)
{
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        uint16_t adc = (uint16_t)HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        latest_adc_value = adc;
        flap_position_t pos = map_adc_to_coarse_position(adc);
        if (pos != FLAP_POS_INVALID) {
            current_flap_position = pos;
            feedback_valid_flag = 1;
        } else {
            current_flap_position = FLAP_POS_INVALID;
            feedback_valid_flag = 0;
        }
    } else {
        HAL_ADC_Stop(&hadc1);
        current_flap_position = FLAP_POS_INVALID;
        feedback_valid_flag = 0;
    }
}

flap_position_t feedback_processor_get_position(void)
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

// Helper for precise stop detection
uint8_t feedback_processor_in_precise_window(flap_position_t target)
{
    uint16_t adc = latest_adc_value;
    for (uint8_t i = 0; i < sizeof(precise_table)/sizeof(precise_table[0]); ++i) {
        if (precise_table[i].pos == target) {
            if (adc >= precise_table[i].min && adc <= precise_table[i].max) {
                return 1;
            }
        }
    }
    return 0;
}