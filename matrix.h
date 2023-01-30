#ifndef __MATRIX_H
#define __MATRIX_H
#include "Arduino.h"
#include "FastLED.h"

void initMatrix();
void fillMatrix(CRGB);
void fillMatrix(uint8_t, uint8_t, uint8_t, uint8_t, CRGB);
void setMatrixLED(uint8_t, uint8_t, CRGB);
uint16_t getMatrixLEDNum(uint8_t, uint8_t);
void refreshMatrix();
void clearMatrix(bool=false);

#endif
