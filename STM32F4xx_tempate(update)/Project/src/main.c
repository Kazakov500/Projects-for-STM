#include "delay.h"
//#include "stm32f4xx.h"
//#include "stm32f4xx_conf.h"
//#include "stm32f4xx_gpio.h"

/*Definisi pin I/O*/
#define PORTLed				GPIOD
#define PORTButton		GPIOA
#define LedHijau			GPIO_Pin_12	
#define LedOrange			GPIO_Pin_13	
#define LedMerah			GPIO_Pin_14	
#define LedBiru				GPIO_Pin_15	
#define UserButton		GPIO_Pin_0	


/*deklarasi struct*/
GPIO_InitTypeDef GPIO_InitStructure;

/*deklarasi fungsi*/
void init_GPIO(void);


/*---------------------------Interrupt Systick-------------------------------*/
void SysTick_Handler()
{
	TimingDelay_Decrement();
}


void init_GPIO()
{
	/*setting PORT Led*/
	/* enable GPIOD (PORT yang terpasang led)*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	/* set PD12, PD13, PD14 and PD15 in menjadi output mode push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*setting PORT User Button*/
	/* enable GPIOD (PORT yang terpasang led)*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


int main(){
	
	/*--------------------------Inisialisasi Clock------------------------------*/
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
	
	/*---------------Inisialisasi Periperal yang akan digunakan-----------------*/
	init_GPIO();																	/* inisialisasi GPIO					*/
	while(1){
		GPIO_SetBits(PORTLed,LedHijau);

	}
}