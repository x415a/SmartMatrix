#include "define.h"
#include "RTClib.h"
#include "Arduino.h"


struct TimeHandler
{
  uint32_t rtc_time = 0, rtc_start = 0, last_frame = 0, last_sync = 0, frame_dt = 0;
  RTC_DS1307 _rtc;

  void synchronizeTime() {
    rtc_time = _rtc.now().unixtime();
    last_sync = millis();

    #ifdef DEBUG
      Serial.print("RTC synchronization: ");
      Serial.println(DateTime(rtc_time).timestamp());
    #endif
  }

  void init() {
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

    synchronizeTime();
    rtc_start = rtc_time;
    last_frame = frame_dt = 0;
  }

  void tick() {
    #define FTIME 1000 / FPS
    frame_dt = millis() - last_frame;

    if (frame_dt < 0) {
      last_frame = frame_dt = 0;
      synchronizeTime();
      return;
    }

    if (frame_dt < FTIME)
      delay(FTIME - frame_dt);
    last_frame = millis();

    if (last_frame - last_sync > RTC_UPDATE_INTERVAL) {
      synchronizeTime();
  }
}

} _TIME_HANDLER;


void initTime() {
  _TIME_HANDLER.init();
}


uint8_t getFps() {
  return (_TIME_HANDLER.frame_dt > 0) ? 1000 / _TIME_HANDLER.frame_dt : 0;
}


uint32_t getUnixTime() {
  return _TIME_HANDLER.rtc_time + ((_TIME_HANDLER.last_frame < _TIME_HANDLER.last_sync) ? 0 : (_TIME_HANDLER.last_frame - _TIME_HANDLER.last_sync) / 1024);
}


float getCurrentTimeSeconds() {
  return millis() / 1024.0;
}


void frameTick() {
  _TIME_HANDLER.tick();
}
