#include "LcdDisplay.h"
#include "stm32f429i_discovery_lcd.h"

void drawCircle(int circleLocation) {
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
	
	
	BSP_LCD_FillCircle(x, y, radius);
}