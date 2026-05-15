#include "potentiometer.h"

#define _PT_UPDATE_DELAY (uint16_t)(1000 / POTENTIOMETER_UPD_FREQ)

uint16_t _readPtValue();

static uint32_t _last_update = 0;
static float _mean = 0;


int32_t getPtInterpolatedValue(int32_t left, int32_t right) {
  return interpolatePtValue(_mean, left, right);
}


int32_t interpolatePtValue(int32_t value, int32_t left, int32_t right) {
  return interpolate(value, POTENTIOMETER_MIN, POTENTIOMETER_MAX, left, right);
}


void initPotentiometer() {
  #ifdef DEBUG
  Serial.println("PTR: init");
  #endif
  pinMode(POTENTIOMETER_PIN, INPUT_PULLUP);
  digitalWrite(POTENTIOMETER_PIN, LOW);
  _mean = _readPtValue();
}


void checkPtUpdates() {
  if (getMillisDelay(_last_update) > _PT_UPDATE_DELAY) {
    _last_update = millis();
    _mean += (_readPtValue() - _mean) / POTENTIOMETER_UPD_BUF;
    #ifdef POTENTIOMETER_DEBUG
      Serial.print("Pt: ");
      Serial.println(_mean);
    #endif
  }
}


inline uint16_t getPtValue() {
  return _mean;
}

uint16_t _readPtValue() {
  return constrain(analogRead(POTENTIOMETER_PIN), POTENTIOMETER_MIN, POTENTIOMETER_MAX - 1);
}
