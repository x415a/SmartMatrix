#include "clock.h"


static RTC_DS1307 rtc;
static CRGB const alarm_color = CRGB(ALARM_CLOCK_COLOR);
static uint8_t alarm_started = false, alarm_aborted = false;
static uint16_t start_time = ALARM_CLOCK_HOUR * 60 + ALARM_CLOCK_MIN - ALARM_CLOCK_DUR;


void _drawClockNumSegment(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, CRGB const &);
void _drawClockNumber(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, CRGB const &);
void _drawPair(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, CRGB const &);
void _drawSecondsIndicator(uint8_t, uint8_t, uint8_t, CRGB const &);
void _drawClock(CRGB const &);
uint16_t _getCurrentDayMinutes();


void initClock() {
  #ifdef DEBUG
  Serial.println("RTC: init");
  #endif

  if (!rtc.begin()) {
    emitError(RTC_ERROR_SIGNAL, "RTC_INI");
  }

  #ifdef RTC_SET_CURRENT_TIME
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)) + TimeSpan(RTC_SECONDS_SHIFT));
  #ifdef DEBUG
  Serial.println("RTC: synced");
  #endif
  #endif

  #ifdef DEBUG
  Serial.print("RTC: ");
  Serial.println(rtc.now().timestamp());

  #ifdef ALARM_CLOCK
  Serial.println("ALR: enabled " STRMACRO(ALARM_CLOCK_HOUR) "h " STRMACRO(ALARM_CLOCK_MIN) "m");
  #endif
  #endif
}


void abortAlarmClock() {
  alarm_aborted = true;
  alarm_started = false;

  #ifdef DEBUG
  Serial.println("ALR: aborted");
  #endif
}


uint32_t getMillisDelay(uint32_t time) {
  return millis() - time;
}


bool checkAlarmClock() {
  uint16_t cur_min = _getCurrentDayMinutes();
  if (alarm_aborted && cur_min < start_time + ALARM_CLOCK_DUR) {
    return false;
  }
  if (alarm_started) {
    return true;
  }

  if (start_time <= cur_min && cur_min <= start_time + ALARM_CLOCK_DUR) {
    #ifdef DEBUG
    Serial.println("ALR: started");
    #endif
    return alarm_started = true;
  }
  return false;
}


bool updateAlarmClock() {
  static uint16_t last_update = UINT16_MAX;
  uint16_t cur_min = _getCurrentDayMinutes();

  if (last_update == cur_min) {
    return true;
  }
  last_update = cur_min;

  if (cur_min <= start_time + ALARM_CLOCK_DUR) {
    setBrightness((uint32_t) (cur_min - start_time) * 255 / ALARM_CLOCK_DUR);
    fillMatrix(alarm_color);
    refreshMatrix();
  } else if (cur_min - start_time - ALARM_CLOCK_DUR <= ALARM_CLOCK_STAY_AFTER_DUR) {
  } else {
    resetMatrix();

    #ifdef DEBUG
    Serial.println("ALR: ended");
    #endif

    return alarm_started = false;
  }
  return true;
}


void updateCommonClock() {
  uint8_t brt = getPtInterpolatedValue(0, 255);
  if (brt != getBrightness()) {
    setBrightness(brt);
    refreshMatrix();
  }
  _drawClock(CRGB::Green);
}


uint32_t getClockTimestamp() {
  return rtc.now().secondstime();
}


void _drawClockNumSegment(uint8_t x, uint8_t y, uint8_t l, uint8_t h, uint8_t seg, CRGB const &color) {
  if (seg & 1) {
    uint8_t ofs = min(h - 1 + h % 2, (seg - 1) / 2 * (h / 2));
    for (uint8_t i = 0; i < l; ++i) {
      setMatrixLED(x + i, y + ofs, color);
    }
  } else {
    uint8_t ofs_x = (seg / 2 % 2) * (l - 1), ofs_y = (seg / 4) * (h / 2);
    for (uint8_t i = 0; i < l; ++i) {
      setMatrixLED(x + ofs_x, y + ofs_y + i, color);
    }
  }
}


void _drawClockNumber(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t num, CRGB const &color) {
  #ifndef DEBUG
  num = pgm_read_byte(&CLOCK_NUMBERS[num]);
  #else
  num = CLOCK_NUMBERS[num];
  #endif

  for (uint8_t seg = 0; seg < 7; ++seg) {
    if ((num >> seg) & 1)
      _drawClockNumSegment(x, y, w, h, seg, color);
  }
}


void _drawPair(uint8_t a, uint8_t b, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t space, CRGB const &color) {
  _drawClockNumber(x, y, w, h, a, color);
  _drawClockNumber(x + w + space, y, w, h, b, color);
}


void _drawSecondsIndicator(uint8_t h_end_x, uint8_t h_end_y, uint8_t seconds, CRGB const &color) {
  // #ifndef USE_ONLY_DEFAULT_CLOCK_BLINKER
  //   if (MTX_H - (5 + 1) * 2 > 0) {
  //     float cur_time = getCurrentTimeSeconds();
  //     uint8_t pos = ((uint8_t) ((MTX_W - 1) * (cur_time - (uint32_t) cur_time)) + 1) % MTX_W;
  //     if ((uint32_t) cur_time % 2) {
  //       pos = MTX_W - pos - 1;
  //     }
  //     setMatrixLED(pos, 0, color);
  //     return;
  //   }
  // #endif
  if (seconds) {
    setMatrixLED(h_end_x, h_end_y, color);
  }
}

void _drawClock(CRGB const &color) {
  static uint8_t last_draw = 0;

  if (last_draw == getMillisDelay(0) / 1024 % 2) {
    return;
  }

  last_draw = getMillisDelay(0) / 1024 % 2;
  clearMatrix();

  uint16_t cur_min = _getCurrentDayMinutes();
  uint8_t hr = cur_min / 60,
  mn = cur_min - hr * 60,
  h = 5,
  w = h / 2 + h % 2,
  space = max(0, (MTX_W - 4 * w) / 3),
  hx = 0,
  hy = 0;

  if (space == 0) {
    uint8_t x = (MTX_W - (w + 1) * 2) / 2,
    y = (MTX_H - (h + 1) * 2) / 2;

    _drawPair(hr / 10 % 10, hr % 10, x, y, w, h, 2, color);
    hx = x + (w + 1) * 2;
    hy = y + h - 1;
    _drawPair(mn / 10 % 10, mn % 10, x, y + h + 2, w, h, 2, color);
  } else {
    uint8_t x = (MTX_W - (w + 1) * 4) / 2,
    y = 1;

    _drawPair(hr / 10 % 10, hr % 10, x, y, w, h, 1, color);
    hx = x + w * 2 + 1;
    hy = y + h - 1;
    _drawPair(mn / 10 % 10, mn % 10, x + w * 2 + 3, y, w, h, 1, color);
  }

  _drawSecondsIndicator(hx, hy, last_draw, color);

  refreshMatrix();
}


uint16_t _getCurrentDayMinutes() {
  DateTime current_time = DateTime(rtc.now().unixtime());
  return (uint16_t) current_time.hour() * 60 + current_time.minute();
}
