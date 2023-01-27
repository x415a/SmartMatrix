#ifndef __TIME_H
#define __TIME_H
#include "Arduino.h"


void frameTick();
uint8_t getFps(); 
uint32_t getUnixTime();
float getCurrentTimeSeconds();

#endif
