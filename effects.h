#ifndef __EFFECTS_H
#define __EFFECTS_H
#include "Arduino.h"

typedef void (*CallbackFunc)();

void sunriseEffect(bool=false);
void ambientLightingEffect(bool=false);

void updateEffects();
void matrixCalibrate();
String* getEffectsList();

#endif
