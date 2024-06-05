#include "mbed.h"
#include "stdlib.h"
#include <cstdint>

// pin usage as follow:
// LCD          VCC GND  CS  RESET DC/RS  SDI/MOSI  SCK  SDO/MISO  LED
// mbed                  D10,  D9,  D8,    D11,     D13, D12,      3V3

SPI spi(D11, D12, D13); // mosi, miso, sclk
DigitalOut dc(D8);      // data or command mode
DigitalOut reset(D9);   // reset signal
DigitalOut cs(D10);     // chip select

#include "lcd.h"
#include "mbed.h"
// LCD           VCC GND  CS  RESET DC/RS  SDI/MOSI  SCK  SDO/MISO  LED
// mbed                   D10,  D9,  D8,    D11,     D13, D12,      3V3

// CS  RESET DC/RS  SDI/MOSI  SCK  SDO/MISO
LCD lcd(D10, D9, D8, D11, D13, D12);

int main() {
  printf("Initializing LCD\n");
  lcd.InitLCD();
  lcd.LCD_Clear(0x0);
  printf("Writing a few empty rectangles on LCD\n");
  for (int i = 0; i < 10; i++) {
    int x = 280-i * (20 + i);
    int y = i * 40;
    int w = 20 + i;
    int h = 40;
    int c = rand() % 65535;
    lcd.Rect(x, y, w, h, c);
    ThisThread::sleep_for(1s);
  }
  printf("Writing a few filled rectangles on LCD\n");
  for (int i = 0; i < 10; i++) {
    int x = i * (20 + i);
    int y = i * 40;
    int w = 20 + i;
    int h = 40;
    int c = rand() % 65535;
    lcd.FillRectangle(x, y, w, h, c);
    ThisThread::sleep_for(1s);
  }
}