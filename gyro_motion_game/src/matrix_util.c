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

void clearBuffer() {
    HUB75E_setDisplayBuffer(myBitmap);
    memset(myBitmap,0, PIXELS_COUNT_IN_BYTES);
    HUB75E_setDisplayColor(Black);
    HUB75E_displayBufferPixels(myBitmap);   
    HUB75E_clearDisplayBuffer();
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


void refresh_display(unsigned char *bitmap, int num_loops) {
    for (int kk=0; kk < num_loops; kk++) {
         HUB75E_setDisplayBuffer(bitmap);
         HUB75E_displayBufferPixels(bitmap);
    }


}

void drawPixels (unsigned char *pixel_list, int pixel_list_size, unsigned char *bitmap) {
    for (int ii=0; ii < pixel_list_size; ii+=2) {
        int idx = pixel_list[ii];
        int pixels = pixel_list[ii+1];
        bitmap[idx] = pixels;
        for (int jj=0; jj < 3; jj++) {
            HUB75E_setDisplayBuffer(bitmap);
            HUB75E_displayBufferPixels(bitmap);
        }
        
    }
}


void animateX() {
    HUB75E_clearDisplayBuffer();
    clearBuffer();
    HUB75E_setDisplayColor(Red);
    unsigned char pixel_list[] = {
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
    
    
    drawPixels(pixel_list, sizeof(pixel_list) / sizeof (unsigned char), myBitmap);

    refresh_display(myBitmap, 1500);

    clearBuffer();
}