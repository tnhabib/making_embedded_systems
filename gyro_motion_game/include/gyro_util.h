#ifndef __GYRO_UTIL_H
#define __GYRO_UTIL_H

#include <stdint.h>
#include "i3g4250d.h"

int waitforGyroMotionDetection(float* xyzGyro);
void setGyroMotionTolerance(float t);
void Gyro_Init();
void getGyroSample(float* xyzGyro);



#endif