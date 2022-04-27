// #include "LcdDisplay.h"

// LcdDisplay::LcdDisplay() {

// }

// LcdDisplay::~LcdDisplay() {

// }
// void LcdDisplay::Init() {
//       /*##-1- Initialize the LCD #################################################*/
//   /* Initialize the LCD */
//   BSP_LCD_Init();
  
//   /* Initialize the LCD Layers */
//   BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER);
// }

// void LcdDisplay::DrawCircle(int x, int y, int radius, int filled, int color) {

//     if (!filled) {
//         BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
//     } else {
//         BSP_LCD_SetTextColor(color);
//     }

//     BSP_LCD_FillCircle(x,y, radius);
// }

// void LcdDisplay::DrawLeftCircle(int filled, int color) {
//     if (!filled) {
//         BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
//     } else  {
//         BSP_LCD_SetTextColor(color);
//     }
//     this->DrawCircle((BSP_LCD_GetXSize() / 5), (310-110), 30, 1, LCD_COLOR_ORANGE);
// }

