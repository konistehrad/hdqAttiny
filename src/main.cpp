#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "HDQ.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
#define HDQ_WIRE_PIN 3
#define DEBUG_LED 4

union Flags {
  uint16_t full;
  struct {
    union {
      uint8_t raw;
      struct {
        unsigned int otc : 1;
        unsigned int otd : 1;
        unsigned int bathi : 1;
        unsigned int batlow : 1;
        unsigned int chginh : 1;
        unsigned int xchg : 1;
        unsigned int fc : 1;
        unsigned int chg : 1;
      } fields;
    } high;
    union {
      uint8_t raw;
      struct {
        unsigned int ocvtaken : 1;
        unsigned int rsvd0 : 1;
        unsigned int rsvd1 : 1;
        unsigned int cf : 1;
        unsigned int rsvd2 : 1;
        unsigned int soc1 : 1;
        unsigned int socf : 1;
        unsigned int dsg : 1;
      } fields;
    } low;
  } components;
  struct {
    unsigned int otc : 1;
    unsigned int otd : 1;
    unsigned int bathi : 1;
    unsigned int batlow : 1;
    unsigned int chginh : 1;
    unsigned int xchg : 1;
    unsigned int fc : 1;
    unsigned int chg : 1;
    unsigned int ocvtaken : 1;
    unsigned int rsvd0 : 1;
    unsigned int rsvd1 : 1;
    unsigned int cf : 1;
    unsigned int rsvd2 : 1;
    unsigned int soc1 : 1;
    unsigned int socf : 1;
    unsigned int dsg : 1;
  } fields;
};

uint16_t lastBatV = __UINT16_MAX__;
HDQ hdq(HDQ_WIRE_PIN);
SSD1306AsciiWire oled;
bool hot = false;
uint16_t tick = 0;
Flags flags;

uint16_t word_write(uint8_t reg0, uint8_t payload0, uint8_t reg1, uint8_t payload1) {
  uint8_t BYT1, BYT2;
  BYT1 = hdq.write(reg0, payload0);
  BYT2 = hdq.write(reg1, payload1);
  return word(BYT2, BYT1);
}

uint16_t word_read(uint8_t read_reg0, uint8_t read_reg1) {
  uint8_t BYT1, BYT2;
  BYT1 = hdq.read(read_reg0);
  BYT2 = hdq.read(read_reg1);
  return word(BYT2, BYT1);
}

uint16_t word_write_read(uint8_t reg0, uint8_t payload0, uint8_t reg1, uint8_t payload1, uint8_t read_reg0, uint8_t read_reg1) {
  uint8_t BYT1, BYT2;
  BYT1 = hdq.write(reg0, payload0);
  BYT2 = hdq.write(reg1, payload1);
  BYT1 = hdq.read(read_reg0);
  BYT2 = hdq.read(read_reg1);
  return word(BYT2, BYT1);
}

void pulse(uint8_t count, uint8_t target = DEBUG_LED, uint16_t holdTime = 500) {
  for(uint8_t i = 0; i < count; ++i) {
    delay(holdTime);
    digitalWrite(target, HIGH);
    delay(holdTime);
    digitalWrite(target, LOW);
  }
}

void print_battery_stats() {
  uint16_t res;
  oled.home();

  res = word_read(0x0E, 0x0F);
  if(res != __UINT16_MAX__)
  {
    flags.full = res;
  }


  // percentage reg
  oled.set2X();
  res = word_read(0x02, 0x03);
  oled.print(res, DEC); oled.print("% "); 
  if(!flags.fields.dsg) {
    oled.print("(+)");
  }
  oled.clearToEOL(); oled.println();
  oled.set1X();

  // capacity register
  res = word_read(0x04, 0x05);
  oled.print("Cap: "); oled.print(res, DEC); oled.print("mAH");
  oled.clearToEOL(); oled.println();

  // voltage register
  res = word_read(0x08, 0x09);
  if(res < 8000) {
    lastBatV = res;
  }
  if(lastBatV != __UINT16_MAX__) {
    oled.print("BatV: "); oled.print(lastBatV/1000); oled.print("."); oled.print(lastBatV%1000); oled.print("V");
    oled.clearToEOL(); oled.println();
  }

  // current register
  res = word_read(0x0A, 0x0B);
  oled.print("BatC: "); oled.print(res, DEC); oled.print("mA");
  oled.clearToEOL(); oled.println();

  res = word_read(0x0C, 0x0D);
  res -= 2732; // rounded Kelvin -> Celsius conversion
  auto c_integer = res / 10;
  auto c_fractional = res % 10;
  oled.print("Temp: "); oled.print(c_integer, DEC); oled.print("."); oled.print(c_fractional, DEC); oled.print("C (");
  auto f = ( (9 * c_integer) + 160 ) / 5;
  oled.print(f, DEC); oled.print("F)");
  oled.clearToEOL(); oled.println();

  oled.print("Flag: "); oled.print(flags.full, DEC); oled.print(" "); oled.print(flags.full, BIN);
  oled.clearToEOL(); oled.println();
}

void setup() {
  pinMode(DEBUG_LED, OUTPUT);
  pulse(3, DEBUG_LED, 100);
  Wire.begin();
  oled.begin(&Adafruit128x64, I2C_ADDRESS); 
  oled.setFont(System5x7);
  oled.home();
  oled.clear();
  word_write(0x00, 0x01, 0x01, 0x00);
}

void loop() {
  auto start_t = millis();

  tick += 1;
  hot = !hot;
  
  print_battery_stats(); 
  oled.setCursor( oled.displayWidth() - oled.charWidth('.'), oled.displayRows() - 1 );
  oled.print(hot ? "." : " ");

  auto end_t = millis();
  auto delta_t = end_t - start_t;
  if(start_t < end_t && delta_t > 100) {
    delay(1000 - delta_t);
  }
}
