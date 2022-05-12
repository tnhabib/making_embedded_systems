
#include "gyro_util.h"
#include "game.h"
#include "LcdDisplay.h"
#include <stdio.h>

static int gMatchSequence[100];
static int gSeqSize;
static int gScore = 0;
static enum GameState gGameState = TITLE_SCREEN;

#define ABS(x)         (x < 0) ? (-x) : x

static const sGameStateMap theGameStateMap[] = 
{
    {TITLE_SCREEN, titleScreen},
    {START_GAME, startGame},
    {PLAY_SEQUENCE, playSequence},
    {COMPARE_SEQUENCE, compareSequence},
    {GAME_OVER, gameOver},
    {INCREMENT_SEQUENCE_SIZE, incMatchSequence},

    {NULL_STATE, NULL}
};
enum GameState getGameState() {
    return gGameState;
}

void setGameState(enum GameState state) {
    gGameState = state;
}
void nextGameState(enum GameState state) {
    int idx = 0;
    int found = 0;
    while ( ( NULL != &theGameStateMap[idx] ) && ( 0 == found ) ) {
        if (theGameStateMap[idx].mState == state) {
            theGameStateMap[idx].mFunction();
            found = 1;
        }
        idx++;
    }
}

int startGame() {

  // reset game properties
    gScore = 0;
    gSeqSize = 0;
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    incMatchSequence();
    gGameState = PLAY_SEQUENCE;

    return 0;
}

int titleScreen() {
    // reset game properties
    gScore = 0;
    gSeqSize = 0;
     BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0,100, (uint8_t*)"Gyro Motion", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0,150, (uint8_t*)"Press Button to Start", CENTER_MODE);

    return 0;

}

int gameOver() {
    
    uint8_t scoreStr[40];

    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
    BSP_LCD_SelectLayer(0);


    BSP_LCD_SetFont(&Font16);
    BSP_LCD_ClearStringLine(1);
    BSP_LCD_ClearStringLine(2);
    BSP_LCD_DisplayStringAt(0,100, (uint8_t*)"Game Over", CENTER_MODE);
    sprintf(scoreStr, "Score : %d", gScore);
    BSP_LCD_DisplayStringAt(0,125, (uint8_t*)scoreStr, CENTER_MODE);
    
    BSP_LCD_DisplayStringAt(0,150, (uint8_t*)"Press Button to ", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0,170, (uint8_t*)"Start New Game", CENTER_MODE);
    return 0;
}
int * getMatchSequence() {
    return gMatchSequence;
}

int getSequenceSize() {
    return gSeqSize;
}

void resetSequence() {
  gSeqSize = 0;
}

int incMatchSequence() {
    gMatchSequence[gSeqSize] = getRandomValue();
    gSeqSize++;

    gGameState = PLAY_SEQUENCE;
    return 0;

}

int getRandomValue() {
    float xyzGyro[3];
    HAL_Delay(50);
    I3G4250D_ReadXYZAngRate(xyzGyro);
    
    int randomVal= (int) ( ABS(xyzGyro[0]) + ABS(xyzGyro[1]) + ABS(xyzGyro[2]) ) % 4 ;
    return randomVal + 1;
}

int playSequence() {
    int animDelay = 1000;
    for (int ii=0; ii < gSeqSize; ii++) {
        drawCircle(gMatchSequence[ii], 1);
        HAL_Delay(animDelay);
        drawCircle(gMatchSequence[ii], 0);
        HAL_Delay(animDelay);
    }
    gGameState = COMPARE_SEQUENCE;
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
    BSP_LCD_SelectLayer(0);
    BSP_LCD_DisplayStringAt(0,100, (uint8_t*)"GO!!", CENTER_MODE);
    HAL_Delay(500);
    return 0;
}

int compareSequence() {
    int matches = 0;
    int userMotionDirection;
    float xyzGyro[3];
  
    for (int ii=0; ii < gSeqSize; ii++) {
        userMotionDirection = waitforGyroMotionDetection(xyzGyro);
        if (userMotionDirection == gMatchSequence[ii]) {
            drawCircle(userMotionDirection,1);
            matches++;
            gScore++;
        } else {
            break;
        }
        HAL_Delay(500);        
        drawCircle(userMotionDirection,0);
    }
    if (matches == gSeqSize) {
        gGameState = INCREMENT_SEQUENCE_SIZE;
    } else {
        gGameState = GAME_OVER;
    }
    HAL_Delay(500);
    return matches;

}