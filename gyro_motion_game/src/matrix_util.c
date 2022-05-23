#include "matrix_util.h"
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "HUB75ELib.h"
#include "stm32f429i_discovery.h"
#include "main.h"

static unsigned char myBitmap[PIXELS_COUNT_IN_BYTES];


TIM_HandleTypeDef    TimHandle;
uint16_t uwPrescalerValue = 0; 
volatile static int graphicDirection = 0;


extern int matrixTimerCount;


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


void drawGraphic(int direction) {
    // converting to zero based since it comes in 1-4 to match prevision LCD drawing code
    int graphicsDir = direction - 1;
    matrixTimerCount = 0;
    uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
    TimHandle.Instance = TIM2;
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

    HUB75E_clearDisplayBuffer();
    while (matrixTimerCount < 2) {
        HUB75E_setDisplayBuffer(myBitmap);
        updateFunctions[graphicsDir](myBitmap, PIXELS_COUNT_IN_BYTES);
        HUB75E_setDisplayBrightness(BrightnessLevel3);
        HUB75E_displayBufferPixels(myBitmap);
    }
    HAL_Delay(250);
    HAL_TIM_Base_Stop_IT(&TimHandle);
}

void playSequenceLoop() {
  
    uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
    TimHandle.Instance = TIM2;
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



void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
  
}



