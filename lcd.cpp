#include "lcd.h"

static LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);


void initLCD() {
  lcd.begin(16, 2);
  lcd.print("Hello world!");
}