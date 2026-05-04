#pragma once

#define ERROR_SIGNAL_DUR_MS 200
#define ERROR_SIGNAL_DELAY_MS 1000

#define ERROR_STATES_MACHINE 5

#include "Arduino.h"
#include "define.h"

void initErrorIndicator();
void emitError(uint8_t, const char (&)[8]);
