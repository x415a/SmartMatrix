#include "button.h"


struct Button;
template <uint8_t N>
void _init(Button (&)[N]);
template <uint8_t N>
void _update(Button (&)[N]);


struct Button {
  Button(uint8_t pin, uint8_t id, bool default_state=false): pin(pin), id(id) {
    _button_state |= default_state << 4;
  }

  uint8_t update() {
    checkClick();
    checkLongPress();
    checkDoubleClick();

    if (isReady()) {
      uint8_t res = getCurrentEvent();
      reset();
      return res;
    }

    return 0;
  }

  uint8_t getDefaultPinState() const {
    return _button_state & BUTTON_DEF_STATE_BIT;
  }

  uint8_t pin, id;

private:
  uint32_t _event_time = 0, _last_state_change = 0;

  // 0b00000001 actual state
  // 0b00000010 mid state
  // 0b00000100 double click state
  // 0b00001000 long press click
  // 0b00010000 default pin state
  // 0b00100000 delay waiting state
  // 0b01000000 button clicked and ready
  uint8_t _button_state = 0;

  void reset() {
    _button_state &= 0b10011011;
  }

  uint8_t isDoubleClick() const {
    return _button_state & BUTTON_DOUBLE_CLICK_BIT;
  }

  uint8_t isLongPress() const {
    return _button_state & BUTTON_LONG_PRESS_BIT;
  }

  uint8_t isPressed() const {
    return _button_state & BUTTON_STATE_BIT;
  }

  uint8_t getCurrentEvent() {
    if (_button_state & BUTTON_DOUBLE_CLICK_BIT)
      return EVENT_FLAG_DOUBLE_CLICK;
    else if (_button_state & BUTTON_LONG_PRESS_BIT)
      return EVENT_FLAG_LONG_PRESS;
    return EVENT_FLAG_CLICK;
  }

  uint8_t isReady() const {
    return _button_state & BUTTON_READY_BIT;
  }

  bool getPinState() const {
    return (getDefaultPinState() > 0) != (digitalRead(pin) > 0);
  }

  void changeState() {
    _event_time = millis();
    _button_state ^= BUTTON_MID_STATE_BIT;
  }

  void checkClick() {
    if (getPinState() != isPressed()) {
      if ((~_button_state & BUTTON_STATE_BIT) ^ ((_button_state & BUTTON_MID_STATE_BIT) >> 1)) {
        changeState();
      } else {
        checkDelay();
      }
    } else {
      _button_state &= 0b11111101 | (isPressed() << 1);
    }
  }

  void checkDelay() {
    if (getMillisDelay(_event_time) < BUTTON_RATTLING_DELAY_MS)
      return;

    _button_state ^= BUTTON_STATE_BIT;
    _last_state_change = millis();

    #ifdef ENABLE_BUTTON_DEBUG
    Serial.print("BTN: ");
    Serial.print(id);
    Serial.print(" changed. st: ");
    Serial.println(isPressed());
    #endif
  }

  void checkDoubleClick() {
    if (_button_state & BUTTON_DELAY_WAIT_BIT) {
      if (isPressed() || getMillisDelay(_last_state_change) > BUTTON_DOUBLE_CLICK_DELAY_MS) {
        _button_state = (_button_state | BUTTON_READY_BIT | (isPressed() << 2)) & 0b11011111;
      }
    } else if (!isLongPress() && !isPressed() && getMillisDelay(_last_state_change) < BUTTON_RATTLING_DELAY_MS) {
      _button_state |= BUTTON_DELAY_WAIT_BIT;
      #ifdef ENABLE_BUTTON_DEBUG
      Serial.print("BTN: ");
      Serial.print(id);
      Serial.print(" dc wait. st: ");
      Serial.println(_button_state);
      #endif
    }
  }

  void checkLongPress() {
    if (!isLongPress() && isPressed() && getMillisDelay(_last_state_change) > BUTTON_LONG_PRESS_DELAY_MS) {
      _button_state = (_button_state | BUTTON_READY_BIT | (isPressed() << 3)) & 0b11011011;
      #ifdef ENABLE_BUTTON_DEBUG
      Serial.print("BTN: ");
      Serial.print(id);
      Serial.println(" lp dec");
      #endif
    } else if (!isPressed() && getMillisDelay(_last_state_change) > BUTTON_RATTLING_DELAY_MS) {
      _button_state &= 0b11110111;
    }
  }
} buttons[] = {
  Button(BUTTON_1_PIN, BUTTON_1_ID, true)
};


void initButtons() {
  _init(buttons);
}


void updateButtons() {
  _update(buttons);
}


template <uint8_t N>
void _init(Button (&arr)[N]) {
    for (int i = 0; i < N; ++i) {
      pinMode(arr[i].pin, INPUT_PULLUP);
      digitalWrite(arr[i].pin, arr[i].getDefaultPinState());
    }
}


template <uint8_t N>
void _update(Button (&arr)[N]) {
  uint8_t event_type = 0;

  for (int i = 0; i < N; ++i) {
    if (event_type = buttons[i].update()) {
      #ifdef DEBUG
      Serial.print("BTN: ");
      Serial.print(buttons[i].id);
      Serial.print(" click! ev: ");
      Serial.println(event_type);
      #endif

      handleButtonClickEvent(buttons[i].id, event_type);
    }
  }
}
