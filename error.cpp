#include "error.h"

void initErrorIndicator() {
  pinMode(LED_SIGNAL_PIN, OUTPUT);
  digitalWrite(LED_SIGNAL_PIN, LOW);
}

void emitError(uint8_t count, const char (&err)[8]) {
  for (;;) {
    #ifdef DEBUG
    if (err != 0) {
      Serial.print("ERR: ");
      Serial.println(err);
    }
    #endif

    for (uint8_t i = 0, st = false; i < count * 2; ++i) {
      digitalWrite(LED_SIGNAL_PIN, (st = !st));
      delay(ERROR_SIGNAL_DUR_MS);
    }
    delay(ERROR_SIGNAL_DELAY_MS);
  }
}
