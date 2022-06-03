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

void clearMatrixDisplay() {
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
    clearMatrixDisplay();
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

    clearMatrixDisplay();
}

void animateSmiley() {

    HUB75E_clearDisplayBuffer();
    clearMatrixDisplay();
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

    drawPixels(circle_list, sizeof(circle_list) / sizeof(unsigned char), myBitmap);

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
     
    drawPixels(face_list, sizeof(face_list) / sizeof(unsigned char), myBitmap);
    refresh_display(myBitmap, 1500);

}

void animateGo() {
    HUB75E_clearDisplayBuffer();
    clearMatrixDisplay();
    HUB75E_setDisplayColor(Green);
    unsigned char g_pixel_list[] = {
        17, 255,
        18, 255,
        25, 255,
        26, 255,
       
        33, 192,
        41, 192,
        49, 192,
        57, 192,
       
        65, 192,
        73, 192,
        81, 192,
        89, 192,
        
        97, 192,
        105, 192,
        113, 192,
        121, 192,

        129, 192,
        137, 192,
        145, 192,
        153, 192,

        161, 192,
        169, 192,
       
        177, 255,
        178, 255,
        179, 192,
        185, 255,
        186, 255,
        187, 192,

        171, 192,
        163, 192,
        155, 192,
        147, 192,
      
        139, 192,
        131, 192,
        123, 192,
        115, 192,

        107, 192,
        99, 192, 
        106, 255,
        98, 255


        

       
    };
    unsigned char o_pixel_list[] = {
        19, 199,
        20, 255,
        21, 255,

        27, 199,
        28, 255, 
        29, 255,

        35, 6,
        43, 6,
        51, 6,
        59, 6,

        67, 6,
        75, 6,
        83, 6,
        91, 6,

        99, 198,
        107, 198,
        115, 198,
        123, 198,

        131, 198,
        139, 198,
        147, 198,
        155, 198,

        163, 198,
        171, 198,
        179, 199,
        187, 199,
      
        180, 255,
        181, 255,
        188, 255,
        189, 255,
        
        173, 3,
        165, 3,
        157, 3,
        149, 3,

        141, 3,
        133, 3,
        125, 3,
        117, 3,

        109, 3,
        101, 3,
        93, 3,
        85, 3,

        77, 3,
        69, 3,
        61, 3,
        53, 3,

        45, 3,
        37, 3,
        
    };

    unsigned char bang_pixel_list[] = {
        22, 12,
        30, 12,
        38, 12,
        46, 12,

        54, 12,
        62, 12,
        70, 12,
        78, 12,

        86, 12,
        94, 12,
        102, 12,
        110, 12,

        118, 12,
        126, 12,
        134, 12,
        142, 12,

        150, 12,
        158, 12,

        182, 12,
        190, 12,

    };

    drawPixels(g_pixel_list, sizeof(g_pixel_list) / sizeof (unsigned char), myBitmap);
    drawPixels(o_pixel_list, sizeof(o_pixel_list) / sizeof (unsigned char), myBitmap);
    drawPixels(bang_pixel_list, sizeof(bang_pixel_list) / sizeof (unsigned char), myBitmap);

    refresh_display(myBitmap, 750);


}
