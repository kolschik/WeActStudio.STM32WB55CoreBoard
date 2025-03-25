/*---------------------------------------
- WeAct Studio Official Link
- taobao: weactstudio.taobao.com
- aliexpress: weactstudio.aliexpress.com
- github: github.com/WeActStudio
- gitee: gitee.com/WeAct-TC
- blog: www.weact-tc.cn
---------------------------------------*/

#include "bsp.h"
#include "adc.h"

const uint32_t adc_inj[] = {LL_ADC_CHANNEL_15, LL_ADC_CHANNEL_16};
adc_t adc1 = {
    .reg_channels = 0,
    .reg_channels_num = 0,
    .inj_channels = &adc_inj,
    .inj_channels_num = sizeof(adc_inj) / sizeof(adc_inj[0]),
    .inj_channels_trig = LL_ADC_REG_TRIG_SOFTWARE,
    .cb = adc_cb,
    .sampling_time = LL_ADC_SAMPLINGTIME_24CYCLES_5,
    .a = ADC1
};

tmr_cc_t tim_frame = {
    .timer = TIM17,
    .channel = LL_TIM_CHANNEL_CH1,
    .mode = LL_TIM_OCMODE_FROZEN
};

tmr_cc_t tim1_frame = {
    .timer = TIM1,
    .channel = LL_TIM_CHANNEL_CH1,
    .mode = LL_TIM_OCMODE_FROZEN
};

void board_button_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // GPIO Ports Clock Enable
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();    
  __HAL_RCC_GPIOC_CLK_ENABLE();

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM17);
  //Configure GPIO pin : PtPin
  GPIO_InitStruct.Pin = KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);

  HAL_GPIO_WritePin(LCD_RES_PORT, LCD_RES_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_PWR_PORT, LCD_PWR_PIN, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = LCD_PWR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_PWR_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SPI_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPI_CS_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_RES_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_RES_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_DC_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_DC_PORT, &GPIO_InitStruct);  

    GPIO_InitTypeDef GPIO_InitStruct_2 = {0};   
    GPIO_InitStruct_2.Pin = GPIO_PIN_8;
    GPIO_InitStruct_2.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct_2.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct_2.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    tim_init(&tim_frame);
    tim_init(&tim1_frame);    
    tim_oc_init(&tim_frame);

    adc_init(&adc1);
}

uint8_t board_button_getstate(void)
{
  return HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin)==GPIO_PIN_SET?1:0;
}

void board_led_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
  
  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
}

void board_led_toggle(void)
{
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
}

void board_led_set(uint8_t set)
{
    if (set)
        HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
}




