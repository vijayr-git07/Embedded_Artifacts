#include "hvac_bsp.h"

// UART handle for HVAC commands
UART_HandleTypeDef huart_hvac;
// ADC handle for feedback
ADC_HandleTypeDef hadc_hvac;
// Timer handle for motor PWM
TIM_HandleTypeDef htim_motor_pwm;

// Local static GPIO/TIM init helpers
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_PWM_Init(void);

void hvac_bsp_init(void)
{
    // Enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();

    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_ADC1_Init();
    MX_TIM3_PWM_Init();
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Motor direction pins
    GPIO_InitStruct.Pin = HVAC_MOTOR_DIR_PIN_A | HVAC_MOTOR_DIR_PIN_B;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HVAC_MOTOR_DIR_GPIO_PORT, &GPIO_InitStruct);

    // Flap position LEDs
    GPIO_InitStruct.Pin = HVAC_LED0_GPIO_PIN | HVAC_LED1_GPIO_PIN | HVAC_LED2_GPIO_PIN |
                          HVAC_LED3_GPIO_PIN | HVAC_LED4_GPIO_PIN | HVAC_PWR_LED_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

static void MX_USART2_UART_Init(void)
{
    huart_hvac.Instance = HVAC_UART_INSTANCE;
    huart_hvac.Init.BaudRate = HVAC_UART_BAUDRATE;
    huart_hvac.Init.WordLength = UART_WORDLENGTH_8B;
    huart_hvac.Init.StopBits = UART_STOPBITS_1;
    huart_hvac.Init.Parity = UART_PARITY_NONE;
    huart_hvac.Init.Mode = UART_MODE_TX_RX;
    huart_hvac.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart_hvac.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart_hvac);
}

static void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    hadc_hvac.Instance = HVAC_ADC_INSTANCE;
    hadc_hvac.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc_hvac.Init.Resolution = ADC_RESOLUTION_12B;
    hadc_hvac.Init.ScanConvMode = DISABLE;
    hadc_hvac.Init.ContinuousConvMode = DISABLE;
    hadc_hvac.Init.DiscontinuousConvMode = DISABLE;
    hadc_hvac.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc_hvac.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc_hvac.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc_hvac.Init.NbrOfConversion = 1;
    hadc_hvac.Init.DMAContinuousRequests = DISABLE;
    hadc_hvac.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    HAL_ADC_Init(&hadc_hvac);

    sConfig.Channel = HVAC_ADC_CHANNEL;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(&hadc_hvac, &sConfig);
}

static void MX_TIM3_PWM_Init(void)
{
    TIM_OC_InitTypeDef sConfigOC = {0};

    htim_motor_pwm.Instance = HVAC_MOTOR_PWM_TIM;
    htim_motor_pwm.Init.Prescaler = 83; // 84MHz/84 = 1MHz
    htim_motor_pwm.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim_motor_pwm.Init.Period = 999; // 1kHz PWM
    htim_motor_pwm.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim_motor_pwm.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_PWM_Init(&htim_motor_pwm);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim_motor_pwm, &sConfigOC, HVAC_MOTOR_PWM_CHANNEL);
}