#include "FastLED.h"
#include "matrix.h"
#include "time.h"
#include "define.h"
#include "Arduino.h"
#include "effects.h"


void initEffects() {
  ambientLightingEffect(true);
  sunriseEffect(true);
}


void updateEffects() {
  ambientLightingEffect();
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
