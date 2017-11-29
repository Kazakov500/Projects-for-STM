/**
  ******************************************************************************
  * @file    main.c
  * @author  Vladimir Leonidov
  * @version V1.0.0
  * @date    06.12.2015
  * @brief   Лабораторная работа №3 - "USART, DMA"
  *			 Отладочная плата: STM32F10C-EVAL
  *
  *			 Реализованы отправка и приём данных по USART.
  *			 Возможны два варианта передачи данных:
  *			   - через процессор (#define USE_DMA в файле main.h должен быть закомментирован)
  *			   - через модуль DMA (#define USE_DMA в файле main.h должен быть раскомментирован)
  *
  ******************************************************************************
  */

#include "main.h"
#include "string.h"


/* Размеры буферов приёма и передачи */
#define	RX_BUFF_SIZE	256
#define TX_BUFF_SIZE	256

/* Управление светодиодами */
#define	LED1_ON()	GPIOD->BSRR |= GPIO_BSRR_BS7
#define	LED1_OFF()	GPIOD->BSRR |= GPIO_BSRR_BR7



char RxBuffer[RX_BUFF_SIZE];					//Буфер приёма USART
bool ComReceived;								//Флаг приёма команды

char Ang_str[4];
int Ang = 0;

/**
  * @brief  Подпрограмма обработчика прерывания
  *			по переполнению тайемера TIM6
  * @param  None
  * @retval None
  */
uint32_t PWM_A=190, PWM_B=200;
uint32_t MStoS = 1000;//1 -> for engine || 1000 -> fo diod and eye
uint32_t PWM_Counter;
int num = 0;

void TIM2_IRQHandler(void)
{
	TIM2->SR &= ~TIM_SR_UIF;			//Сброс флага переполнения
	
	if (PWM_Counter >= PWM_B*MStoS)
	{
		PWM_Counter = 0;
		LED1_ON();
	} else if (PWM_Counter == PWM_A*MStoS)
	{
		PWM_Counter++;
		LED1_OFF();
	} else 
	{
		PWM_Counter++;
	}
	
}

/**
  * @brief  Инициализация таймера TIM6
  * @param  None
  * @retval None
  */
void initTIM2(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	//Включить тактирование TIM6
	
	//Частота APB1 для таймеров = APB1Clk * 2 = 36МГц * 2 = 72МГц
	TIM2->PSC = 24000-1;				//Предделитель частоты (72МГц/36000 = 2кГц)
	TIM2->ARR = 10-1;					//Модуль счёта таймера (2кГц/2000 = 1с)  --> 10 => 0.01c
	TIM2->DIER |= TIM_DIER_UIE;			//Разрешить прерывание по переполнению таймера
	TIM2->CR1 |= TIM_CR1_CEN;			//Включить таймер
	
	NVIC_EnableIRQ(TIM2_IRQn);			//Рарзрешить прерывание от TIM6
	NVIC_SetPriority(TIM2_IRQn, 1);		//Выставляем приоритет
}



/**
  * @brief  Обработчик прерывания от USART2
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	char tmp;
	int t;
	if ((USART1->SR & USART_SR_RXNE)!=0)		//Прерывание по приёму данных?
	{
		tmp=USART1->DR;
		if (tmp==0x0D)							//Если это символ конца строки
		{
			ComReceived=true;					//- выставляем флаг приёма строки
			return;								//- и выходим
		}
		RxBuffer[strlen(RxBuffer)] = tmp;		//Иначе добавляем его в буфер
	}
}

/**
  * @brief  Инициализация модуля USART1
  * @param  None
  * @retval None
  */
void initUART1(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;			//включить тактирование GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;			//включить тактирование альтернативных ф-ций портов
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;		//включить тактирование UART1
	
	GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);		//PD5 на выход
	GPIOA->CRH |= (GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1);
	
	GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);		//PD6 - вход
	GPIOA->CRH |= GPIO_CRH_CNF10_0;
	
	
	/*****************************************
	Скорость передачи данных - 19200
	Частота шины APB1 - 24МГц
	
	1. USARTDIV = 24000000/(16*19200)=78 -> 4E
	2. 16*0.5 = 8
	3. Итого 0x4E8
	*****************************************/
	//USART1->BRR = 0x138;
	USART1->BRR = 0x4E8;
	
	USART1->CR2 = 0;
	USART1->CR1 = 0;
	USART1->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	USART1->CR1 |= USART_CR1_RXNEIE;				//разрешить прерывание по приему байта данных
	
	NVIC_EnableIRQ(USART1_IRQn);					//разрешить прерывание от модуля USART2
}

/**
  * @brief  Обработчик команд
  * @param  None
  * @retval None
  */
void ExecuteCommand(void)
{
	uint32_t Cycle;

	//ANGLE XXX
	//0123456789
	memcpy( Ang_str, &RxBuffer[6], 3 );
	Ang_str[3] = '\0';
	Ang = atoi(Ang_str);
	
	Cycle = (uint32_t)(100 + (((200-100) / 180.0) * Ang));//(80 + ( ((336.0-80.0) / 180.0 ) * Ang ));
	
	PWM_A = Cycle * MStoS;//convert ms -> s
	PWM_Counter = 0;

	memset(RxBuffer,0,RX_BUFF_SIZE);				//Очищаем буфер приёма
	ComReceived = false;							//Сбрасываем флаг приёма строки	
}

void init_OutPin()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;	//включить тактирование GPIOD
	GPIOC->CRH = 0;
	GPIOC->CRH |= GPIO_CRH_MODE8_1;		//LD1, выход 2МГц
}

/**
  * @brief  Основная программа
  * @param  None
  * @retval None
  */
int main(void)
{
	initUART1();
	initTIM2();
	init_OutPin();
	
	/*Основной цикл*/
	while(true)
	{
		if (ComReceived)			//Ждём приема строки
			ExecuteCommand();
	}
	
}

/**
  * @brief  Подпрограмма задержки
  * @param  takts - Кол-во тактов
  * @retval None
  */
void delay(uint32_t takts)
{
	uint32_t i;
	for (i = 0; i < takts; i++) {};
}
