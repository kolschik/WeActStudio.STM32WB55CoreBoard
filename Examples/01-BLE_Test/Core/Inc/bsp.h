#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "tmr.h"

#ifndef KEY_Pin
#define KEY_Pin GPIO_PIN_13
#endif
#ifndef KEY_GPIO_Port
#define KEY_GPIO_Port GPIOC
#endif
#ifndef LED_Pin
#define LED_Pin GPIO_PIN_4
#endif
#ifndef LED_GPIO_Port
#define LED_GPIO_Port GPIOE
#endif


#define SPI_CS_PORT GPIOA
#define SPI_CS_PIN GPIO_PIN_2
#define LCD_RES_PORT GPIOA
#define LCD_RES_PIN GPIO_PIN_3
#define LCD_DC_PORT GPIOA
#define LCD_DC_PIN GPIO_PIN_4

#define SPI_SCLK_PORT GPIOA
#define SPI_SCLK_PIN GPIO_PIN_5

#define SPI_MOSI_PORT GPIOA
#define SPI_MOSI_PIN GPIO_PIN_7

#define ADC_VOLT_PORT GPIOA
#define ADC_VOLT_PIN GPIO_PIN_8

#define ADC_TERM_PORT GPIOA
#define ADC_TERM_PIN GPIO_PIN_9

#define LCD_PWR_PORT GPIOB
#define LCD_PWR_PIN GPIO_PIN_6

#define DS_DC_PORT GPIOB
#define DS_DC_PIN GPIO_PIN_3

    void board_button_init(void);
    uint8_t board_button_getstate(void);
    void board_led_init(void);
    void board_led_toggle(void);
    void board_led_set(uint8_t set);

#ifdef __cplusplus
}
#endif

#endif
