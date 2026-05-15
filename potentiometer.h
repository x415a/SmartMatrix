#pragma once

#include "Arduino.h"
#include "define.h"
#include "clock.h"
#include "connections.h"

#define POTENTIOMETER_MIN 5
#define POTENTIOMETER_MAX 1000
#define POTENTIOMETER_TRESHOLD 3
#define POTENTIOMETER_UPD_FREQ 50
#define POTENTIOMETER_UPD_BUF 4
#ifdef DEBUG
// #define POTENTIOMETER_DEBUG
#endif


void initPotentiometer();
int32_t getPtInterpolatedValue(int32_t, int32_t);
int32_t interpolatePtValue(int32_t, int32_t, int32_t);
uint16_t getPtValue();
void checkPtUpdates();
