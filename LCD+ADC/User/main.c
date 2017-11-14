#include "stm32f4xx.h"
#include "lcd.h"
#include "Delay.h"
#include "SYM.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "arm_math.h"

#define BUTTON_READ() GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

#define DATA_SAMPLES_COUNT 1024


uint16_t dataSamples_test[DATA_SAMPLES_COUNT] = 
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

uint16_t dataSamples[DATA_SAMPLES_COUNT];

int p = 0;
uint32_t DataSamples_Pos = 0;
	
int f=0;//sin control

float32_t input[DATA_SAMPLES_COUNT*2];
float32_t output[DATA_SAMPLES_COUNT];
float32_t maxValue;
uint32_t maxIndex;


int state = 0;
int samples_count = DATA_SAMPLES_COUNT;
int samples_rate = 21;

void init_audio_dac(void);
void ADC_ini(void);
void LED_ini();
void Button_ini();
void FFT();

uint8_t Draw=0;


uint8_t Button_count=0;
uint8_t Button_state=0;

int samp_rate = 1;

void SysTick_Handler(void)//1ms
{
	if (BUTTON_READ() == 1)
	{
		if (Button_count < 5)
		{
			Button_count++;
		} else 
		{
			if (Button_state == 0)
			{
				Button_state=1;
				state++;
				Clear_LCD();
				//delay_us(1000);
				//Draw = 0;
			}
		}
	} else 
	{
		if (Button_count > 0) Button_count--;
		else Button_state=0;
	}
}

void LOG(char* msg)
{
	LCD_clear_page(p);
	lcd_draw_str(msg, 0, p);
	p++;
	if (p>3) p = 0;
	delay_ms(100);
}

void draw_fft_info(int fft_s, int samp_r)
{
	
			samples_count = fft_s;
			samples_rate = samp_r;
			
			GPIOD->ODR = 0x1000;
			
			Clear_LCD();
			LCD_clear_page(0);
			lcd_draw_str("-------[FFT Info]--------", 0, 0);
			
			LCD_draw_msg_int("FFT size: ", samples_count, "", 1);
			LCD_draw_msg_int("Samples rate: ", samples_rate, "us", 2);
			LCD_draw_msg_int("Frequency: ", (37.653 * pow(samples_rate, -0.79))*1000, "Hz", 3);	
			
	
			

}

void LoadingBar()
{
	for(int i=0; i<=120; i++) 
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
		delay_ms(5);
	}
	Clear_LCD();
	p = 0;	
}

int main(void)
{
	lcd_init();
	Clear_LCD();
	
	//LOG("> Kernel Loading...");
	//LoadingBar();

	
	
	
	
	int del_led = 15;
	uint32_t i;
	
	
	//LOG("> LED Init...");
	LED_ini();
	GPIOD->ODR = 0x4000;
	//LOG("> Button Init...");
	Button_ini();	
	//LOG("> ADC Init...");
	ADC_ini();
	//LOG("> SysTick Init...");
	SysTick_Config(SystemCoreClock/1000);//1 ms
	
	Clear_LCD();
	//LOG("> Starting system...");
	
	int buff_state = 0;
	int FFT_win[] = {16, 64, 256, 1024};
	
	int pos_fft_win = 2;
	samp_rate = 4;
	while(1)
	{
		if(buff_state + 1 == state) 
		{
			Draw = 0;
			buff_state = state;
		}
		
		
		
		if (state % 2 == 0) // Start screen with info
		{
			if(Draw == 1) continue;
			else Draw = 1;
			
			draw_fft_info(FFT_win[pos_fft_win], samp_rate);	
			samp_rate *= 2;			
		} else 
		{
			GPIOD->ODR = 0x2000;
			FFT();
		}		// Screen with FFT 16
		
		if (samp_rate > 8192)
		{
			samp_rate = 1;
			pos_fft_win++;
			if(pos_fft_win > 3) pos_fft_win = 0;
		}
		
		
		
		
		/*if (state == 0) // Start screen with info
		{
			draw_fft_info(256, 20);			
		} else if (state == 1) // Screen with FFT 16
		{		
			GPIOD->ODR = 0x2000;
			FFT();
		}	else if(state == 2)
		{
			draw_fft_info(64, 21);				
		} else if (state == 3) // Screen with FFT 64
		{		
			GPIOD->ODR = 0x2000;
			FFT();
		}	else if(state == 4)
		{
			draw_fft_info(256, 21);				
		} else if (state == 5) // Screen with FFT 256
		{		
			GPIOD->ODR = 0x2000;
			FFT();
		}	else if(state == 6)
		{
			draw_fft_info(1024, 21);				
		} else if (state == 7) // Screen with FFT 1024
		{		
			GPIOD->ODR = 0x2000;
			FFT();
		}
		else if (state == 8) // Screen with FFT 1024
		{	
			if(Draw == 1) continue;
			else Draw = 1;
			GPIOD->ODR = 0x8000;
			
			Clear_LCD();
			//LCD_clear_page(0);
			lcd_draw_str("---[System Info]----[V1]", 0, 0);
			lcd_draw_str("Input amp: 0 - 3.3 V", 0, 1);
			LCD_draw_msg_int("Samples rate: ", samples_rate, "us", 2);
			LCD_draw_msg_int("Frequency: ", (1.0/samples_rate)*1000, "kHz", 3);	
		}
		else 
		{
			state = 0;
			//draw_fft_info(16, 21);	
		}*/
		
/*1
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
		*/
	}
	
	
	
}

