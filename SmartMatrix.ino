#include "define.h"
#include "storage.h"

#ifndef REWRITE_STORAGE

#include "matrix.h"
#include "clock.h"
#include "controls.h"
#include "error.h"
#include "button.h"
#include "effects.h"
#include "potentiometer.h"
#include "lcd.h"


void setup() {
  #ifdef DEBUG
  Serial.begin(BOD_RATE);
  #endif

  initErrorIndicator();
  initButtons();
  initControls();
  initMatrix();
  initClock();
  initPotentiometer();
  initLCD();
  initEffects();
  readyBlink();
  powerOff();
}


void loop() {
  updateButtons();
  checkPtUpdates();
  updateStateMachine();
}

#else

void setup() {
  Serial.begin(BOD_RATE);
  Serial.println("TEST");
  showStorage();
}

void loop() {}

#endif
