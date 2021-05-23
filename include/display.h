#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
void dispayStats(void) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.print(F("T0 "));
    display.print(cleanTempWasser0);
    display.setCursor(128/2, 0);
    display.print(F("T1 "));
    display.println(cleanTempWasser1);
    display.setCursor(10, 10);
    display.print(F("exh. "));
    display.print(int(TempFlame0));
    display.setCursor(128/2, 12);
    display.print(F("1 "));
    display.println(Board_Voltage);
    display.setCursor(10, 24);
    display.print(F("!!"));
    display.println(major_fail);
    display.setCursor(128/3, 24);
    display.print(F("P "));
    display.println(MC_Voltage);
    display.setCursor((128/3)*2, 24);
    display.print(F("F "));
    display.println(fan_state);
    display.display();
}
void testscrolltext(void) {
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}