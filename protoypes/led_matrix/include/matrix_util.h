#ifndef __MATRIX_UTIL__H
#define __MATRIX_UTIL__H



void fillRightRectBuffer(unsigned char*bitmap, int bitmap_size);
void fillLeftRectBuffer(unsigned char*bitmap, int bitmap_size);
void fillTopRectBuffer(unsigned char*bitmap, int bitmap_size);
void fillBottomRectBuffer(unsigned char*bitmap, int bitmap_size);
void clearBuffer(bitmap, bitmap_size);
void playSequenceLoop();


#endif