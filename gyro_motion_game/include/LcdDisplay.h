
#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H
#include "stm32f429i_discovery_lcd.h"

void drawCircle(int circleLocation, int filled);
void drawTitleScreen();
void drawGameOverScreen();
void updateScoreDisplay();
void drawGoText();

#endif // LCDDISPLAY_H
