CRGB matrix[MTX_NUM_LEDS];


void initMatrix() {
  FastLED.addLeds<MTX_CHIPSET, MTX_PIN, MTX_CLR_ORDER>(matrix, MTX_NUM_LEDS);
  
  #ifdef MTX_CURRENT_LIM
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MTX_CURRENT_LIM);
  #endif

  FastLED.setBrightness(32);

  matrixCalibrate();
}


void fillMatrix(CRGB color) {
  for (uint16_t i = 0; i < MTX_NUM_LEDS; i++)
    matrix[i] = color;
}


void fillMatrix(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, CRGB color) {
  for (uint8_t x = start_x; x < end_x; ++x) {
    for (uint8_t y = start_y; y < end_y; ++y) {
      matrix[getMatrixLEDNum(x, y)] = color;
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
    matrix[getMatrixLEDNum(x, y)] = color;
}


void refreshMatrix() {
  FastLED.show();
}


void clearMatrix(bool refresh) {
  FastLED.clear();
  if (refresh)
    FastLED.show();
}
