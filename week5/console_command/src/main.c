#include "stm32f4xx_hal.h"
#include <string.h>

/*Function prototype for delay and UART2 coznfiguration functions */
void UART1_Configuration(int type);
static void SystemClock_Config(void);
void Error_Handler(void);

UART_HandleTypeDef UART_Handler; /*Create UART_HandleTypeDef struct instance */
char Message[] = "Write anything on Serial Terminal\r\n"; /* Message to be transmitted through UART */


int main(void)
{
	HAL_Init(); /* HAL library initialization */
 // SystemClock_Config();
	UART1_Configuration(1); /* 1 for USART1 (Virtual Com) or 2 for USART2 (2 wire cable) */
	HAL_UART_Transmit(&UART_Handler, (uint8_t *)Message, strlen(Message), 10);
	while(1)
	{
		volatile HAL_StatusTypeDef tx_status, rx_status;
		uint8_t buffer[4];
    rx_status = HAL_UART_Receive(&UART_Handler, buffer, sizeof(buffer), HAL_MAX_DELAY);
		tx_status = HAL_UART_Transmit(&UART_Handler, buffer, sizeof(buffer), HAL_MAX_DELAY);
		
	}
}

void UART1_Configuration(int type)
{
	__HAL_RCC_GPIOA_CLK_ENABLE(); /* Enable clock to PORTA - UART2 pins PA2 and PA3 or UART1 PA9 and PA10 */
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE(); /* Enable clock to UART2 module */
	

	GPIO_InitTypeDef UART2_GPIO_Handler; /*Create GPIO_InitTypeDef struct instance */
  if (type == 1) {
    UART2_GPIO_Handler.Pin = GPIO_PIN_9 | GPIO_PIN_10;
     
  } else if (type == 2) {
    UART2_GPIO_Handler.Pin = GPIO_PIN_2 | GPIO_PIN_3;
  }
 
  
	UART2_GPIO_Handler.Mode = GPIO_MODE_AF_PP;
	UART2_GPIO_Handler.Pull = GPIO_PULLUP;
	UART2_GPIO_Handler.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  if (type == 1) {
    UART2_GPIO_Handler.Alternate = GPIO_AF7_USART1;
  } else if (type == 2) {
    UART2_GPIO_Handler.Alternate = GPIO_AF7_USART2;
  }
	 
	HAL_GPIO_Init(GPIOA, &UART2_GPIO_Handler);
	//UART Configuration
  if (type == 1) {
    	UART_Handler.Instance = USART1;
  } else if (type == 2){
     UART_Handler.Instance = USART2;
  }

  if (type == 1) {
    	UART_Handler.Init.BaudRate = 115200;
  } else if (type == 2) {
    UART_Handler.Init.BaudRate = 9600;
  }

	UART_Handler.Init.Mode = UART_MODE_TX_RX;
	UART_Handler.Init.WordLength = UART_WORDLENGTH_8B;
	UART_Handler.Init.Parity = UART_PARITY_NONE;
	UART_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART_Handler.Init.StopBits = UART_STOPBITS_1;
	UART_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART_Handler);	
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}


static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  if(HAL_PWREx_ActivateOverDrive() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}