void FFT()
{
			GPIOD->ODR |= !0x8000;
	
	
			delay_us(samples_rate);
			ADC_SoftwareStartConv(ADC1);
			if(DataSamples_Pos > samples_count)
			{
				GPIOD->ODR |= 0x8000;
				//Clear_LCD();
				
				DataSamples_Pos = 0;
				f++;
				int ii=0;
				for(int i=0; i<samples_count; i++)
				{
					//dataSamples[i] *= 10;
					input[ii] = (dataSamples[i]-2048.0)/ 2078.0;
					ii++;
					input[ii] = 0;
					ii++;
				}
				/*
				arm_cfft_instance_f32 SS;
				arm_cfft_f32(&SS, input, 0, 1);
				arm_cmplx_mag_f32(input, output, DATA_SAMPLES_COUNT);
				arm_max_f32(output, DATA_SAMPLES_COUNT, &maxValue, &maxIndex);
				*/
				
					arm_cfft_radix4_instance_f32 S; 
					arm_cfft_radix4_init_f32(&S, samples_count, 0, 1);
					arm_cfft_radix4_f32(&S, input);
					arm_cmplx_mag_f32(input, output, samples_count);
					arm_max_f32(output, samples_count, &maxValue, &maxIndex);
				
				
				int m = 40;
				if(samples_count == 16)
				{
					for(int i = 1; i < samples_count/2; i++)
						for(int ii=0; ii<15; ii++)
							lcd_draw_vert_line((i-1)*15+ii, m*30*(output[i]/maxValue));
					//lcd_draw_vert_line((samples_count/2)*15 + 1, 30);
				} else
				if(samples_count == 64)
				{
					for(int i = 1; i < samples_count/2; i++)
						for(int ii=0; ii<4; ii++)
							lcd_draw_vert_line((i-1)*4+ii, m*30*(output[i]/maxValue));
					//lcd_draw_vert_line((samples_count/2)*4 + 1, 30);
				} else
				if(samples_count>512)
				{
					for(int i = 1; i < samples_count/2; i+=4)
						lcd_draw_vert_line((i-1)/4, m*30*(output[i]/maxValue));
					//lcd_draw_vert_line((samples_count / (2*4))+1, 30);
				}
				else
				{
					for(int i = 1; i < samples_count/2; i++)
						lcd_draw_vert_line(i-1, m*30*(output[i]/maxValue));
					//lcd_draw_vert_line( (samples_count/2)+1, 30);
				}
			}
	
}

void Button_ini()
{
	GPIO_InitTypeDef GPIO_Init_Button;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_Init_Button.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init_Button.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init_Button.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init_Button.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_Button.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_Init_Button);
}


void LED_ini()
{
//LOG("> RCC init");
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;
//LOG("> GPIO init start...");

	GPIOD->MODER = 0x55000000;
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
}

void ADC_ini()
{
	GPIO_InitTypeDef GPIO_Init_ADC;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_Init_ADC.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init_ADC.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init_ADC.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init_ADC.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_ADC.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_Init_ADC);
	
	
	
	
	
	ADC_InitTypeDef ADC_Init_user; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC, ENABLE);
	
  ADC_Init_user.ADC_Resolution = ADC_Resolution_12b;
  ADC_Init_user.ADC_ScanConvMode = DISABLE;
  ADC_Init_user.ADC_ContinuousConvMode = DISABLE;
  ADC_Init_user.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_Init_user.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_Init_user.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_Init_user.ADC_NbrOfConversion = 1;
	
	ADC_Init(ADC1, &ADC_Init_user);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_56Cycles);
	
	
	
	
	
	NVIC_EnableIRQ(ADC_IRQn);
	
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	
	ADC_Cmd(ADC1, ENABLE);
	
	
}

uint16_t data_adc;
void ADC_IRQHandler()
{
	if(ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET)
	{
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
		
		data_adc = ADC_GetConversionValue(ADC1);
		dataSamples[DataSamples_Pos] = data_adc;
		
		//dataSamples[DataSamples_Pos] = (sin(DataSamples_Pos*(0.5 + (0.1 * f)) )+1)*2048;//<<<<<<<<<<<<TEST
		
		DataSamples_Pos++;
	}
}
