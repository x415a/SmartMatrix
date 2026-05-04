#pragma once

#define ALARM_CLOCK
#define ALARM_CLOCK_COLOR 0xfcc203

#define RTC_UPDATE_INTERVAL (uint32_t) 10 * 60 * 1000  // min * sec * msec
#define RTC_ERROR_SIGNAL 2
#define RTC_SECONDS_SHIFT 10

#include "Arduino.h"
#include "matrix.h"
#include "FastLED.h"
#include "RTClib.h"
#include "storage.h"
#include "define.h"
#include "potentiometer.h"
#include "error.h"

uint32_t getMillisDelay(uint32_t);

bool checkAlarmClock();
bool updateAlarmClock();
void abortAlarmClock();
void updateCommonClock();
uint32_t getClockTimestamp();
void initClock();
