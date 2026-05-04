#pragma once

#include "connections.h"

#define DEBUG
#define BOD_RATE 38400
#define STRMACRO(A) __XSTRMACRO(A)
#define __XSTRMACRO(A) #A

#define ALARM_CLOCK_HOUR 6
#define ALARM_CLOCK_MIN 50
#define ALARM_CLOCK_DUR 30
#define ALARM_CLOCK_STAY_AFTER_DUR 15

// #define RTC_SET_CURRENT_TIME
// #define REWRITE_STORAGE
