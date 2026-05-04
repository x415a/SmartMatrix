#pragma once

#include "Arduino.h"
#include "define.h"
#include "controls.h"

#ifdef DEBUG
// #define ENABLE_BUTTON_DEBUG
#endif

#define BUTTON_STATE_BIT        0b1
#define BUTTON_MID_STATE_BIT    0b10
#define BUTTON_DOUBLE_CLICK_BIT 0b100
#define BUTTON_LONG_PRESS_BIT   0b1000
#define BUTTON_DEF_STATE_BIT    0b10000
#define BUTTON_DELAY_WAIT_BIT   0b100000
#define BUTTON_READY_BIT        0b1000000

#define EVENT_FLAG_CLICK 1
#define EVENT_FLAG_DOUBLE_CLICK 2
#define EVENT_FLAG_LONG_PRESS 3

#define BUTTON_RATTLING_DELAY_MS 25
#define BUTTON_DOUBLE_CLICK_DELAY_MS 150
#define BUTTON_LONG_PRESS_DELAY_MS 500

void initButtons();
void updateButtons();
