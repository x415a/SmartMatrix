#include "RTClib.h"
#include "define.h"
#include "matrix.h"
#include "time.h"
#include "storage.h"
#include "clock.h"


void _drawClockNumSegment(uint8_t x, uint8_t y, uint8_t l, uint8_t h, uint8_t seg, CRGB &color) {
  if (seg % 2) {
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


void _drawClockNumber(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t num, CRGB &color) {
  num = pgm_read_byte(&CLOCK_NUMBERS[num]);

  for (uint8_t seg = 0; seg < 7; ++seg) {
    if ((num >> seg) & 1)
      _drawClockNumSegment(x, y, w, h, seg, color);
  }
}


void _drawPair(uint8_t a, uint8_t b, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t space, CRGB &color) {
  _drawClockNumber(x, y, w, h, a, color);
  _drawClockNumber(x + w + space, y, w, h, b, color);
}


void _drawSecondsIndicator(uint8_t h_end_x, uint8_t h_end_y, CRGB &color) {
  #ifndef USE_ONLY_DEFAULT_CLOCK_BLINKER
    if (MTX_H - (5 + 1) * 2 > 0) {
      float cur_time = getCurrentTimeSeconds();
      uint8_t pos = ((uint8_t) ((MTX_W - 1) * (cur_time - (uint32_t) cur_time)) + 1) % MTX_W;
      if ((uint32_t) cur_time % 2) {
        pos = MTX_W - pos - 1;
      }
      setMatrixLED(pos, 0, color);
      return;
    }
  #endif
  if ((uint32_t) getCurrentTimeSeconds() % 2) {
    setMatrixLED(h_end_x, h_end_y, color);  
  }
}

void drawClock(DateTime time, CRGB color) {
  uint8_t hr = time.hour(), mn = time.minute(), numbers[] = {hr / 10 % 10, hr % 10, mn / 10 % 10, mn % 10};
  uint8_t h = 5, w = h / 2 + h % 2,
          space = max(0, (MTX_W - 4 * w) / 3), hx = 0, hy = 0;

  if (space == 0) {
    uint8_t x = (MTX_W - (w + 1) * 2) / 2, y = (MTX_H - (h + 1) * 2) / 2;
    _drawPair(numbers[0], numbers[1], x, y, w, h, 2, color);
    hx = x + (w + 1) * 2;
    hy = y + h - 1;
    _drawPair(numbers[2], numbers[3], x, y + h + 2, w, h, 2, color);
  } else {
    uint8_t x = (MTX_W - (w + 1) * 4) / 2, y = 1;
    _drawPair(numbers[0], numbers[1], x, y, w, h, 1, color);
    hx = x + w * 2 + 1;
    hy = y + h - 1;
    _drawPair(numbers[2], numbers[3], x + w * 2 + 3, y, w, h, 1, color);
  }

  _drawSecondsIndicator(hx, hy, color);
}


void updateClock() {
  clearMatrix();
  drawClock(DateTime(getUnixTime()), CRGB::Yellow);
  refreshMatrix();
}
