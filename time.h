#ifndef __TIME_H
#define __TIME_H
#include "Arduino.h"


void fpsTick();
uint8_t getFps(); 
uint32_t getUnixTime();
float getCurrentTimeSeconds();

#endif