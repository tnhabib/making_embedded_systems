
#include "gyro_util.h"
#include "game.h"

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