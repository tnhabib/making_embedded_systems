#ifndef __GYRO_UTIL_H
#define __GYRO_UTIL_H

#include <stdint.h>
#include "i3g4250d.h"
#include "stm32f429i_discovery_gyroscope.h"
int waitforGyroMotionDetection();
void setGyroMotionTolerance(float t);
void Gyro_Init();
void getGyroSample(float* xyzGyro);

#endif