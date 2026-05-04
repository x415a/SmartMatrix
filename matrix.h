#pragma once

#include "Arduino.h"
#include "FastLED.h"
#include "define.h"

#define MTX_CLR_ORDER GRB
#define MTX_W 16
#define MTX_H 16
#define MTX_NUM_LEDS MTX_W * MTX_H
#define MTX_CURRENT_LIM 1600
#define MTX_CHIPSET WS2813
#define MTX_MIRROR false
#define MTX_FLIP
#define MTX_BRIGHTNESS 255
#define MTX_DEFAULT_BRIGHTNESS 100
#define MTX_CALIBRATION_BRIGHTNESS 32
// #define MTX_CALIBRATION
// #define MTX_CALIBRATION_FILL


void initMatrix();
void resetMatrix();
void fillMatrix(CRGB const &);
void fillMatrix(uint8_t, uint8_t, uint8_t, uint8_t, CRGB const &);
void setMatrixLED(uint8_t, uint8_t, CRGB const &);
void setMatrixLED(uint16_t, CRGB const &);
uint16_t getMatrixLEDNum(uint8_t, uint8_t);
void refreshMatrix();
void setBrightness(uint8_t);
void resetBrightness();
void clearMatrix(bool=false);
void smoothFillMatrix(CRGB const &, uint16_t);
