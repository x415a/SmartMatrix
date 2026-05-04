#pragma once

#include "Arduino.h"

struct Menu;

Menu* getCurrentMenu();
void setCurrentMenu(uint8_t);
