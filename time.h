#ifndef __TIME_H
#define __TIME_H
#include "Arduino.h"


void frameTick();
void initTime();
uint8_t getFps(); 
uint32_t getUnixTime();
uint32_t getMillisDelay(uint32_t);
float getCurrentTimeSeconds();

#endif
