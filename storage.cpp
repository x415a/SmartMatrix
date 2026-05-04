#include "storage.h"


void showStorage() {
  Serial.println("=== STORAGE ===");

  Serial.print("CLOCK_NUMBERS: ");
  Serial.println((uintptr_t)CLOCK_NUMBERS, HEX);
  // 0xb0

  Serial.print("DAYS_OF_THE_WEEK: ");
  Serial.println((uintptr_t)DAYS_OF_THE_WEEK, HEX);
  // 0x82

  Serial.print("EFFECTS_LIST: ");
  Serial.println((uintptr_t)EFFECTS_LIST, HEX);
  // 0x64
}