
#include "u8g_arm.h"

uint8_t control = 0;

/* set TIM5 to run at DELAY_TIM_FREQUENCY */
/*
void delay_init( void )
{
  // Enable timer clock  - use TIMER5 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


  // Time base configuration 
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = (SYS_CLK / DELAY_TIM_FREQUENCY) - 1;
  TIM_TimeBaseStructure.TIM_Period = UINT16_MAX;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

 // Enable counter 
  TIM_Cmd(TIM2, ENABLE);
}
*/

/*
  Delay by the provided number of micro seconds.
  Limitation: "us" * System-Freq in MHz must now overflow in 32 bit.
  Values between 0 and 1.000.000 (1 second) are ok.
*/
void delay_micro_seconds(uint32_t us)
{
	/*
	TIM_SetCounter(TIM2, 0);
	  // use 16 bit count wrap around 
	  while((uint16_t)(TIM2->CNT) <= us);
	*/
}

/*========================================================================*/
/*
  The following delay procedures must be implemented for u8glib

  void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
  void u8g_MicroDelay(void)		Delay be one microsecond
  void u8g_10MicroDelay(void)	Delay by 10 microseconds

*/

void u8g_Delay(uint16_t val)
{
    
  delay_micro_seconds(1000UL*(uint32_t)val);
}

void u8g_MicroDelay(void)
{
  delay_micro_seconds(1);
}

void u8g_10MicroDelay(void)
{
  delay_micro_seconds(10);
}

//*************************************************************************************
//*************************************************************************************
uint8_t u8g_com_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{ 
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      //STOP THE DEVICE
      break;
 
    case U8G_COM_MSG_INIT:
      //INIT HARDWARE INTERFACES, TIMERS, GPIOS...		
			Delay_mss();  //Ждем, пока контроллер LCD включится.
			SystemInit(); //Инициализация настроек кварца
			init_SPI1() ;    //Инициализация SPI1 (stm32f4)		
      break;
 
    case U8G_COM_MSG_ADDRESS:  
      //SWITCH FROM DATA TO COMMAND MODE (arg_val == 0 for command mode)
		  //управляющий байт переключает режим на устройстве и устанавливается здесь 		
		  // define cmd (arg_val = 0) or data mode (arg_val = 1) 
		  // cmd - передача команды ,data mode - режим передачи данных
      if (arg_val == 0)
      {
    	  control = 0xF8;
      }
      else
      {
    	  control = 0xFA;
      }
      u8g_10MicroDelay();		 
     break;
 
    case U8G_COM_MSG_RESET:
      //TOGGLE THE RESET PIN ON THE DISPLAY BY THE VALUE IN arg_val
      break;
 
    case U8G_COM_MSG_WRITE_BYTE:
      //WRITE BYTE TO DEVICE
		  //ЗАПИСЬ байта НА УСТРОЙСТВО		
		  LCD_OUT (arg_val, control);
		  u8g_MicroDelay();		
      break;
 
    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P:
      //WRITE A SEQUENCE OF BYTES TO THE DEVICE
		  //ЗАПИСЬ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТОВ НА УСТРОЙСТВО
      {
			  register uint8_t *ptr = arg_ptr;
	      CS_ON();                               //Передача начата.
        SPI_Out_LCD12864_Ap (control);				
				while( arg_val > 0 )
				{					
          SPI_Out_LCD12864_Ap( *ptr & 0xF0);   //Старшая половина и 4 "0".
          SPI_Out_LCD12864_Ap( *ptr << 4);     //Младшая и 4 "0".
          *ptr++;
				  arg_val--;
				}
				CS_OFF(); 			
			}		
      break;
  }
  return 1;
}
//****************************************************************************
//****************************************************************************
void Delay_mss (void)
{
	uint32_t Sl;
	
	for (Sl = 0; Sl < 0x5FFFFF; Sl++)
	{
	}
}
//****************************************************************************
//****************************************************************************
void init_SPI1(void)
{
	 /* configure pins used by SPI1
	 * PA5 = SCK
	 * PA6 = MISO
	 * PA7 = MOSI
	 */
	
  GPIO_InitTypeDef SPI_Pin_Init;	                       //Настройка пинов SPI1.
	GPIO_InitTypeDef SPI_Pin_CS_Init;                      //CS.
	SPI_InitTypeDef  SPI1_Init_LCD12864;                   //Настройка SPI1.
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //Вкл. тактирование порта 
	                                                       //с пинами SPI1. 
  SPI_Pin_Init.GPIO_Mode  = GPIO_Mode_AF;                //Настраиваем SPI1_SCK и
	SPI_Pin_Init.GPIO_OType = GPIO_OType_PP;               //SPI1_MOSI
	SPI_Pin_Init.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_7;
	SPI_Pin_Init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	SPI_Pin_Init.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &SPI_Pin_Init);                      
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);//Присваиваем вльт. функцию.
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //Настройка пина CS.
  SPI_Pin_CS_Init.GPIO_Mode  = GPIO_Mode_OUT;
	SPI_Pin_CS_Init.GPIO_OType = GPIO_OType_PP;
	SPI_Pin_CS_Init.GPIO_Pin   = GPIO_Pin_6;
	SPI_Pin_CS_Init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	SPI_Pin_CS_Init.GPIO_Speed = GPIO_Speed_2MHz;
	
  GPIO_Init(GPIOA, &SPI_Pin_CS_Init);	
	
	CS_OFF();                                               //CS=0.

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//Вкл. тактирования SPI1.
	
	SPI1_Init_LCD12864.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;//Скорость.
	SPI1_Init_LCD12864.SPI_CPHA = SPI_CPHA_2Edge;//Со 2-го фронта.
	SPI1_Init_LCD12864.SPI_CPOL = SPI_CPOL_Low; //В режиме ожидания SCK - 0.
	SPI1_Init_LCD12864.SPI_CRCPolynomial = 7;//Фигня какая-то.
	SPI1_Init_LCD12864.SPI_DataSize = SPI_DataSize_8b;//Можно и 16!
	SPI1_Init_LCD12864.SPI_Direction = SPI_Direction_1Line_Tx;//Только TX = MOSI = выход.
	SPI1_Init_LCD12864.SPI_FirstBit = SPI_FirstBit_MSB;//Со старшего бита.
	SPI1_Init_LCD12864.SPI_Mode = SPI_Mode_Master;//Мастер.
	SPI1_Init_LCD12864.SPI_NSS = SPI_NSS_Soft;//Програмный NSS (в железе отключено).	
	SPI_Init (SPI1, &SPI1_Init_LCD12864);
  SPI_Cmd	 (SPI1, ENABLE);                   //Запуск SPI1.
}
//*********************************************************************************
//*********************************************************************************
void SPI_Out_LCD12864_Ap (uint8_t Data_LCD12864) 
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) {};
	SPI_I2S_SendData(SPI1, Data_LCD12864);
}
//**********************************************************************************
//**********************************************************************************
void LCD_OUT (uint8_t Data_LCD, uint8_t Np_LCD)
{
  CS_ON();                                //Передача начата.
	SPI_Out_LCD12864_Ap (Np_LCD);           //Передача данных или комманды.
  SPI_Out_LCD12864_Ap(Data_LCD & 0xF0);   //Старшая половина и 4 "0".
  SPI_Out_LCD12864_Ap(Data_LCD << 4);     //Младшая и 4 "0".
  CS_OFF();                               //Передача окончина. 
}