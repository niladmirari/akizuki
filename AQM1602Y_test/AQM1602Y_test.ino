#include <Wire.h>

#define AQM1602Y_ADDR 0x3e

int writeCommand(byte command) {
  Wire.beginTransmission(AQM1602Y_ADDR);
  Wire.write(0x0);
  Wire.write(command);
  return Wire.endTransmission();
}

int writeData(byte data) {
  Wire.beginTransmission(AQM1602Y_ADDR);
  Wire.write(0b01000000);
  Wire.write(data);
  return Wire.endTransmission();
}

void initLcd() {
  delay(100);
  writeCommand(0x38); // Function Set
  delay(1);
  writeCommand(0x39); // Unction Set
  delay(1);
  writeCommand(0x14); // BS=0:1/5 BIAS; F2 F1 F0: 100
  delay(1);
  writeCommand(0x73); // Contrast set
  delay(1);
  writeCommand(0x5e); // Icon on, booster circuit on
  delay(1);
  writeCommand(0x6c); // Follower circuit on
  delay(1);
  writeCommand(0x0c); // Entire display on
  delay(1);
  writeCommand(0x01); // Clear display
  delay(1);
  writeCommand(0x06); // Entry Mode Set, increment
  delay(1);
}

void setLcdCursor(int x, int y) {
  y *= 0x40;
  y += x;
  y |= 0b10000000;

  writeCommand(y);
}

int writeChar(byte c) {
  if (c < 0x20) {
    return -1; // error
  }

  if (0x7f < c) {
    return -1; // error
  }
  return writeData((byte)c);
}

void writeLine(char *s) {
  int len = strlen(s);
  int ret = 0;
  
  for (int i = 0;i < len; i++) {
    writeChar(s[i]);
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin(32,33);
  initLcd();

  // コレハテスト
  writeData(0xba);
  writeData(0xda);
  writeData(0xca);
  writeData(0xc3);
  writeData(0xbd);
  writeData(0xc4);
  //writeData(0xa0);
  //writeData(0x62);
  //writeData(0x79);
  //writeData(0xa0);
  //writeData(0xdb);
  //writeData(0xdd);
    

  writeCommand(0b10001111);
  writeChar('b');
  
  //writeCommand(0b11000000);
  setLcdCursor(1, 1);
  writeLine("Hello World");

  delay(50000);
}

void loop() {
  static int counter = 0;
  setLcdCursor(0, 1);
  char countLine[17] = {0};
  snprintf(countLine, 16, "%03d", counter);
  writeLine(countLine);
  counter++;
  delay(100);
}
