#include "stm32f4xx.h"
#include "stdbool.h"
#include "string.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

#define	RX_BUFF_SIZE	256






#define	RED_ON()				GPIO_SetBits(GPIOD, GPIO_Pin_14)
#define	RED_OFF()				GPIO_ResetBits(GPIOD, GPIO_Pin_14)
#define	BLUE_ON()				GPIO_SetBits(GPIOD, GPIO_Pin_15)
#define	BLUE_OFF()			GPIO_ResetBits(GPIOD, GPIO_Pin_15)
#define	GREEN_ON()			GPIO_SetBits(GPIOD, GPIO_Pin_12)
#define	GREEN_OFF()			GPIO_ResetBits(GPIOD, GPIO_Pin_12)
#define	YELLOW_ON()			GPIO_SetBits(GPIOD, GPIO_Pin_13)
#define	YELLOW_OFF()		GPIO_ResetBits(GPIOD, GPIO_Pin_13)

#define TOGGLE_RED()		GPIO_ToggleBits(GPIOD, GPIO_Pin_14)

char RxBuffer[RX_BUFF_SIZE];					//Ѕуфер приЄма USART
bool ComReceived;								//‘лаг приЄма команды
char Ang_str[4];
int Ang = 0;




uint8_t Recieve_buf[256];
uint8_t Recieve_W=0, Recieve_R=0, Recieve_C=0;


/*******************************************************************/
// ќбъ€вл€ем переменные
GPIO_InitTypeDef gpio;
USART_InitTypeDef usart;
// ѕусть нам надо передать 8 байт, создадим массив дл€ данных
uint8_t sendData[8];
uint8_t bytesToSend = 8;
// —четчик отправленных байт
uint8_t sendDataCounter = 0;
 
 
uint16_t delay_count=0;



uint32_t PWM_Counter=0;


// 1000 - SEC
// 1 ---- MS
uint32_t PWM_A=190000, PWM_B=200000;
uint32_t MStoS = 1000;




