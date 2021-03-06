#include "main.h"
#include <stdio.h>
// #include <stdlib.h>
#include "HUB75ELib.h"
#include "stm32f4xx_hal.h"

static void SystemClock_Config(void);
static void MX_NVIC_Init(void);
void Error_Handler(void);

#include "dwt_delay.h"

#include "matrix_util.h"
#include "matrix_util.h"
#include <string.h>
#include <stdio.h>




#include <stdint.h>

extern unsigned char myBitmap2[PIXELS_COUNT_IN_BYTES];
int main(void) {
 


  

    
    HAL_Init();
    SystemClock_Config();
    DWT_Init();
    // MX_NVIC_Init();

    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
    // MX_NVIC_Init();

    HUB75E_Init();
    HUB75E_setDisplayBrightness(BrightnessLevel3);
    // HUB75E_setDisplayBuffer(myBitmap);
    // HUB75E_setDisplayBrightness(BrightnessLevel3);
    // HUB75E_setDisplayColor(Red);
    // HUB75E_displayBufferPixels(myBitmap);

  animateGo(myBitmap2, PIXELS_COUNT_IN_BYTES);
  //  animateSmiley(myBitmap2, PIXELS_COUNT_IN_BYTES);
  //   animateX(myBitmap2, PIXELS_COUNT_IN_BYTES);
    memset(myBitmap2, 0, sizeof(myBitmap2));
    clearBuffer();
    HAL_Delay(250);
  
    while(1) {
      //  playSequenceLoop();
        // HUB75E_displayBufferPixels(myBitmap);
        // HUB75E_setDisplayColor(Red);
        // HAL_Delay(500);
        // BSP_LED_Toggle(LED3);
    }

}



void SysTick_Handler(void) {
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
  
  /* Activate the Over-Drive mode */
  HAL_PWREx_EnableOverDrive();

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
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





