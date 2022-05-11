
#include "gyro_util.h"
#include "game.h"
#include "LcdDisplay.h"

static int matchSequence[100];
static int seqSize;

#define ABS(x)         (x < 0) ? (-x) : x

int * getMatchSequence() {
    return matchSequence;
}

int getSequenceSize() {
    return seqSize;
}

void resetSequence() {
  seqSize = 0;
}

void incMatchSequence() {
    matchSequence[seqSize] = getRandomValue();
    seqSize++;

}

int getRandomValue() {
    float xyzGyro[3];
    I3G4250D_ReadXYZAngRate(xyzGyro);
    
    int randomVal= (int) ( ABS(xyzGyro[0]) + ABS(xyzGyro[1]) + ABS(xyzGyro[2]) ) % 4 ;
    return randomVal + 1;
}

void playSequence() {
    int animDelay = 1000;
    for (int ii=0; ii < seqSize; ii++) {
        drawCircle(matchSequence[ii], 1);
        HAL_Delay(animDelay);
        drawCircle(matchSequence[ii], 0);
        HAL_Delay(animDelay);
    }

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
        } else {
            break;
        }
        HAL_Delay(750);        
        drawCircle(userMotionDirection,0);
    }

    return matches;

}