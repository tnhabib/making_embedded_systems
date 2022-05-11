
#include "gyro_util.h"
#include "game.h"
#include "LcdDisplay.h"

static int matchSequence[100];
static int seqSize;
static int score = 0;
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

    BSP_LCD_Clear(LCD_COLOR_WHITE);
    incMatchSequence();
    gGameState = PLAY_SEQUENCE;

    return 0;
}

int titleScreen() {
    // reset game properties
    score = 0;
    seqSize = 0;
     BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0,100, (uint8_t*)"Gyro Motion", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0,150, (uint8_t*)"Press Button to Start", CENTER_MODE);

    return 0;

}

int gameOver() {
    //BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_ClearStringLine(1);
    BSP_LCD_ClearStringLine(2);
    BSP_LCD_DisplayStringAt(100,100, (uint8_t*)"Game Over", CENTER_MODE);
 
    return 0;
}
int * getMatchSequence() {
    return matchSequence;
}

int getSequenceSize() {
    return seqSize;
}

void resetSequence() {
  seqSize = 0;
}

int incMatchSequence() {
    matchSequence[seqSize] = getRandomValue();
    seqSize++;

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
    for (int ii=0; ii < seqSize; ii++) {
        drawCircle(matchSequence[ii], 1);
        HAL_Delay(animDelay);
        drawCircle(matchSequence[ii], 0);
        HAL_Delay(animDelay);
    }
    gGameState = COMPARE_SEQUENCE;
    BSP_LCD_DisplayStringAt(0,100, (uint8_t*)"GO!!", CENTER_MODE);
    return 0;
}

int compareSequence() {
    int matches = 0;
    int userMotionDirection;
    float xyzGyro[3];
  
    for (int ii=0; ii < seqSize; ii++) {
        userMotionDirection = waitforGyroMotionDetection(xyzGyro);
        if (userMotionDirection == matchSequence[ii]) {
            drawCircle(userMotionDirection,1);
            matches++;
            score++;
        } else {
            break;
        }
        HAL_Delay(250);        
        drawCircle(userMotionDirection,0);
    }
    if (matches == seqSize) {
        gGameState = INCREMENT_SEQUENCE_SIZE;
    } else {
        gGameState = GAME_OVER;
    }
    HAL_Delay(500);
    return matches;

}