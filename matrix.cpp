#include "matrix.h"

static CRGB MATRIX[MTX_NUM_LEDS];
static uint8_t brightness = 0;


void _matrixCalibrate() {
  FastLED.setBrightness(MTX_CALIBRATION_BRIGHTNESS);

  setMatrixLED(0, 0, CRGB::Red);
  setMatrixLED(0, 1, CRGB::Blue);
  setMatrixLED(1, 0, CRGB::Green);
  setMatrixLED(2, 0, CRGB::Green);
  refreshMatrix();
  clearMatrix();
  delay(2000);

  #ifdef MTX_CALIBRATION_FILL
  for (uint16_t x = 0; x < MTX_W; ++x) {
    for (uint16_t y = 0; y < MTX_H; ++y) {
      setMatrixLED(x, y, (x % 2) ? ((y % 2) ? CRGB::Yellow : CRGB::Green) : ((y % 2 == 0) ? CRGB::Violet : CRGB::Blue));
      refreshMatrix();
      delay(20);
    }
  }
  delay(1000);
  #endif

  clearMatrix(true);
}


void initMatrix() {
  FastLED.addLeds<MTX_CHIPSET, MTX_PIN, MTX_CLR_ORDER>(MATRIX, MTX_NUM_LEDS);

  #ifdef MTX_CURRENT_LIM
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MTX_CURRENT_LIM);
  #endif

  clearMatrix(true);
  #ifdef MTX_CALIBRATION
  _matrixCalibrate();
  #endif

  FastLED.setBrightness(MTX_BRIGHTNESS);
}

void resetMatrix() {
  resetBrightness();
  clearMatrix(true);
}


void smoothFillMatrix(CRGB const &color, uint16_t change_delay) {
  for (uint16_t i = 0; i < MTX_NUM_LEDS; i++) {
    MATRIX[i] = color;
    refreshMatrix();
    delay(change_delay);
  }
}


void fillMatrix(CRGB const & color) {
  for (uint16_t i = 0; i < MTX_NUM_LEDS; i++)
    MATRIX[i] = color;
}


void fillMatrix(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, CRGB const & color) {
  for (uint8_t x = start_x; x < end_x; ++x) {
    for (uint8_t y = start_y; y < end_y; ++y) {
      MATRIX[getMatrixLEDNum(x, y)] = color;
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


void setMatrixLED(uint8_t x, uint8_t y, CRGB const & color) {
  MATRIX[getMatrixLEDNum(x, y)] = color;
}


void setMatrixLED(uint16_t led, CRGB const & color) {
  MATRIX[led] = color;
}


void refreshMatrix() {
  FastLED.show();
}


void resetBrightness() {
  FastLED.setBrightness(MTX_BRIGHTNESS);
}


uint8_t getBrightness() {
  return brightness;
}


void setBrightness(uint8_t value) {
  static const float _brt_log = log(256);
  FastLED.setBrightness(brightness = (1 + (MTX_BRIGHTNESS - 1) * (1 - log(255 - value + 1) / _brt_log)));
}


void clearMatrix(bool refresh) {
  FastLED.clear(refresh);
}
