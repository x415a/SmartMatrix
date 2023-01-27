#include "FastLED.h"
#include "define.h"
#include "control.h"
#include "effects.h"
#include "clock.h"
#include "matrix.h"
#include "time.h"


void setup() {
  #ifdef DEBUG
    Serial.begin(BOD_RATE);
  #endif

  randomSeed(analogRead(0));

  initTime();
  initMatrix();
  initEffects();
}


void loop() {
  updateControl();
  //updateEffects();
  drawClock(DateTime(getUnixTime()), CRGB::Green);
  refreshMatrix();
  clearMatrix();
  frameTick();
}