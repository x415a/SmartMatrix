typedef void (*CallbackFunc)();

void sunriseEffect(bool=false);
void ambientLightingEffect(bool=false);


void initEffects() {
  ambientLightingEffect(true);
  sunriseEffect(true);
}


void updateEffects() {
  ambientLightingEffect();
}


void matrixCalibrate() {
  clearMatrix();
  setMatrixLED(0, 0, CRGB::Red);
  setMatrixLED(0, 1, CRGB::Blue);
  setMatrixLED(1, 0, CRGB::Green);
  setMatrixLED(2, 0, CRGB::Green);
  refreshMatrix();
  delay(2000);
  clearMatrix(true);
}


void sunriseEffect(bool reset) {
  
}


void ambientLightingEffect(bool reset) {
  static uint32_t offset = 0;
  if (reset) {
    offset = random(0, AMBIENT_LIGHTING_SPEED);
    return;
  }
  float state = ((float) getCurrentTimeSeconds() + offset) / AMBIENT_LIGHTING_SPEED;
  uint8_t color = (state - (uint32_t) state) * 255;
  fillMatrix(CHSV(color, 100, 100));
}