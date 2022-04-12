#include "main.h"



void SystemClock_Config(void);
void Error_Handler(void);
void EXTI0_Callback(uint16_t GPIO_Pin);



int main(void) {
 
    SystemClock_Config();
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
    BSP_LCD_SelectLayer(0);
    BSP_LCD_DisplayOn();

    //BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_BLUE);


    BSP_LCD_DisplayStringAt(20, 20, (uint8_t *)"Hello World!!", LEFT_MODE);
    
    while (1)
    {
      // no code here is as the interrupt turns the led on/off 
    // BSP_LCD_FillRect(20,20, 5, 5);      
    }
    
    return 0;
}



void SysTick_Handler(void) {
  HAL_IncTick();
}

// copied from Cube IDE Generated code
void SystemClock_Config(void)
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
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


// copied from Cube IDE Generated code
void EXTI0_IRQHandler(void)
{
    
}


// programmer provided callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

}

// copied from Cube IDE Generated code
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
