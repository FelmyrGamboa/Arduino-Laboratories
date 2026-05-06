#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Peace Sign (Circular with internal lines)
//byte peaceL[8] = {0b00111,0b01000,0b10010,0b10101,0b10010,0b01001,0b00111,0b00000};
//byte peaceR[8] = {0b11100,0b00010,0b01001,0b10101,0b01001,0b10010,0b11100,0b00000};
byte peaceL[8] = {0b00000,0b00000,0b00001,0b00001,0b00001,0b00001,0b00000,0b00000};
byte peaceM[8] = {0b01110,0b10101,0b00100,0b00100,0b00100,0b01010,0b10001,0b01110};
byte peaceR[8] = {0b00000,0b00000,0b10000,0b10000,0b10000,0b10000,0b00000,0b00000};


// Pointed Heart (Classic sharp bottom)
byte heartL[8] = {0b00110,0b01111,0b11111,0b11111,0b01111,0b00111,0b00011,0b00001};
byte heartR[8] = {0b01100,0b11110,0b11111,0b11111,0b11110,0b11100,0b11000,0b10000};

// Pointed Code Brackets < >
byte codeL[8]  = {0b00001,0b00010,0b00100,0b01000,0b00100,0b00010,0b00001};
byte codeR[8]  = {0b10000,0b01000,0b00100,0b00010,0b00100,0b01000,0b10000}; 

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, peaceL);
  lcd.createChar(6, peaceM);
  lcd.createChar(1, peaceR);
  lcd.createChar(2, heartL);
  lcd.createChar(3, heartR);
  lcd.createChar(4, codeL);
  lcd.createChar(5, codeR);

  lcd.clear();
  
  // Top Row: Dual-grid Icons
  lcd.setCursor(1, 0);  lcd.write(0); lcd.write(6); lcd.write(1); 
  lcd.setCursor(7, 0);  lcd.write(2); lcd.write(3); 
  lcd.setCursor(12, 0); lcd.write(4); lcd.write(5); 

  // Bottom Row: Exact text centering
  lcd.setCursor(0, 1);  lcd.print("Peace");
  lcd.setCursor(6, 1);  lcd.print("Love");
  lcd.setCursor(11, 1); lcd.print("Code");
}

void loop() {}