#include "stm32f4xx.h"
#include "lcd.h"
#include "Delay.h"
#include "SYM.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "waverecorder.h"
#include "stm32f4_discovery_audio_codec.h"
#include "main.h"


#include "arm_math.h"
#include "defines.h"

int16_t audiodata[MIC_FILTER_RESULT_LENGTH*2];//this data is playing by speaker

extern int16_t RecBuf0[MIC_FILTER_RESULT_LENGTH]; //buffer for filtered PCM data from MIC
extern int16_t RecBuf1[MIC_FILTER_RESULT_LENGTH]; //buffer for filtered PCM data from MIC
extern uint8_t buffer_ready;



	arm_cfft_radix4_instance_f32 S;	/* ARM CFFT module */
	float32_t maxValue;				/* Max FFT value is stored here */
	uint32_t maxIndex;				/* Index in Output array where max value is */
	uint16_t i;


int p = 0;

int vol = 90;

#define COL 2
int k = 0;
/* FFT settings */

#define SAMPLES                    16 * COL             /* 256 real party and 256 imaginary parts */
#define INP_SAMPLES                    2*16 * COL             /* 256 real party and 256 imaginary parts */
#define FFT_SIZE                INP_SAMPLES / 2        /* FFT size is always the same size as we have samples, so 256 in our case */
 
int16_t audio_data[SAMPLES];
 
/* Global variables */
float32_t Input[INP_SAMPLES];
float32_t Output[FFT_SIZE];
 
 
 
 
 
 

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
	
	
	
	
	

	//delay_ms(100);
	
	RCC_ClocksTypeDef RCC_Clocks;

  /* Initialize LEDs and User_Button on STM32F4-Discovery --------------------*/
  //STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO); 
  
  //STM_EVAL_LEDInit(LED4);
  //STM_EVAL_LEDInit(LED3);
  //STM_EVAL_LEDInit(LED5);
  //STM_EVAL_LEDInit(LED6);
  
	
	

	//RCC_GetClocksFreq(&RCC_Clocks);//test
	lcd_init();
	Clear_LCD();
	LOG("> Kernel Loading...");  
	
	
  EVAL_AUDIO_Init(OUTPUT_DEVICE_HEADPHONE, 60, I2S_AudioFreq_16k);//init speaker (optional)16k I2S_AudioFreq_16k 
  simple_rec_start();//initialise MIC and start capture data from MIC
  //EVAL_AUDIO_Play((uint16_t*)(&audiodata[0]),MIC_FILTER_RESULT_LENGTH*2*2);//play data to speaker (optional)
  EVAL_AUDIO_Play((uint16_t*)(&RecBuf0[0]),MIC_FILTER_RESULT_LENGTH*2*2);

	
	
	//EVAL_AUDIO_VolumeCtl(15);
	
	//RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;

	//GPIOD->MODER = 0x55000000;
	//GPIOD->OTYPER = 0;
	//GPIOD->OSPEEDR = 0;
	//uint16_t *audBuf;
  while(1)
  {
		//delay_ms(1000);
				//rand lines
		/*for(int i=0; i<=120-5; i+=5)
		{
			int pos_ = rand() % 32;
			lcd_draw_vert_line(i, pos_);
			lcd_draw_vert_line(i+1, pos_);
			lcd_draw_vert_line(i+2, pos_);
			lcd_draw_vert_line(i+3, pos_);
			lcd_draw_vert_line(i+4, pos_);
		}
		delay_us(5000);*/
    //__asm__("nop");
		//if (vol <= 1) vol = 90;
		//EVAL_AUDIO_VolumeCtl(vol--);
		//delay_ms(300);
		if (k >= COL)
		{
			//if (k > col) k = 0;
			
			int tt = 0;
			float32_t INP[INP_SAMPLES];
			for (i=0;i<(SAMPLES);i++)
			{	  
					INP[tt++] = audio_data[i]/ (65535.0);
					INP[tt++] = 0;
			}
			
			
			
			/* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
			//arm_cfft_radix4_init_f32(&S, FFT_SIZE, 0, 1);
		
			/* Process the data through the CFFT/CIFFT module */
			//arm_cfft_radix4_f32(&S, INP);
			arm_cfft_instance_f32 SS;
			arm_cfft_f32(&SS, INP, 0, 1);
			
			
			/* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
			//arm_cmplx_mag_f32(INP, Output, FFT_SIZE);
			arm_cmplx_mag_f32(INP, Output, FFT_SIZE);//*2
			
			
			/* Calculates maxValue and returns corresponding value */
			//arm_max_f32(Output, FFT_SIZE/2, &maxValue, &maxIndex);
			
			/*int draw[16];
			for(int k = 0; k < 16; k++)
			{
				draw[k] = ((RecBuf0[k] / 65535.0)+1.0) * 14;
			}*/
			//int kk = 0;
			/*for(int t = 0; t < FFT_SIZE; t++)
			{
				lcd_draw_vert_line(t, 30*((audio_data[t]+32767.0)/(65535.0)));//Output[t]*1000
			}*/
			
			k = 0;
			//EVAL_AUDIO_Play((uint16_t*)(&RecBuf0[0]),MIC_FILTER_RESULT_LENGTH*2*2);
		}
		//GPIOD->ODR |= 0x1000;
		//GPIOD->ODR |= 0x2000;
		//GPIOD->ODR |= 0x4000;
		//GPIOD->ODR |= 0x8000;
  }
	
	
	
	
	
}


