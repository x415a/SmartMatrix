#include "FastLED.h"
#include "effects.h"
#include "define.h"
#include "matrix.h"


CRGB __MATRIX[MTX_NUM_LEDS];


void _matrixCalibrate() {
  FastLED.setBrightness(MTX_CB_BRIGHTNESS);

  clearMatrix();
  setMatrixLED(0, 0, CRGB::Red);
  setMatrixLED(0, 1, CRGB::Blue);
  setMatrixLED(1, 0, CRGB::Green);
  setMatrixLED(2, 0, CRGB::Green);
  refreshMatrix();
  delay(2000);

  clearMatrix();
  for (uint16_t x = 0; x < MTX_W; ++x) {
    for (uint16_t y = 0; y < MTX_H; ++y) {
      setMatrixLED(x, y, (x % 2) ? ((y % 2) ? CRGB::Yellow : CRGB::Green) : ((y % 2 == 0) ? CRGB::Violet : CRGB::Blue));
      refreshMatrix();
      delay(20);
    }
  }
  delay(1000);

  clearMatrix(true);
}


void initMatrix() {
  FastLED.addLeds<MTX_CHIPSET, MTX_PIN, MTX_CLR_ORDER>(__MATRIX, MTX_NUM_LEDS);

  #ifdef MTX_CURRENT_LIM
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MTX_CURRENT_LIM);
  #endif

  #ifdef MTX_CALIBRATION
    _matrixCalibrate();
  #endif
}


void fillMatrix(CRGB color) {
  for (uint16_t i = 0; i < MTX_NUM_LEDS; i++)
    __MATRIX[i] = color;
}


void fillMatrix(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, CRGB color) {
  for (uint8_t x = start_x; x < end_x; ++x) {
    for (uint8_t y = start_y; y < end_y; ++y) {
      __MATRIX[getMatrixLEDNum(x, y)] = color;
    }
  }
}


uint16_t getMatrixLEDNum(uint8_t x, uint8_t y) {
  #ifndef MTX_FLIP
    if ((y % 2) == !MTX_MIRROR)
      return constrain(y, 0, MTX_H - 1) * MTX_W + MTX_W - constrain(x + 1, 1, MTX_W);
    return constrain(y, 0, MTX_H - 1) * MTX_W + constrain(x, 0, MTX_W - 1);
  #else
    if ((x % 2) == !MTX_MIRROR)
      return constrain(x, 0, MTX_W - 1) * MTX_H + MTX_H - constrain(y + 1, 1, MTX_H);
     return constrain(x, 0, MTX_W - 1) * MTX_H + constrain(y, 0, MTX_H - 1);
  #endif
}


void setMatrixLED(uint8_t x, uint8_t y, CRGB color) {
    __MATRIX[getMatrixLEDNum(x, y)] = color;
}


void refreshMatrix() {
  FastLED.show();
}


void clearMatrix(bool refresh) {
  FastLED.clear();
  if (refresh)
    FastLED.show();
}
