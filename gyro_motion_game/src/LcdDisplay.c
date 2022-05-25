#include "LcdDisplay.h"
#include "game.h"
#include <stdio.h>

void drawCircle(int circleLocation, int filled) {
    int x, y, radius;
	switch (circleLocation) {
		case 1: // Left
			x = (BSP_LCD_GetXSize() / 5);
			y = 155;
			BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
			break;
		case 2: // Up
			x = (BSP_LCD_GetXSize() / 2);
			y = 40;
			BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
			break;
		case 3: // Right
		    x = (BSP_LCD_GetXSize() / 5) * 4;
			y = 155;
			BSP_LCD_SetTextColor(LCD_COLOR_DARKYELLOW);
			break;
		case 4: // down
			x = (BSP_LCD_GetXSize() / 2);
			y = 270;
			BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
			break;
        default:
            // if a circle appears in the center, it's  a bug
            x= (BSP_LCD_GetXSize() / 2);
            y= (BSP_LCD_GetXSize() / 2);
            break;
	}
	radius = 30;
	
	if (!filled) {
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	}
	BSP_LCD_FillCircle(x, y, radius);
}

void drawTitleScreen() {
	BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0,100, (uint8_t*)"Gyro Motion", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0,150, (uint8_t*)"Press Button to Start", CENTER_MODE);
}

void drawGameOverScreen() {
	uint8_t scoreStr[40];
	int gameScore = getGameScore();
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
    BSP_LCD_SelectLayer(0);

	BSP_LCD_Clear(LCD_COLOR_WHITE);

    BSP_LCD_SetFont(&Font16);
 
    BSP_LCD_DisplayStringAt(0,100, (uint8_t*)"Game Over", CENTER_MODE);
    sprintf((char *)scoreStr, "Score : %d", gameScore);
    BSP_LCD_DisplayStringAt(0,125, (uint8_t*)scoreStr, CENTER_MODE);
    
    BSP_LCD_DisplayStringAt(0,150, (uint8_t*)"Press Button to ", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0,170, (uint8_t*)"Start New Game", CENTER_MODE);
}

void updateScoreDisplay() {
    char scoreStr[20];
 	int gameScore = getGameScore();
    sprintf(scoreStr, "Score: %d", gameScore);
    BSP_LCD_DisplayStringAtLine(2, (uint8_t*)scoreStr);
    
}

void drawGoText() {
	BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"GO!!");
    HAL_Delay(500);
    BSP_LCD_ClearStringLine(1);

}