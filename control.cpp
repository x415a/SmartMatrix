#include "time.h"
#include "define.h"
#include "control.h"


#define MENU_DEFAULT_STATE 0


class SensorButton {
private:
  uint8_t _pin, _clicks = 0, _res_clicks = 0;
  float _last_change = 0, _last_click = 0;
  bool _pressed = false, _state = false;
  String _name;
public:
  SensorButton(uint8_t pin, String name): _pin(pin), _name(name) {}
  
  uint8_t clicksCount() {
    return _res_clicks;
  }

  void resetClicks() {
    _res_clicks = 0;
  }
  
  bool isPressed() {
    return _pressed;
  }
  
  float pressedTime() {
    if (_pressed) {
      return getCurrentTimeSeconds() - _last_change; 
    }
    return 0;
  }

  bool update() {
    uint8_t state = digitalRead(_pin);
    float t = getCurrentTimeSeconds(), delay = t - _last_change;

    if (state) {
      if (!_state) {
        _state = true;
        _last_change = t;
      } else if (delay > BUTTON_PRESSED_DELAY) {
        _pressed = true;
      }
    } else if (_state) {
      if (delay > BUTTON_CLICK_DELAY && !_pressed) {
        _clicks++;
      }
      _state = _pressed = false;
    } else if (_clicks > 0 && delay > BUTTON_MULTICLICK_DELAY) {
      _res_clicks = _clicks;
      _clicks = 0;
    }

    return _pressed || _res_clicks > 0;
  }

  void repr() {
    #ifdef DEBUG
      Serial.print("Button <" + _name + ">: ");
      Serial.print("pr: ");
      Serial.print(_pressed);
      Serial.print(" cl: ");
      Serial.println(_res_clicks);
    #endif
  }
} _buttons[] = {SensorButton(BUTTON_L_PIN, "LB"), SensorButton(BUTTON_M_PIN, "MB"), SensorButton(BUTTON_R_PIN, "RB")};


typedef void (*HandlerType)(uint8_t);
uint8_t _menu_state = MENU_DEFAULT_STATE;


void handleDefaultState(uint8_t btn_id) {
  Serial.println(btn_id);
}


HandlerType _handlers[] = {
  handleDefaultState
};


void updateControl() {
  for (uint8_t i = 0; i < BUTTONS_COUNT; ++i) {
    if (_buttons[i].update()) {
      _handlers[i](i);
      #ifdef DEBUG
        _buttons[i].repr();
      #endif
      _buttons[i].resetClicks();
    }
  }
}
