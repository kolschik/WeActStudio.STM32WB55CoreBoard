/*---------------------------------------
- WeAct Studio Official Link
- taobao: weactstudio.taobao.com
- aliexpress: weactstudio.aliexpress.com
- github: github.com/WeActStudio
- gitee: gitee.com/WeAct-TC
- blog: www.weact-tc.cn
---------------------------------------*/

#include "bsp.h"

void board_button_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  // GPIO Ports Clock Enable
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();    
  __HAL_RCC_GPIOC_CLK_ENABLE();


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
