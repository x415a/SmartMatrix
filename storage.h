#ifndef __STORAGE_CPP
#define __STORAGE_CPP
#include "Arduino.h"

                    //7-segment digits:  0           1           2           3           4           5           6           7           8           9
uint8_t const CLOCK_NUMBERS[] PROGMEM = {0b01110111, 0b01000100, 0b00111110, 0b01101110, 0b01001101, 0b01101011, 0b01111011, 0b01000110, 0b01111111, 0b01101111};
char const DAYS_OF_THE_WEEK[7][12] PROGMEM = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char const EFFECTS_LIST[][32] PROGMEM = {"Calibrate", "Ambient", "Static Ambient", "Fire"};

#endif