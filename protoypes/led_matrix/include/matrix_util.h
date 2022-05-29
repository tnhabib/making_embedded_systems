#ifndef __MATRIX_UTIL__H
#define __MATRIX_UTIL__H
#include <stdint.h>


void fillRightRectBuffer(unsigned char*bitmap, int bitmap_size);
void fillLeftRectBuffer(unsigned char*bitmap, int bitmap_size);
void fillTopRectBuffer(unsigned char*bitmap, int bitmap_size);
void fillBottomRectBuffer(unsigned char*bitmap, int bitmap_size);
void clearBuffer(bitmap, bitmap_size);
void playSequenceLoop();
void drawGrapic(int direction);
void animateX(unsigned char*bitmap, int bitmap_size);
void animateSmiley(unsigned char*bitmap, int bitmap_size);


#endif