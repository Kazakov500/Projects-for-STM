#include "stm32f4xx.h"
#include "lcd.h"
#include "Delay.h"
#include "SYM.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "stm32f4_discovery.h"
#include "stm32f4_discovery_audio_codec.h"
#include "main.h"
#include "waverecorder.h"


int16_t audiodata[64];//this data is playing by speaker

extern uint16_t RecBuf0[MIC_FILTER_RESULT_LENGTH]; //buffer for filtered PCM data from MIC
extern uint16_t RecBuf1[MIC_FILTER_RESULT_LENGTH]; //buffer for filtered PCM data from MIC
extern uint8_t buffer_ready;


int p = 0;

	
void init_audio_dac(void);

void LOG(char* msg)
{
	LCD_clear_page(p);
	lcd_draw_str(msg, 0, p);
	p++;
	if (p>3) p = 0;
	delay_ms(200);
}


int main(void)
{
	//lcd_init();
	//Clear_LCD();
	
	//RCC_ClocksTypeDef RCC_Clocks;
	
	
  /* Initialize LEDS */
  //STM_EVAL_LEDInit(LED3);
  //STM_EVAL_LEDInit(LED4);
  //STM_EVAL_LEDInit(LED5);
  //STM_EVAL_LEDInit(LED6);
 
  /* Green Led On: start of application */
  //STM_EVAL_LEDOn(LED4);
	
	  /* SysTick end of count event each 10ms */
  //RCC_GetClocksFreq(&RCC_Clocks);
  //SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

	
  //EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO, 86, I2S_AudioFreq_16k);//init speaker (optional)
  //simple_rec_start();//initialise MIC and start capture data from MIC 
	//EVAL_AUDIO_Play((uint16_t*)(&audiodata[0]),MIC_FILTER_RESULT_LENGTH*2*2);//play data to speaker (optional)
  	
  
  /* Initialize the repeat status */
  //RepeatState = 0;
  //LED_Toggle = 7;	
	
	
	//LOG("> Kernel Loading...");
	
	//lcd_draw_int(0021, 0, 3);
	//lcd_draw_int(15, 0, 2);
	//lcd_draw_float(-123.456, 3, 10, 1);
	//lcd_draw_float(567.432, 3, 20, 2);
	//lcd_draw_int(15, 20, 3);
	//lcd_draw_int(12, 40, 0);

	/*for(int i=0; i<=120; i++) 
	{
		lcd_set_Y(i, 1);
		lcd_data("00000001");
		
		lcd_set_Y(i, 3);
		lcd_data("10000000");
	}
	lcd_set_Y(0, 2);
		lcd_data("11111111");
	lcd_set_Y(120, 2);
		lcd_data("11111111");
	
	for(int i=2; i<120-1; i++) 
	{
		lcd_set_Y(i, 2);
		lcd_data("01111110");
		delay_ms(10);
	}
	Clear_LCD();
	p = 0;
	
LOG("> LCD init...");
LOG("> start...");
LOG("> initialization...");

*/
	int del_led = 15;
	uint32_t i;
	
	
	

//LOG("> RCC init");
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;
//LOG("> GPIO init start...");

	GPIOD->MODER = 0x55000000;
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
//LOG("> GPIO init end");	
	

	
	//lcd_draw_str("à", 10, 1);
//LOG("ÝÒÀ ÔÈÃÍß ÇÀÏÀÕÀËÀ!!!");

	
	
//LOG("> Main loop start...");
	

	
	int k = 0;
	while(1)
	{
		/*
		int x = 16;
		for(int i=0; i<=120; i++)
			lcd_draw_vert_line(i, x+15*sin((i+k)/8.0));
		k++;*/
		
		/*
		//rand lines
		for(int i=0; i<=120-5; i+=5)
		{
			int pos_ = rand() % 32;
			lcd_draw_vert_line(i, pos_);
			lcd_draw_vert_line(i+1, pos_);
			lcd_draw_vert_line(i+2, pos_);
			lcd_draw_vert_line(i+3, pos_);
			lcd_draw_vert_line(i+4, pos_);
		}
		delay_us(5000);*/
	
	

		GPIOD->ODR |= 0x1000;
//LOG(">Green LED on...");
		//delay_ms(2);
		
		GPIOD->ODR |= 0x2000;
//LOG(">Orange LED on...");	
		//delay_ms(2);

		GPIOD->ODR |= 0x4000;
//LOG(">Read LED on...");
		//delay_ms(2);

		GPIOD->ODR |= 0x8000;
//LOG(">Blue LED on...");	
		//delay_ms(2);
		
		
	}
	
	
	
}



void EVAL_AUDIO_TransferComplete_CallBack(uint32_t* pBuffer, uint32_t Size)
{
  /*uint8_t i;
  //called after speaker data TransferComplete
  __asm__("nop");
//#ifdef AUDIO_MAL_MODE_NORMAL
  for (i=0;i<(MIC_FILTER_RESULT_LENGTH*2);i++)
  {
    if (buffer_ready == 1) {audiodata[i] = RecBuf1[i>>1];} else {audiodata[i] = RecBuf0[i>>1];}//make pseudo-stereo
  }
  
  EVAL_AUDIO_Play((uint16_t*)(&audiodata[0]),MIC_FILTER_RESULT_LENGTH*2*2);
//#endif*/
}