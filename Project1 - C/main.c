#include "main.h"

char RxBuffer[RX_BUFF_SIZE];					//Буфер приёма USART
char TxBuffer[TX_BUFF_SIZE];					//Буфер передачи USART
bool ComReceived;								//Флаг приёма команды
char Ang_str[4];
int Ang = 0;
/**
  * @brief  Обработчик прерывания от USART2
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
	char tmp;

	if ((USART2->SR & USART_SR_RXNE)!=0)		//Прерывание по приёму данных?
	{
		tmp=USART2->DR;
		if (tmp==0x0D)							//Если это символ конца строки
		{
			ComReceived=true;					//- выставляем флаг приёма строки
			return;								//- и выходим
		}
		RxBuffer[strlen(RxBuffer)] = tmp;		//Иначе добавляем его в буфер
	}
}

/**
  * @brief  Инициализация портов ввода-вывода
  * @param  None
  * @retval None
  */
void initPorts(void)
{
	//Включить тактирование порта GPIOD
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	
	// Сбрасываем биты конфигурации портов...
	GPIOD->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
	GPIOD->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
	GPIOD->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
	GPIOD->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	
	//...и выставляем так, как нам нужно
	GPIOD->CRL |= GPIO_CRL_MODE4_1;		//LD4, выход 2МГц
	GPIOD->CRL |= GPIO_CRL_MODE3_1;		//LD3, выход 2МГц
	GPIOD->CRH |= GPIO_CRH_MODE13_1;	//LD2, выход 2МГц
	GPIOD->CRL |= GPIO_CRL_MODE7_1;		//LD1, выход 2МГц	
	
	
	RCC->APB1ENR	|= RCC_APB1ENR_TIM2EN;	

	GPIOA->CRL  |=GPIO_CRL_MODE2;
	GPIOA->CRL	&=~GPIO_CRL_CNF2_0;
	GPIOA->CRL	|=GPIO_CRL_CNF2_1;

	GPIOA->CRL  |=GPIO_CRL_MODE3;
	GPIOA->CRL	&=~GPIO_CRL_CNF3_0;
	GPIOA->CRL	|=GPIO_CRL_CNF3_1;
}

/**
  * @brief  Инициализация модуля USART2
  * @param  None
  * @retval None
  */
void initUART2(void)
{
	/*UART2 Remapped - PD5-TX, PD6-RX	*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;			//включить тактирование GPIOD
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;			//включить тактирование альтернативных ф-ций портов
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;		//включить тактирование UART2
	
	GPIOD->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);		//PD5 на выход
	GPIOD->CRL |= (GPIO_CRL_MODE5_1 | GPIO_CRL_CNF5_1);
	
	GPIOD->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);		//PD6 - вход
	GPIOD->CRL |= GPIO_CRL_CNF6_0;
	
	AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;		//REMAP USART2 -> PD5,PD6
	
	/*****************************************
	Скорость передачи данных - 115200
	Частота шины APB1 - 36МГц
	
	1. USARTDIV = 36000000/(16*115200) = 19.5
	2. 19 = 0x13
	3. 16*0.5 = 8
	4. Итого 0x138
	*****************************************/
	USART2->BRR = 0x138;
	
	USART2->CR2 = 0;
	USART2->CR1 = 0;
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	USART2->CR1 |= USART_CR1_RXNEIE;				//разрешить прерывание по приему байта данных
	
	NVIC_EnableIRQ(USART2_IRQn);					//разрешить прерывание от модуля USART2
}

/**
  * @brief  Основная программа
  * @param  None
  * @retval None
  */
int main(void)
{
	/*Инициализации всякие*/
	initPorts();
	initUART2();
	/*initDMA();*/
	
	Timer_Init();

	/*Основной цикл*/
	while(true)
	{
		
		if (ComReceived)			//Ждём приема строки
		{
			memmove(TxBuffer,RxBuffer,RX_BUFF_SIZE);		//RxBuffer -> TxBuffer
			//ANGLE XXX
			//0123456789
			memcpy( Ang_str, &TxBuffer[6], 3 );
			Ang_str[3] = '\0';
			//Ang = (int)strtol(Ang_str, (char **)NULL, 10);
			Ang = atoi(Ang_str);
			*DutyCycle[0] = (int)(80 + ( ((336.0-80.0) / 180.0 ) * Ang ));
			
		}
		
	}
}

void Timer_Init()
{
	TIM2->PSC = 0x00A7;
	TIM2->ARR = 0x0FFF;
	TIM2->CCR3 = 0x050;
	//TIM2->CCR4 = 0x0150;
	
	TIM2->CCMR2 |= TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3M_2;
	TIM2->CCER  |= TIM_CCER_CC3E;

	//TIM2->CCMR2 |= TIM_CCMR2_OC4M_1|TIM_CCMR2_OC4M_2;
	//TIM2->CCER  |= TIM_CCER_CC4E;

	TIM2->CR1 |= TIM_CR1_CEN;
}