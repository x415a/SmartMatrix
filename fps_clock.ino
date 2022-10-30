#define RTC_UPDATE_INTERVAL ((uint32_t)24 * 3600 * 1000) / RTC_DAY_UPDATES


uint32_t _current_rtc_time = 0, _start_rtc_time = 0, _last_frame = 0, _last_sync = 0, _frame_delta = 0;
RTC_DS1307 _rtc;


void _synchronizeTime() {
  _current_rtc_time = _rtc.now().unixtime();
  _last_sync = (_current_rtc_time - _start_rtc_time) * 1000;

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
  static const uint16_t frame_time = 1000 / FPS;
  _frame_delta = millis() - _last_frame;

  if (_frame_delta < 0) {
    _last_frame = _frame_delta = 0;
    _synchronizeTime();
    return;
  }

  if (_last_frame - _last_sync > RTC_UPDATE_INTERVAL) {
    Serial.print(_last_frame);
    Serial.print(' ');
    Serial.println(_last_sync);
    _synchronizeTime();
  }

  if (_frame_delta < frame_time)
    delay(frame_time - _frame_delta);
  _last_frame = millis();
}


uint8_t getFps() {
  return (_frame_delta > 0) ? 1000 / _frame_delta : 0;
}


uint32_t getUnixTime() {
  return _current_rtc_time + (_last_frame - _last_sync) / 1000;
}


float getCurrentTimeSeconds() {
  return (_current_rtc_time - _start_rtc_time) + (float)(_last_frame - _last_sync) / 1000;
}
