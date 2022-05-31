
#include "gyro_util.h"
#include "game.h"
#include "LcdDisplay.h"
#include "matrix_util.h"
#include <stdio.h>
#include "HUB75ELib.h"

static int gMatchSequence[100];
static int gSeqSize;
static int gScore = 0;
static enum GraphicsMode gGraphicsMode = LED_MATRIX;
static enum GameState gGameState = TITLE_SCREEN;

#define ABS(x)         (x < 0) ? (-x) : x

int getGameScore() {
    return gScore;
}
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
enum GraphicsMode getGraphicsMode() {
    return gGraphicsMode;
}

void setGraphicsMode(int16_t mode) {
    
    switch (mode) 
    {
        case 1:
            gGraphicsMode = LCD_SCREEN;
            break;
        case 2:
            gGraphicsMode = LED_MATRIX;
            break;
        default:
            gGraphicsMode = LED_MATRIX;
    }
    
}

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

  // reset game properties, and clear displays
    HUB75E_clearDisplayBuffer();
    gScore = 0;
    gSeqSize = 0;
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    incMatchSequence();
    gGameState = PLAY_SEQUENCE;

  
    return 0;
}

int titleScreen() {
    // reset game properties
     HUB75E_clearDisplayBuffer();
    gScore = 0;
    gSeqSize = 0;
    drawTitleScreen();

    return 0;

}

int gameOver() {
    
    drawGameOverScreen();
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

        if (gGraphicsMode == LCD_SCREEN) {
            drawCircle(gMatchSequence[ii], 1);
            HAL_Delay(animDelay);
            drawCircle(gMatchSequence[ii], 0);
        }
        if (gGraphicsMode == LED_MATRIX) {
            drawGraphic(gMatchSequence[ii]);
        }

        HAL_Delay(animDelay);
    }
    if (gGraphicsMode == LED_MATRIX) {
        animateGo();
    }
       
    gGameState = COMPARE_SEQUENCE;
    if (gGraphicsMode == LCD_SCREEN) {
        drawGoText();
    }
    
    
    return 0;
}



int compareSequence() {
    int matches = 0;
    int userMotionDirection;
    float xyzGyro[3];

  
    for (int ii=0; ii < gSeqSize; ii++) {
        userMotionDirection = waitforGyroMotionDetection(xyzGyro);
        if (userMotionDirection == gMatchSequence[ii]) {
            matches++;
            gScore++;
            if (gGraphicsMode == LCD_SCREEN) {
                drawCircle(userMotionDirection,1);
            } 
            if (gGraphicsMode == LED_MATRIX) {
                drawGraphic(userMotionDirection);
                updateScoreDisplay();
                HAL_Delay(250);
            }
            
         
        } else {
            if (gGraphicsMode == LED_MATRIX) {
                animateX();
            }
           
            break;
        }
        if (gGraphicsMode == LCD_SCREEN) {   
            drawCircle(userMotionDirection,0);
            HAL_Delay(500);
        }
       
        
    }
    if (matches == gSeqSize) {
         if (gGraphicsMode == LED_MATRIX) {
                animateSmiley();
         }
           
        gGameState = INCREMENT_SEQUENCE_SIZE;
    } else {
        gGameState = GAME_OVER;
    }
    HAL_Delay(500);
    return matches;

}