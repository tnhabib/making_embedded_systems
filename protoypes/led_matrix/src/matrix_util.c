#include "matrix_util.h"
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "HUB75ELib.h"
#include "stm32f429i_discovery.h"

#define TIMx                           TIM3
#define TIMx_CLK_ENABLE                __HAL_RCC_TIM3_CLK_ENABLE
#define TIMx_IRQn                      TIM3_IRQn
#define TIMx_IRQHandler                TIM3_IRQHandler

static unsigned char myBitmap[PIXELS_COUNT_IN_BYTES];

volatile int timerCount = 0;
TIM_HandleTypeDef    TimHandle;
uint16_t uwPrescalerValue = 0; 
volatile static int graphicDirection = 0;

#define TIMx                           TIM2
#define TIMx_CLK_ENABLE                __HAL_RCC_TIM2_CLK_ENABLE
#define TIMx_IRQn                      TIM2_IRQn
#define TIMx_IRQHandler                TIM2_IRQHandler




typedef void(*graphic_buffer_update_function)(unsigned char*bitmap, int bitmap_size);

graphic_buffer_update_function updateFunctions[4] = {
    fillLeftRectBuffer,
    fillTopRectBuffer, 
    fillRightRectBuffer,
    fillBottomRectBuffer
};

void clearBuffer(unsigned char * bitmap, int bitmap_size) {
    HUB75E_setDisplayBuffer(bitmap);
    memset(bitmap,0, bitmap_size);
    HUB75E_setDisplayColor(Black);
    HUB75E_displayBufferPixels(myBitmap);
    // HAL_Delay(250);
   
}
void fillLeftRectBuffer(unsigned char*bitmap, int bitmap_size) {
//    clearBuffer(bitmap, bitmap_size);
   HUB75E_setDisplayColor(Red);
   memset(bitmap,0, bitmap_size);
   for (int ii=0; ii < 2; ii++){
       for (int jj=0; jj < 32; jj++) {
           bitmap[(jj*8)+ii] = 255;
       }     
   } 
}

void fillRightRectBuffer(unsigned char*bitmap, int bitmap_size) {
    //  clearBuffer(bitmap, bitmap_size);
     HUB75E_setDisplayColor(Green);
     memset(bitmap,0, bitmap_size);

     for (int ii=6; ii < 8; ii++){
       for (int jj=0; jj < 32; jj++) {
           bitmap[(jj*8)+ii] = 255;
       }     
   }
}

void fillTopRectBuffer(unsigned char*bitmap, int bitmap_size) {
    //  clearBuffer(bitmap, bitmap_size);
     HUB75E_setDisplayColor(Blue);
     memset(bitmap,0, bitmap_size);

     for (int ii=0; ii < 8; ii++){
       for (int jj=0; jj < 8; jj++) {
           bitmap[(jj*8)+ii] = 255;
       }     
   }
}

void fillBottomRectBuffer(unsigned char*bitmap, int bitmap_size) {
    // clearBuffer(bitmap, bitmap_size);
    HUB75E_setDisplayColor(Yellow);
     memset(bitmap,0, bitmap_size);

     for (int ii=0; ii < 8; ii++){
       for (int jj=24; jj < 32; jj++) {
           bitmap[(jj*8)+ii] = 255;
       }     
   }
}


void playSequenceLoop() {
  
    uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
    TimHandle.Instance = TIMx;
    TimHandle.Init.Period = 10000 - 1;
    TimHandle.Init.Prescaler = uwPrescalerValue;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&TimHandle);
 
    if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
    {
            /* Starting Error */
            Error_Handler();
    }



    while (1) {
        HUB75E_setDisplayBuffer(myBitmap);
        updateFunctions[graphicDirection %4](myBitmap, PIXELS_COUNT_IN_BYTES);
        HUB75E_setDisplayBrightness(BrightnessLevel3);
        HUB75E_displayBufferPixels(myBitmap);
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  TIMx_CLK_ENABLE();

  /*##-2- Configure the NVIC for TIMx ########################################*/
  /* Set the TIMx priority */
  HAL_NVIC_SetPriority(TIMx_IRQn, 0, 1);
  
  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIMx_IRQn);

}

void TIMx_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
  
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
    BSP_LED_Toggle(LED3);
    graphicDirection++;
    HUB75E_clearDisplayBuffer();
    //clearBuffer(myBitmap, PIXELS_COUNT_IN_BYTES);
    //DWT_Delay(500);

}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
 BSP_LED_On(LED4);
  while(1)
  {
  }
}