/*******************************************************************/
void Systick_PWM(void)
{
	if (PWM_Counter >= PWM_B)
	{
		PWM_Counter = 0;
		GREEN_ON();
	} else if (PWM_Counter == PWM_A)
	{
		PWM_Counter++;
		GREEN_OFF();
	} else 
	{
		PWM_Counter++;
	}
}
void SysTick_Handler(void)//0,01ms
{
	if (delay_count > 0)
	{
		delay_count--;
	}
	
	Systick_PWM();
}
void LEDs_ini(void)//TODO REWRITE
{
	//GPIO_InitTypeDef GPIO_Init_LED;
	
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOD;
	
	/*
	GPIO_Init_LED.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init_LED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init_LED.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init_LED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	
	
	GPIO_Init(GPIOD, &GPIO_Init_LED);
	*/
	GPIOD->MODER &= GPIO_MODER_MODER12;
	GPIOD->OSPEEDR &= GPIO_OSPEEDER_OSPEEDR12;
	GPIOD->OTYPER &= GPIO_OTYPER_OT_12;
	GPIOD->PUPDR &= GPIO_PUPDR_PUPDR12;
	
	GPIOD->MODER |= GPIO_MODER_MODER12_1;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12_1;
	//GPIOD->OTYPER |= GPIO_OTYPER_OT_12;
	//GPIOD->PUPDR |= GPIO_PUPDR_PUPDR12;
	
	
	
	GPIOD->MODER &= GPIO_MODER_MODER13;
	GPIOD->OSPEEDR &= GPIO_OSPEEDER_OSPEEDR13;
	GPIOD->OTYPER &= GPIO_OTYPER_OT_13;
	GPIOD->PUPDR &= GPIO_PUPDR_PUPDR13;
	
	GPIOD->MODER |= GPIO_MODER_MODER13_1;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13_1;

	GPIOD->MODER &= GPIO_MODER_MODER14;
	GPIOD->OSPEEDR &= GPIO_OSPEEDER_OSPEEDR14;
	GPIOD->OTYPER &= GPIO_OTYPER_OT_14;
	GPIOD->PUPDR &= GPIO_PUPDR_PUPDR14;
	
	GPIOD->MODER |= GPIO_MODER_MODER14_1;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14_1;
	
	GPIOD->MODER &= GPIO_MODER_MODER15;
	GPIOD->OSPEEDR &= GPIO_OSPEEDER_OSPEEDR15;
	GPIOD->OTYPER &= GPIO_OTYPER_OT_15;
	GPIOD->PUPDR &= GPIO_PUPDR_PUPDR15;
	
	GPIOD->MODER |= GPIO_MODER_MODER15_1;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15_1;	
	
	
	/*
	//L->H
	GPIOD->CRH &= ~(GPIO_CRL_MODE12 | GPIO_CRH_CNF12);
	GPIOD->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	GPIOD->CRH &= ~(GPIO_CRH_MODE14 | GPIO_CRH_CNF14);
	GPIOD->CRH &= ~(GPIO_CRH_MODE15 | GPIO_CRH_CNF15);
	
	GPIOD->CRH |= GPIO_CRH_MODE12_1;
	GPIOD->CRH |= GPIO_CRH_MODE13_1;
	GPIOD->CRH |= GPIO_CRH_MODE14_1;
	GPIOD->CRH |= GPIO_CRH_MODE15_1;
	*/
	
	
	
}
void USART1_ini(void)//TODO REWRITE
{
	GPIO_InitTypeDef GPIO_Init_USART;
	USART_InitTypeDef USART_InitUser;
	
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOA;
	
	GPIO_Init_USART.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Init_USART.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init_USART.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init_USART.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_USART.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &GPIO_Init_USART);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitUser.USART_BaudRate=19200;//9600;
	USART_InitUser.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitUser.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitUser.USART_Parity=USART_Parity_No;
	USART_InitUser.USART_StopBits=USART_StopBits_1;
	USART_InitUser.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1, &USART_InitUser);
	
	NVIC_EnableIRQ(USART1_IRQn);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
	//USART1->CR1 |= USART_CR1_UE;
}
/*******************************************************************/
// ќбработчик прерывани€
void USART1_IRQHandler(void)//TODO REWRITE
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		if (ComReceived == false)
		{
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			
			Recieve_buf[Recieve_W] = USART_ReceiveData(USART1);
			
			if (Recieve_W ==10)
				if ((Recieve_buf[9] == 0x0D) && (Recieve_buf[10] == 0x0A)) 
					ComReceived = true;
			
			Recieve_W++;
		}
	}
}


void delay_ms(uint16_t delay_temp)
{
	delay_count = delay_temp;
	while(delay_count){}
}


int main(void)
{
	LEDs_ini();
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/100000);//0,01 ms
	/*
	SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         // set reload register 
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); // set Priority for Systick Interrupt 
  SysTick->VAL   = 0UL;                                             // Load the SysTick Counter Value 
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         // Enable SysTick IRQ and SysTick Timer 
	*/
	
	
	USART1_ini();
	
	
	
	while(1)
	{
		//char b;
		//if (Recieve_W >= 11)
		if (ComReceived == true)
		{			
			BLUE_ON();
			//ANGLE XXX
			//0123456789
			memcpy( Ang_str, &Recieve_buf[6], 3 );
			Ang_str[3] = '\0';
			Ang = atoi(Ang_str);
			
			uint32_t Cycle = (uint32_t)(100 + (((200-100) / 180.0) * Ang));//(80 + ( ((336.0-80.0) / 180.0 ) * Ang ));
			
			PWM_A = Cycle * MStoS;//convert ms -> s
			PWM_Counter = 0;

			Recieve_W = 0;
			BLUE_OFF();
			ComReceived = false;
		}
	}
}





//UART2
//STM - NODE_MCU
//GND - GND
//TX  - PA3
//RX  - PA2

//UART1
//STM - NODE_MCU
//GND - GND
//TX  - PA10
//RX  - PA9
