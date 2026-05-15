#pragma once

#define AMBIENT_CIRCLE_DURATION 30  // mins for 0-255 hue circle 
#define AMBIENT_CHANGE_DELAY 3      // msec between 2 leds update
#define AMBIENT_CHANGE_STEP 17      // hue step increment

#include "Arduino.h"
#include "HardwareSerial.h"
#include "define.h"
#include "matrix.h"
#include "button.h"
#include "potentiometer.h"
#include "clock.h"


#define AMBIENT_STEP_DURATION_MS ((uint32_t) AMBIENT_CIRCLE_DURATION * 60 * 1000 / 255)
#define AMBIENT_SHIFT_MS ((uint32_t) MTX_W * MTX_H * AMBIENT_CHANGE_DELAY)

struct Effect {
  virtual void enter() = 0;
  virtual void exit() = 0;
  virtual void update() = 0;
  virtual void event(uint8_t) = 0;
  virtual ~Effect() = default;
};


void initEffects();
Effect* nextEffect();
Effect* getCurrentEffect();


struct Blackout : public Effect {
  Blackout();
  void enter() override;
  void update() override;
  void event(uint8_t) override;
  void exit() override;
};

struct StaticAmbient : public Effect {
  StaticAmbient();
  void enter() override;
  void update() override;
  void event(uint8_t) override;
  void exit() override;

  protected:
    uint8_t hue, brt, mode;
    enum Modes { MODE_NORMAL, MODE_SET_BRT };
};


struct Ambient : public StaticAmbient {
  Ambient();
  void update();
  void enter();

  protected:
    uint8_t fill;
    uint16_t led, beta, loop;
    uint32_t last_update;
    const uint32_t delay_ = (AMBIENT_STEP_DURATION_MS > AMBIENT_SHIFT_MS) ? AMBIENT_STEP_DURATION_MS - AMBIENT_SHIFT_MS : 0;

    uint16_t calculateLED();
    uint8_t calculateHue();
};
