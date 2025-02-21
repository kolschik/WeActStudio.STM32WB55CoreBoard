#include "lcd.h"
#include "bsp.h"
#include "main.h"
volatile int uii;
static void lcd_frame_refresh();
static void frame_refresh_timer_cb();
static u8g2_t u8g2;
/*
#include "string.h"
extern 

int16_t t_meas, t_junc, t_ext = UNREAL_TEMP;
uint8_t fault_code;
extern int16_t v_disp;
extern uint8_t global_pwr;

osThreadId Lcd_taskHandle;
uint32_t Lcd_taskBuffer[ LCD_TASK_STACK_SIZE ];
osStaticThreadDef_t Lcd_taskControlBlock;

osMutexId SpimutexHandle;
osStaticMutexDef_t SpimutexControlBlock;



void spi_periph_init()
{

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Peripheral clock enable 
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  ///SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA6   ------> SPI1_MISO
  PA7   ------> SPI1_MOSI
  
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  spi_init(SPI1, 500000,  LL_SPI_POLARITY_LOW | LL_SPI_PHASE_1EDGE);
  dma1_init(spi1_tx_dma, LL_DMA_REQUEST_1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH, LL_DMA_PRIORITY_MEDIUM);

  osMutexStaticDef(Spimutex, &SpimutexControlBlock);
  SpimutexHandle = osMutexCreate(osMutex(Spimutex));    

  osThreadStaticDef(Lcd_task, StartLcd_task, osPriorityNormal, 0, LCD_TASK_STACK_SIZE, Lcd_taskBuffer, &Lcd_taskControlBlock);
  Lcd_taskHandle = osThreadCreate(osThread(Lcd_task), NULL);

  NVIC_EnableIRQ     (SPI1_IRQn);
  NVIC_SetPriority(SPI1_IRQn, 5);

  NVIC_EnableIRQ     (DMA1_Channel2_3_IRQn);
  NVIC_SetPriority(DMA1_Channel2_3_IRQn, 5);
}



void StartLcd_task(void const * argument)
{
    int rv;
    uint8_t pwr, pwr_l;
    uint32_t last_meas = TERMOCOUPLE_MEAS + 1;
    lcd_init();
    for(;;)
    {
      pwr = global_pwr;
      if(pwr)
      {
          if (!pwr_l) u8g2_SetPowerSave(&u8g2, 0);
          
          if (xTaskGetTickCount() - last_meas > TERMOCOUPLE_MEAS) 
          {
              max_31855_meas(&t_meas, &t_junc, &fault_code);
              last_meas = xTaskGetTickCount();
          }
          
          //print_ext();
          print_main();
      }
      else if (pwr_l)
      {
          u8g2_SetPowerSave(&u8g2, 1);
      }
      pwr_l = pwr;
      osDelay(100);
    }

}
*/


void lcd_pwr(display_pwr_et state)
{
    HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, !state);
}


extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;

volatile int done = 0;
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);
  done =1 ;
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxCpltCallback should be implemented in the user file
   */
}



uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  int rv;
  switch (msg)
  {
  case U8X8_MSG_BYTE_SEND:
    done = 0;
    rv = HAL_SPI_Transmit_DMA(&hspi1, arg_ptr, arg_int);
    
    if (rv)
    {
      uii = rv;
    }
    while (done == 0){

    }
    break;

  case U8X8_MSG_BYTE_INIT:
    break;

  case U8X8_MSG_BYTE_SET_DC:
    HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, arg_int);

   // arg_int ? HAL_GPIO_SetOutputPin(OLED_DC_GPIO_Port, OLED_DC_Pin) : LL_GPIO_ResetOutputPin(OLED_DC_GPIO_Port, OLED_DC_Pin);
    break;

  case U8X8_MSG_BYTE_START_TRANSFER:
    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, 0);// CS=0;
    break;
  case U8X8_MSG_BYTE_END_TRANSFER:
    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, 1);// CS=1;
    break;
  default:
    return 0;
  }
  return 1;
}

