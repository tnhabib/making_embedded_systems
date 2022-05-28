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
unsigned char myBitmap2[PIXELS_COUNT_IN_BYTES];

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


void animateX(unsigned char*bitmap, int bitmap_size) {
    HUB75E_clearDisplayBuffer();
    clearBuffer(bitmap, bitmap_size);
    HUB75E_setDisplayColor(Red);
    int pixel_list[] = {
        0,  192,
        7, 3,
        248, 192,
        255, 3,

        8, 48,
        15, 12,
        240, 48,
        247, 12,

        16, 12,
        23, 48,
        232, 12,
        239, 48,

        24, 3,
        31, 192,
        224, 3,
        231, 192,

        33, 192,
        38, 3,
        217,192,
        222, 3,

        41, 48,
        46, 12,
        209, 48,
        214, 12,

        49, 12,
        54, 48,
        201, 12,
        206, 48,

        57, 3,
        62, 192,
        193, 3,
        198, 192,

        66, 192,
        69, 3,
        186, 192,
        189, 3,

        74, 48,
        77, 12,
        178, 48,
        181, 12,

        82, 12,
        85, 48,
        170, 12,
        173, 48,

        90, 3,
        93, 192,
        162, 3,
        165, 192,

        99, 192,
        100, 3,
        155, 192,
        156, 3,

        107, 48,
        108, 12,
        147, 48,
        148, 12,

        115, 12,
        116, 48,
        139, 12,
        140, 48,

        123, 3,
        124, 192,
        131, 3,
        132, 192
    };
    
  
    for (int ii=0; ii < sizeof(pixel_list) / sizeof(int); ii+=2) {
        int idx = pixel_list[ii];
        int pixels = pixel_list[ii+1];
        bitmap[idx] = pixels;
        for (int jj=0; jj < 10; jj++) {
            HUB75E_setDisplayBuffer(bitmap);
            HUB75E_displayBufferPixels(bitmap);
        }
        
    }

    for (int kk=0; kk < 1500; kk++) {
         HUB75E_setDisplayBuffer(bitmap);
         HUB75E_displayBufferPixels(bitmap);
    }
}

void animateO(unsigned char*bitmap, int bitmap_size) {

   
    HUB75E_setDisplayColor(Yellow);
    while (1)
    {
        HUB75E_setDisplayBuffer(bitmap);
        HUB75E_displayBufferPixels(bitmap);

    }
}

void clearBuffer(unsigned char * bitmap, int bitmap_size) {
    HUB75E_setDisplayBuffer(bitmap);
    memset(bitmap,0, bitmap_size);
    HUB75E_setDisplayColor(Black);
    HUB75E_displayBufferPixels(bitmap);
    HUB75E_clearDisplayBuffer();
    // HAL_Delay(250);
   
}
void fillLeftRectBuffer(unsigned char*bitmap, int bitmap_size) {

   HUB75E_setDisplayColor(Red);
   memset(bitmap,0, bitmap_size);
   for (int ii=0; ii < 2; ii++){
       for (int jj=0; jj < 32; jj++) {
           bitmap[(jj*8)+ii] = 255;
       }     
   } 
}

void fillRightRectBuffer(unsigned char*bitmap, int bitmap_size) {

     HUB75E_setDisplayColor(Green);
     memset(bitmap,0, bitmap_size);

     for (int ii=6; ii < 8; ii++){
       for (int jj=0; jj < 32; jj++) {
           bitmap[(jj*8)+ii] = 255;
       }     
   }
}

void fillTopRectBuffer(unsigned char*bitmap, int bitmap_size) {

     HUB75E_setDisplayColor(Blue);
     memset(bitmap,0, bitmap_size);

     for (int ii=0; ii < 8; ii++){
       for (int jj=0; jj < 8; jj++) {
           bitmap[(jj*8)+ii] = 255;
       }     
   }
}

void fillBottomRectBuffer(unsigned char*bitmap, int bitmap_size) {

    HUB75E_setDisplayColor(Yellow);
    memset(bitmap,0, bitmap_size);

     for (int ii=0; ii < 8; ii++){
       for (int jj=24; jj < 32; jj++) {
           bitmap[(jj*8)+ii] = 255;
       }     
   }
}


void drawGrapic(int direction) {
    // converting to zero based since it comes in 1-4 to match prevision LCD drawing code
    int graphicsDir = direction--;
    timerCount = 0;
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

    while (timerCount < 2) {
        HUB75E_setDisplayBuffer(myBitmap);
        updateFunctions[graphicDirection %4](myBitmap, PIXELS_COUNT_IN_BYTES);
        HUB75E_setDisplayBrightness(BrightnessLevel3);
        HUB75E_displayBufferPixels(myBitmap);
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
    timerCount++;
    if (timerCount > 1) {
        BSP_LED_Toggle(LED3);
        graphicDirection++;
        HUB75E_clearDisplayBuffer();
    }

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