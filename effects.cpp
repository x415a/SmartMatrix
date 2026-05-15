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
  bool update = false;
  uint16_t pt = getPtInterpolatedValue(0, 255);

  switch (mode) {
    case MODE_NORMAL:
      if (hue != pt) {
        hue = pt;
        update = true;
      }
      break;
    case MODE_SET_BRT:
      if (brt != pt) {
        brt = pt;
        update = true;
      }
      break;
  }

  if (update) {
    fillMatrix(CHSV(hue, 255, brt));
    refreshMatrix();
  }
}

void StaticAmbient::enter() {
  fillMatrix(CHSV(hue = getPtInterpolatedValue(0, 255), 255, brt));
  refreshMatrix();
}

void StaticAmbient::exit() {}


Ambient::Ambient() : StaticAmbient(), fill(false), led(0), last_update(0), beta(0) {}

void Ambient::update() {
  if (!fill) {
    if (getMillisDelay(last_update) > delay_) {
      hue += AMBIENT_CHANGE_STEP;
      fill = true;
      beta = getClockTimestamp() % 1024;
      loop = MTX_NUM_LEDS / gcd(beta, MTX_NUM_LEDS);
    }
  } else if (getMillisDelay(last_update) > AMBIENT_CHANGE_DELAY) {
    setMatrixLED(calculateLED(), CHSV(calculateHue(), 255, brt));
    refreshMatrix();
    last_update = millis();
    if (++led >= MTX_NUM_LEDS) {
      fill = false;
      led = 0;
    }
  }
}

uint16_t Ambient::calculateLED() {
  uint8_t xled = (uint32_t) ((led * beta + led / loop) % MTX_NUM_LEDS);
  return (uint32_t) (xled * beta + xled / loop) % MTX_NUM_LEDS;
}


uint8_t Ambient::calculateHue() {
  return hue + getPtInterpolatedValue(0, 255);
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
