#include "main.h"
#include "spi.h"
#include "dma.h"
#include "setup.h"
#include "cmsis_os.h"
#include "u8g2.h"


typedef enum{
    display_off,
    display_on
} display_pwr_et;

void spi_periph_init();
void StartLcd_task(void const * argument);
void lcd_init(void);
void lcd_pwr(display_pwr_et state);



uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_stm32_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void spi1_complete_payload();

void dig_print(int16_t dig, char* buf);