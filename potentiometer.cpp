#include "potentiometer.h"

#define _PT_UPDATE_DELAY (uint16_t)(1000 / POTENTIOMETER_UPD_FREQ)

static uint16_t _current_value = 0, _mean = 0;
static uint32_t _last_update = 0;


int32_t getPtInterpolatedValue(int32_t left, int32_t right) {
  return interpolatePtValue(left, right, _current_value);
}


int32_t interpolatePtValue(int32_t left, int32_t right, int32_t value) {
  return left + value * (right - left) / (POTENTIOMETER_MAX - POTENTIOMETER_MIN);
}


void initPotentiometer() {
  #ifdef DEBUG
  Serial.println("PTR: init");
  #endif
  pinMode(POTENTIOMETER_PIN, INPUT_PULLUP);
  digitalWrite(POTENTIOMETER_PIN, LOW);
  _mean = analogRead(POTENTIOMETER_PIN);
  updatePtValue();
}


void checkPtUpdates() {
  if (getMillisDelay(_last_update) > _PT_UPDATE_DELAY) {
    _last_update = millis();
    _mean += (int16_t) (analogRead(POTENTIOMETER_PIN) - _mean) / POTENTIOMETER_UPD_BUF;
  }
}


bool isPtValueChanged() {
  return abs(_mean - _current_value) > POTENTIOMETER_TRESHOLD;
}


uint16_t updatePtValue() {
  #ifdef POTENTIOMETER_DEBUG
  Serial.print("Pt: ");
  Serial.println(_mean);
  #endif
  _current_value = _mean;
  return _current_value;
}


inline uint16_t getPtValue(bool update = false) {
  return (update) ? updatePtValue() : _current_value;
}
