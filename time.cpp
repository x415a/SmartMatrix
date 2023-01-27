#include "define.h"
#include "RTClib.h"
#include "Arduino.h"

uint32_t _current_rtc_time = 0, _start_rtc_time = 0, _last_frame = 0, _last_sync = 0, _frame_delta = 0;
RTC_DS1307 _rtc;


void _synchronizeTime() {
  _current_rtc_time = _rtc.now().unixtime();
  _last_sync = millis();

  #ifdef DEBUG
    Serial.print("RTC synchronization: ");
    Serial.println(DateTime(_current_rtc_time).timestamp());
  #endif
}


void initTime() {
  Serial.println(RTC_UPDATE_INTERVAL);
  if (!_rtc.begin()) {
    #ifdef DEBUG
      Serial.println("Couldn't find RTC");
      Serial.flush();
    #endif
    exit(0);
  }

  #ifdef SET_CURRENT_TIME
    _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  #endif

  _synchronizeTime();
  _start_rtc_time = _current_rtc_time;
  _last_frame = _frame_delta = 0;
}


void fpsTick() {
  #define FTIME 1000 / FPS
  _frame_delta = millis() - _last_frame;

  if (_frame_delta < 0) {
    _last_frame = _frame_delta = 0;
    _synchronizeTime();
    return;
  }

  if (_frame_delta < FTIME)
    delay(FTIME - _frame_delta);
  _last_frame = millis();

  if (_last_frame - _last_sync > RTC_UPDATE_INTERVAL) {
    _synchronizeTime();
  }
}


uint8_t getFps() {
  return (_frame_delta > 0) ? 1000 / _frame_delta : 0;
}


uint32_t getUnixTime() {
  return _current_rtc_time + ((_last_frame > _last_sync) ? (_last_frame - _last_sync) / 1024 : 0);
}


float getCurrentTimeSeconds() {
  return millis() / 1024.0;
}
