#include "effects.h"

template <uint8_t N>
Effect* _getEffectByNumber(Effect* (&)[N], uint8_t);

static uint8_t _current_effect = 1;
static Effect* _effects_list[3];

Blackout::Blackout() {}

void Blackout::enter() {
  clearMatrix(true);
  powerOff();
}

void Blackout::exit() {
  resetMatrix();
  powerOn();
}

void Blackout::event(uint8_t event_type) {
}

void Blackout::update() {}


StaticAmbient::StaticAmbient(): hue(0), brt(255), mode(MODE_NORMAL) {}

void StaticAmbient::event(uint8_t event_type) {
  switch (mode) {
    case MODE_NORMAL:
      if (event_type == EVENT_FLAG_DOUBLE_CLICK) mode = MODE_SET_BRT;
      break;
    case MODE_SET_BRT:
      if (event_type == EVENT_FLAG_CLICK) mode = MODE_NORMAL;
      break;
  }
}

void StaticAmbient::update() {
  if (isPtValueChanged()) {
    switch (mode) {
      case MODE_NORMAL:
        int32_t v1 = getPtInterpolatedValue(0, 255);
        hue += interpolatePtValue(0, 255, updatePtValue()) - v1;  // one-liner just won't work, idk
        fillMatrix(CHSV(hue, 255, brt));
        refreshMatrix();
        break;
      case MODE_SET_BRT:
        brt = interpolatePtValue(0, 255, updatePtValue());
        fillMatrix(CHSV(hue, 255, brt));
        refreshMatrix();
        break;
    }
  }
}

void StaticAmbient::enter() {
  fillMatrix(CHSV(hue = getPtInterpolatedValue(0, 255), 255, brt));
  refreshMatrix();
}

void StaticAmbient::exit() {}

Ambient::Ambient() : StaticAmbient(), fill(true), led(0), last_update(0) {}

void Ambient::update() {
  StaticAmbient::update();

  if (!fill) {
    if (getMillisDelay(last_update) > delay_) {
      hue += AMBIENT_CHANGE_STEP;
      fill = true;
    }
  } else if (getMillisDelay(last_update) > AMBIENT_CHANGE_DELAY) {
    setMatrixLED(led, CHSV(hue, 255, brt));
    refreshMatrix();
    last_update = millis();
    if (++led >= MTX_NUM_LEDS) {
      fill = false;
      led = 0;
    }
  }
}


void Ambient::enter() {
  StaticAmbient::enter();
  hue = getClockTimestamp();
}


void initEffects() {
  #ifdef DEBUG
  Serial.println("EFF: init");
  #endif
  // _effects_list[0] = new Blackout();
  _effects_list[1] = new Ambient();
  // _effects_list[2] = new StaticAmbient();
}


Effect* nextEffect() {
  return _getEffectByNumber(_effects_list, ++_current_effect);
}


Effect* getCurrentEffect() {
  return _getEffectByNumber(_effects_list, _current_effect);
}


template <uint8_t N>
Effect* _getEffectByNumber(Effect* (&arr)[N], uint8_t num) {
  return arr[num % N];
}