uint8_t u8x8_stm32_gpio_and_delay_cb(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  switch (msg)
  {
  case U8X8_MSG_GPIO_AND_DELAY_INIT:
    HAL_Delay(1);
    break;
  case U8X8_MSG_DELAY_MILLI:
    HAL_Delay(arg_int);
    break;
  case U8X8_MSG_GPIO_DC:
      HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, arg_int);
    //arg_int ? LL_GPIO_SetOutputPin(OLED_DC_GPIO_Port, OLED_DC_Pin) : LL_GPIO_ResetOutputPin(OLED_DC_GPIO_Port, OLED_DC_Pin);
    break;
  case U8X8_MSG_GPIO_RESET:
      HAL_GPIO_WritePin(LCD_RES_PORT, LCD_RES_PIN, arg_int);

    //arg_int ? LL_GPIO_SetOutputPin(OLED_RES_GPIO_Port, OLED_RES_Pin) : LL_GPIO_ResetOutputPin(OLED_RES_GPIO_Port, OLED_RES_Pin);
    break;
  }
  return 1;
}

#define BAT_0 1050
#define BAT_1 1100
#define BAT_2 1150
#define BAT_3 1200
#define BAT_4 1250
#define BAT_F 1400
#define SNOW_THR 500
#define UNREAL_TEMP 32767

int t_ext = 100;
int v_disp = 1380;
int fault_code = 0;
int t_meas = 0;

typedef struct 
{
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
}time_et;

void print_main()
{
    time_et time_dec;
    char str_indoor[6]; 
    char str_volt[6];        
    char time[6];

    rtc_get_time(&time_dec, &time);

    if (t_ext != UNREAL_TEMP)
    {
        dig_print(t_ext, str_indoor);
        str_indoor[2]=0;
    }
    else
    {
        strcpy(str_indoor, "..");
    }

    u8g2_FirstPage(&u8g2);
    do
    {   
        u8g2_SetFont(&u8g2, u8g2_font_inb21_mn);
        u8g2_DrawStr(&u8g2, 0, 26, &time);

        u8g2_SetFont(&u8g2, u8g2_font_10x20_mn);
        u8g2_DrawStr(&u8g2, 100, 16, str_indoor);
        u8g2_DrawCircle(&u8g2, 124, 5, 2, U8G2_DRAW_ALL);
        u8g2_DrawVLine(&u8g2, 93, 0, 31);


        u8g2_SetFont(&u8g2, u8g2_font_battery19_tn);
        u8g2_SetFontDirection(&u8g2, 1);
        char bat[2];
        if (v_disp < BAT_0) bat[0] = '0';
        else if (v_disp < BAT_1) bat[0] = '1';
        else if (v_disp < BAT_2) bat[0] = '2';        
        else if (v_disp < BAT_3) bat[0] = '3';
        else if (v_disp < BAT_4) bat[0] = '4';        
        else bat[0] = '5';
        if (v_disp > BAT_F) bat[0] = '6';
        bat[1] = 0;
        u8g2_DrawStr(&u8g2, 100, 24, bat);
        u8g2_SetFontDirection(&u8g2, 0);

        u8g2_SetFont(&u8g2, u8g2_font_6x12_t_symbols);
        if (fault_code) u8g2_DrawUTF8(&u8g2, 122, 31,"\u2717");
        else if (t_meas > SNOW_THR) u8g2_DrawUTF8(&u8g2, 122, 31,"\u2717");
        else u8g2_DrawUTF8(&u8g2, 122, 31,"*");

    }   while (u8g2_NextPage(&u8g2));
}


