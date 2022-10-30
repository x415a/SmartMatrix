#define DEBUG
//#define SET_CURRENT_TIME

#define FPS 60
#define RTC_DAY_UPDATES (6 * 60 * 24)

#define MTX_PIN 10
#define MTX_CLR_ORDER GRB
#define MTX_W 16
#define MTX_H 16
#define MTX_NUM_LEDS MTX_W * MTX_H
#define MTX_CURRENT_LIM 1900
#define MTX_CHIPSET WS2811
#define MTX_MIRROR false
#define MTX_FLIP


#define BUTTONS_COUNT 4
#define BUTTON_L_PIN 3
#define BUTTON_M_PIN 4
#define BUTTON_R_PIN 5
#define BUTTON_CLICK_DELAY 0.030
#define BUTTON_MULTICLICK_DELAY 0.500
#define BUTTON_PRESSED_DELAY 0.200


#define AUDIO_PIN A1
#define AUDIO_FR 2000
#define AUDIO_SAMPLE (int) (1 / FPS * AUDIO_FR)
#define AUDIO_THRESHOLD 2
#define AUDIO_AMP_GAIN 1.2
#define AUDIO_MAX_AMP 100
#define AUDIO_MAX_VOLUME 100

#define CLOCK_SIZE_SMALL 5
#define CLOCK_SIZE_MEDIUM 6
#define CLOCK_SIZE_BIG 7
//#define USE_ONLY_DEFAULT_CLOCK_BLINKER
#define CLOCK_BLINKER_TRACE_LEN 8


#define AMBIENT_LIGHTING_SPEED 600


#include "FastLED.h"
#include "GyverTimers.h"
#include "RTClib.h"


void initMatrix();
void initTime();
void initEffects();
void initMicrophone();

void fillMatrix(CRGB);
void fillMatrix(uint8_t, uint8_t, uint8_t, uint8_t, CRGB);
void setMatrixLED(uint8_t, uint8_t, CRGB);
void refreshMatrix();
void clearMatrix(bool=false);

void fpsTick();
uint8_t getFps(); 
uint32_t getUnixTime();
float getCurrentTimeSeconds();

uint8_t getAudioVolume();
void printAudioInfo();

void updateEffects();
void matrixCalibrate();
String* getEffectsList();


void drawClock(DateTime, CRGB);
void setClockHeight(uint8_t);

void updateControl();


void setup() {
  #ifdef DEBUG
    Serial.begin(57600);
  #endif

  randomSeed(analogRead(0));

  initTime();
  initMatrix();
  initEffects();
}


void loop() {
  updateControl();
  //updateEffects();
  drawClock(DateTime(getUnixTime()), CRGB::Green);
  refreshMatrix();
  clearMatrix();
  fpsTick();
}
