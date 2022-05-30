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

void refresh_display(unsigned char *bitmap, int num_loops) {
    for (int kk=0; kk < num_loops; kk++) {
         HUB75E_setDisplayBuffer(bitmap);
         HUB75E_displayBufferPixels(bitmap);
    }


}
void animateX(unsigned char*bitmap, int bitmap_size) {
    HUB75E_clearDisplayBuffer();
    clearBuffer(bitmap, bitmap_size);
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
    
  
    drawPixels(pixel_list, sizeof(pixel_list) / sizeof (unsigned char), bitmap);

    refresh_display(bitmap, 1500);
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
void animateSmiley(unsigned char*bitmap, int bitmap_size) {

    HUB75E_clearDisplayBuffer();
    clearBuffer(bitmap, bitmap_size);
    HUB75E_setDisplayColor(Green);
    
    unsigned char circle_list[] = {
        3, 3,
        4, 240,
        11, 28, 
       
        19, 32,
        27, 64,
        35, 128,
        42, 1,
        50, 2,
        58, 4, // eyes start here
        66, 4,
        74, 8,
        82, 8,
        90, 8,
        
        98,  16,
        106, 16,
        114, 16,
        122, 16,
        130, 16,
        138, 16, // smile starts here
        146, 16,

        154, 8,
        162, 8,
        170, 8,

        178, 4,
        186, 4,

        194, 2,
        202, 1,
        211, 128,
        219, 64,
        227, 32,
        235, 28,
        243, 3,
        244, 240,
        213, 64,
        221, 128,
        228, 1,
        236, 14,
        205, 32,
        197, 16,
        181, 8,
        189, 8,
        173, 4,
        165, 4,
        157, 4,
        
        149, 2,
        141, 2,
        133, 2,
        125, 2,
        117, 2,
        109, 2,
        101, 2,

        93, 4,
        85, 4,
        77, 4,

        69, 8,
        61, 8,

        53, 16,
        45, 32,
        37, 64,
        29, 128,
        20, 1,

        12, 14
    };

    drawPixels(circle_list, sizeof(circle_list) / sizeof(unsigned char), bitmap);

    unsigned char face_list[] = {
        59, 24,
        67, 24,
        75, 24,

        60, 6,
        68, 6,
        76, 6,

        138, 23,
        139, 192,
        146, 17,
        154, 9,
        155, 128,
        163, 192,
        171, 96,
        179, 48,
        187, 31,
        188, 254,
        180, 3,
        172, 1,
        173, 132,
        165, 196,
        157, 100,
        149, 34,
        141, 250,
    };
     
    drawPixels(face_list, sizeof(face_list) / sizeof(unsigned char), bitmap);
    refresh_display(bitmap, 1500);

}

void clearBuffer(unsigned char * bitmap, int bitmap_size) {
    HUB75E_setDisplayBuffer(bitmap);
    memset(bitmap,0, bitmap_size);
    HUB75E_setDisplayColor(Black);
    HUB75E_displayBufferPixels(bitmap);
    HUB75E_clearDisplayBuffer();
    HAL_Delay(250);
   
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