#include "controls.h"

#define ID_BITS(x) uint8_t(x) << 6
#define ST_BITS(x) uint8_t(x) << 2
#define TRANSITION(a, b, c, d) (uint16_t)((ID_BITS(a)) << 8) | ((ID_BITS(b)) << 6) | ((ST_BITS(c)) << 4) | ((ST_BITS(d)) >> 2)
#define TRANS_COMPARE(a, b, c) ((uint16_t)((ID_BITS(a)) << 8) | ((ID_BITS(b)) << 6) | ((ST_BITS(c)) << 4))


void checkCurrentStateExit();
void checkCurrentStateEnter();


static uint8_t current_state = STATE_BLACKOUT;

uint16_t control_schema[] = {
  TRANSITION(BUTTON_1_ID, EVENT_FLAG_CLICK, STATE_BLACKOUT, STATE_CLOCK),
  TRANSITION(BUTTON_1_ID, EVENT_FLAG_CLICK, STATE_CLOCK, STATE_AMBIENT),

  TRANSITION(BUTTON_1_ID, EVENT_FLAG_CLICK, STATE_AMBIENT, STATE_BLACKOUT),
  // TRANSITION(BUTTON_1_ID, EVENT_FLAG_DOUBLE_CLICK, STATE_AMBIENT, STATE_AMBIENT_SET_BRT),
  TRANSITION(BUTTON_1_ID, EVENT_FLAG_CLICK, STATE_AMBIENT_SET_BRT, STATE_AMBIENT),

  TRANSITION(BUTTON_1_ID, EVENT_FLAG_CLICK, STATE_ALARM_CLOCK, STATE_ALARM_CLOCK_ABORTED),

  // BUTTON_1_ID, EVENT_FLAG_CLICK, STATE_IN_MENU, STATE_SET_NEXT_MENU_ITEM,
  // BUTTON_1_ID, EVENT_FLAG_DOUBLE_CLICK, STATE_IN_MENU, STATE_SELECT_MENU_ITEM,
  // BUTTON_1_ID, EVENT_FLAG_LONG_PRESS, STATE_IN_MENU, STATE_EXIT_MENU,

  // BUTTON_1_ID, EVENT_FLAG_CLICK, STATE_BLACKOUT, STATE_CLOCK,
  // BUTTON_1_ID, EVENT_FLAG_DOUBLE_CLICK, STATE_BLACKOUT, STATE_OPEN_MAIN_MENU,
  // BUTTON_1_ID, EVENT_FLAG_LONG_PRESS, STATE_BLACKOUT, STATE_OPEN_SETTINGS_MENU,

  // BUTTON_1_ID, EVENT_FLAG_CLICK, STATE_PLAYING_EFFECT, STATE_SET_NEXT_MENU_ITEM,
  // BUTTON_1_ID, EVENT_FLAG_DOUBLE_CLICK, STATE_PLAYING_EFFECT, STATE_SET_PREV_MENU_ITEM,
  // BUTTON_1_ID, EVENT_FLAG_LONG_PRESS, STATE_PLAYING_EFFECT, STATE_OPEN_EFF_SET_MENU,
};


void initControls() {
  pinMode(MAIN_POWER_TR_PIN, OUTPUT);
  powerOn();
}


void readyBlink() {
  for (int i = 0, st = 0; i < 10; ++i) {
    digitalWrite(LED_SIGNAL_PIN, st = !st);
    delay(100);
  }
}


void setCurrentState(uint8_t state) {
  #ifdef DEBUG
  Serial.print("STM: switch ");
  Serial.print(current_state);
  Serial.print("->");
  Serial.println(state);
  #endif
  checkCurrentStateExit();
  current_state = state;
  checkCurrentStateEnter();
}


template <uint8_t N>
bool _changeCurrentState(uint16_t (&arr)[N], uint8_t button_id, uint8_t event_type) {
  bool success = false;
  for (uint8_t i = 0; i < N; ++i) {
    if (_checkoutTransition(button_id, event_type, arr[i])) {
      setCurrentState(_extractTransitionState(arr[i]));
      success = true;
      break;
    }
  }
  return success;
}


bool _checkoutTransition(uint8_t btn, uint8_t ev, uint16_t trans) {
  return !(trans & 0xffc0 ^ TRANS_COMPARE(btn, ev, current_state));
}


uint8_t _extractTransitionState(uint16_t trans) {
  return trans & 0x3f;
}


void handleButtonClickEvent(uint8_t button_id, uint8_t event_type) {
  #ifdef DEBUG
  Serial.println("STM: new ev");
  #endif

  if (!_changeCurrentState(control_schema, button_id, event_type)) {
    #ifdef DEBUG
    Serial.println();
    Serial.println("STM: 404");
    #endif
  }
}

inline void powerOn() {
  #ifdef DEBUG
  Serial.println("PWR: ON");
  #endif
  digitalWrite(MAIN_POWER_TR_PIN, HIGH);
}

inline void powerOff() {
  #ifdef DEBUG
  Serial.println("PWR: OFF");
  #endif
  digitalWrite(MAIN_POWER_TR_PIN, LOW);
}


void updateStateMachine() {
  if (checkAlarmClock() && current_state != STATE_ALARM_CLOCK && current_state != STATE_ALARM_CLOCK_ABORTED) {
    powerOn();
    setCurrentState(STATE_ALARM_CLOCK);
  }

  switch (current_state)
  {
    case STATE_BLACKOUT:
      break;
    case STATE_AMBIENT:
      getCurrentEffect()->update();
      break;
    case STATE_CLOCK:
      updateCommonClock();
      break;
    case STATE_ALARM_CLOCK:
      if (!updateAlarmClock())
        setCurrentState(STATE_CLOCK);
      break;
    case STATE_ALARM_CLOCK_ABORTED:
      abortAlarmClock();
      setCurrentState(STATE_CLOCK);
      break;
    default:
      emitError(ERROR_STATES_MACHINE, "STM_INV");
  }
}

void checkCurrentStateEnter() {
  switch(current_state) {
    case STATE_CLOCK:
      setBrightness(getPtInterpolatedValue(0, 255));
      refreshMatrix();
      break;
    case STATE_BLACKOUT:
      clearMatrix(true);
      powerOff();
      break;
  }
}

void checkCurrentStateExit() {
  switch(current_state) {
    case STATE_BLACKOUT:
      resetMatrix();
      powerOn();
      break;
    case STATE_ALARM_CLOCK_ABORTED:
    case STATE_ALARM_CLOCK:
      resetMatrix();
      break;
    case STATE_AMBIENT:
      clearMatrix(false);
      break;
    case STATE_CLOCK:
      clearMatrix(false);
      resetBrightness();
      break;
  }
}
