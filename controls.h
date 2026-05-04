#pragma once

#define BUTTON_1_ID 1


// MAX ID: 63
#define STATE_NULL 0
#define STATE_BLACKOUT 1
#define STATE_AMBIENT 2
#define STATE_AMBIENT_SET_BRT 3
#define STATE_STATIC_COLOR 4
#define STATE_CLOCK 5
#define STATE_ALARM_CLOCK 6
#define STATE_ALARM_CLOCK_ABORTED 7
#define STATE_ALARM_CLOCK_SET_HOURS 8
#define STATE_ALARM_CLOCK_SET_MINUTES 9

#define STATE_IN_MENU 19
#define STATE_SET_NEXT_MENU_ITEM 20
#define STATE_SET_PREV_MENU_ITEM 21
#define STATE_SELECT_MENU_ITEM 22
#define STATE_EXIT_MENU 23

#define STATE_OPEN_MAIN_MENU 30
#define STATE_OPEN_EFFECTS_MENU 31
#define STATE_OPEN_SETTINGS_MENU 32
#define STATE_OPEN_EFF_SET_MENU 33


#include "Arduino.h"
#include "HardwareSerial.h"
#include "define.h"
#include "button.h"
#include "clock.h"
#include "error.h"
#include "effects.h"


void initControls();
void readyBlink();
void powerOn();
void powerOff();
void updateStateMachine();
void setCurrentState(uint8_t);
void handleButtonClickEvent(uint8_t, uint8_t);
void enableCommonClock();

template <uint8_t N>
bool _changeCurrentState(uint8_t (&)[N], uint8_t, uint8_t);