void rtc_get_time(time_et *time, char *str)
{
    time->hour = LL_RTC_TIME_GetHour(RTC); 
    time->min = LL_RTC_TIME_GetMinute(RTC);
    time->sec = LL_RTC_TIME_GetSecond(RTC);

    if ((time->hour != LL_RTC_TIME_GetHour(RTC)) || (time->min != LL_RTC_TIME_GetMinute(RTC)) || (time->sec != LL_RTC_TIME_GetSecond(RTC)))
    {
        time->hour = LL_RTC_TIME_GetHour(RTC);
        time->min = LL_RTC_TIME_GetMinute(RTC);
        time->sec = LL_RTC_TIME_GetSecond(RTC);
    }
    
    if (str)
    {
      str[0] = time->hour & 0xf0 ? ((time->hour >> 4) & 0x0f) + '0' : ' ';
      str[1] = ((time->hour >> 0) & 0x0f) + '0';
      str[2] = time->sec & 0x01 ? ':' : ' ';
      str[3] = ((time->min >> 4) & 0x0f) + '0';
      str[4] = ((time->min >> 0) & 0x0f) + '0';    
      str[5] = 0;
    }
}
/*
void print_ext()
{
    time_et time_dec;
    char str_indoor[6];
    char str_outdoor[6];      
    char str_volt[6];        
    char time[6];

    rtc_get_time(&time_dec, &time);

    if (t_ext != UNREAL_TEMP)
    {
        dig_print(t_ext, str_indoor);
    }
    else
    {
        strcpy(str_indoor, "   ");
    }

    dig_print(v_disp, str_volt);

    switch (fault_code)
    {

      case 1: 
        strcpy(str_outdoor, "opn");
        break;
      case 2: 
        strcpy(str_outdoor, "Sh+");
        break;
      case 4: 
        strcpy(str_outdoor, "Sh-");
        break;   
      default:
        dig_print(t_meas * 10, str_outdoor);
        break;         
    }

    
    u8g2_FirstPage(&u8g2);
    do
    {   
        u8g2_SetFont(&u8g2, u8g2_font_inb21_mn);
        u8g2_DrawStr(&u8g2, 0, 26, &time);

        u8g2_SetFont(&u8g2, u8g2_font_6x12_t_symbols);//u8g2_SetFont(&u8g2, u8g2_font_6x12_t_cyrillic);
        u8g2_DrawUTF8(&u8g2, 103, 31, str_indoor);
        u8g2_DrawUTF8(&u8g2, 103, 21, str_volt);
        u8g2_DrawUTF8(&u8g2, 103, 10, str_outdoor);     

        u8g2_DrawVLine(&u8g2, 93, 0, 31);

        u8g2_DrawUTF8(&u8g2, 96, 31,"\u2600");
        u8g2_DrawUTF8(&u8g2, 96, 20,"\u00b1");      
        u8g2_DrawUTF8(&u8g2, 96, 10,"\u25b9");

    }   while (u8g2_NextPage(&u8g2));
}

void spi1_complete_payload()
{
    BaseType_t xHigherPriorityTaskWoken;
    vTaskNotifyGiveFromISR( Lcd_taskHandle, &xHigherPriorityTaskWoken );
}
*/
void dig_print(int16_t dig, char* buf)
{
        if (dig > 9999)
        {
          strcpy(buf, "Hi");
        }
        else if (dig < -9900)
        {
          strcpy(buf, "Lo");
        }
        else if(dig >= 0)
        {
            int16_t temp = dig / 100;
            *buf++  = (temp / 10) ? (temp / 10) + '0' : ' ';           
            *buf++  = temp % 10 + '0';
            *buf++  = '.';
            *buf++ = (dig % 100) / 10 + '0';
        }
        else
        {
            dig = dig / (-100);
            *buf++ = '-';
            *buf++ = dig / 10 + '0';
            *buf++ = dig % 10 + '0';
            *buf++ = 0;            
        }
        *buf++ = 0;
}

uint8_t frame_refresh_timer;
int lcd_init(){

    HW_TS_Create(CFG_TIM_PROC_ID_ISR, &frame_refresh_timer, hw_ts_Repeated, frame_refresh_timer_cb);    
    UTIL_SEQ_RegTask(1<< CFG_TASK_DISPLAY, UTIL_SEQ_RFU, lcd_frame_refresh);


    lcd_pwr(display_on);
  
      u8g2_Setup_ssd1305_128x32_adafruit_1(&u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay_cb);
      u8g2_InitDisplay(&u8g2);
  
      u8g2_SetPowerSave(&u8g2, 0);
      
      const uint32_t fps = 100*1000/CFG_TS_TICK_VAL; // 10Hz
      HW_TS_Start(frame_refresh_timer, fps);



    return 0;
}
uint32_t tick;

void lcd_frame_refresh(){
    print_main();
}

void frame_refresh_timer_cb(){
    UTIL_SEQ_SetTask(1<<CFG_TASK_DISPLAY, CFG_SCH_PRIO_0);
}