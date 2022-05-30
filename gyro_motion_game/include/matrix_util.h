#ifndef __MATRIX_UTIL__H
#define __MATRIX_UTIL__H

void fillRightRectBuffer(unsigned char*bitmap, int bitmap_size);
void fillLeftRectBuffer(unsigned char*bitmap, int bitmap_size);
void fillTopRectBuffer(unsigned char*bitmap, int bitmap_size);
void fillBottomRectBuffer(unsigned char*bitmap, int bitmap_size);
void clearBuffer();
void playSequenceLoop();
void drawGraphic(int direction);
void animateX();
void animateSmiley();

#endif