void EVAL_AUDIO_TransferComplete_CallBack(uint32_t* pBuffer, uint32_t Size)
{
  //uint8_t i;
	//Clear_LCD();
	
  //called after speaker data TransferComplete
	//for (int i=0;i<28;i++) __asm__("nop");
	
	#ifdef AUDIO_MAL_MODE_NORMAL
  /*for (i=0;i<(MIC_FILTER_RESULT_LENGTH*2);i++)
  {
    if (buffer_ready == 1) 
			{audiodata[i] = RecBuf1[i>>1];} else 
			{audiodata[i] = RecBuf0[i>>1];}//make pseudo-stereo
  }*/
	
  /*__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");*/
	if (k < COL) 
	{
		for(int i = 0; i<16; i++)
			audio_data[(k*16)+i] = RecBuf0[i];
		k++;	
		delay_us(1000);
		EVAL_AUDIO_Play((uint16_t*)(&RecBuf0[0]),MIC_FILTER_RESULT_LENGTH);
	}
	if (k>=COL)
	{
		for(int t = 0; t < FFT_SIZE; t++)
		{
			lcd_draw_vert_line(t, 30*((audio_data[t]+32767.0)/(65535.0)));//Output[t]*1000
		}
		k=0;
		delay_us(1000);
		EVAL_AUDIO_Play((uint16_t*)(&RecBuf0[0]),MIC_FILTER_RESULT_LENGTH);
	}
	
  
		/*if (i % 2 == 0)
			Input[i] = (RecBuf0[i] - 32767.0) / (33000.0);// 65535.0; 32767
		else
			Input[i] = 0;*/
	
		
		//__asm__("nop");
		//lcd_draw_vert_line(i, (audiodata[i] / 65535.0) * 60);
		
  //}	
	

	
		/* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
		//arm_cfft_radix4_init_f32(&S, FFT_SIZE, 0, 1);
	
		/* Process the data through the CFFT/CIFFT module */
		//arm_cfft_radix4_f32(&S, Input);
		
		/* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
		//arm_cmplx_mag_f32(Input, Output, FFT_SIZE);
		
		/* Calculates maxValue and returns corresponding value */
		//arm_max_f32(Output, FFT_SIZE, &maxValue, &maxIndex);
	//for(int k = 0; k < 32; k++)
	//{
		//if(Output[k] < 100)
		//lcd_draw_vert_line(k, (Output[k] / maxValue) * 20);
		//lcd_draw_vert_line(k, (Output[k] / maxValue));
	//}
	//delay_us(2);
	
	//EVAL_AUDIO_Play((uint16_t*)(&RecBuf0[0]),MIC_FILTER_RESULT_LENGTH*2*2);//*2
	//EVAL_AUDIO_Play((uint16_t*)(&audiodata[0]),MIC_FILTER_RESULT_LENGTH*2*2);//*2
	
	#endif
}