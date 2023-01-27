#ifndef __DEFINE_H
#define __DEFINE_H
#include "Arduino.h"

#define DEBUG
// #define SET_CURRENT_TIME

#define FPS 60
#define RTC_UPDATE_INTERVAL (uint32_t) 10 * 60 * 1000

#define MTX_PIN 10
#define MTX_CLR_ORDER GRB
#define MTX_W 16
#define MTX_H 16
#define MTX_NUM_LEDS MTX_W * MTX_H
#define MTX_CURRENT_LIM 1900
#define MTX_CHIPSET WS2811
#define MTX_MIRROR false
#define MTX_FLIP


#define BUTTONS_COUNT 3
#define BUTTON_L_PIN 3
#define BUTTON_M_PIN 4
#define BUTTON_R_PIN 5
#define BUTTON_CLICK_DELAY 0.030
#define BUTTON_MULTICLICK_DELAY 0.500
#define BUTTON_PRESSED_DELAY 0.200


//#define AUDIO_PIN A1
#define AUDIO_FR 2000
#define AUDIO_SAMPLE (uint16_t) (1 / FPS * AUDIO_FR)
#define AUDIO_THRESHOLD 2
#define AUDIO_AMP_GAIN 1.2
#define AUDIO_MAX_AMP 100
#define AUDIO_MAX_VOLUME 100


#define CLOCK_STYLE_SMALL 1
#define CLOCK_STYLE_BIG 2
#define CLOCK_STYLE_SECONDS 3


#define AMBIENT_LIGHTING_SPEED 600

#endif