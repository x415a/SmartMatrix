#include "define.h"
#include "control.h"
#include "modules.h"
#include "time.h"


void setup() {
  #ifdef DEBUG
    Serial.begin(BOD_RATE);
  #endif

  randomSeed(analogRead(0));
  initModules();
}


void loop() {
  updateControl();
  updateModules();
  frameTick();
}
