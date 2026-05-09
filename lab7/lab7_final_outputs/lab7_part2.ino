#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensorPin[2] = {10, 11};

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(sensorPin[0], INPUT);
  pinMode(sensorPin[1], INPUT);

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("SYSTEM READY");
  delay(1500);
}

void loop() {
  bool ldr1Active = (digitalRead(sensorPin[0]) == LOW);
  bool ldr2Active = (digitalRead(sensorPin[1]) == LOW);

  if (ldr1Active == ldr2Active) {
    lcd.setCursor(0, 0);
    lcd.print("  AWAITING LDR  ");
    lcd.setCursor(0, 1);
    lcd.print("   SENSORS...   ");
  }
  
  else if (ldr1Active) {
    lcd.clear();
    String text = "WELCOME TO THE PUP-CPE DEPARTMENT";
    String padded = "                " + text + "                ";
    int totalLen = padded.length();

    for (int i = totalLen - 16; i >= 0; i--) {
      if (digitalRead(sensorPin[0]) == HIGH) {
        lcd.clear();
        break;
      }
      if (digitalRead(sensorPin[1]) == LOW) {
        lcd.clear();
        break;
      }
      lcd.setCursor(0, 0);
      lcd.print(padded.substring(i, i + 16));
      delay(300);
    }
  }
  
  else if (ldr2Active) {
    lcd.clear();
    String text = "GOODBYE, SEE YOU AGAIN NEXT TIME!!!!";
    String padded = "                " + text + "                ";
    int totalLen = padded.length();

    for (int i = totalLen - 16; i >= 0; i--) {
      if (digitalRead(sensorPin[1]) == HIGH) {
        lcd.clear();
        break;
      }
      if (digitalRead(sensorPin[0]) == LOW) {
        lcd.clear();
        break;
      }
      lcd.setCursor(0, 0);
      lcd.print(padded.substring(i, i + 16));
      delay(300);
    }
  }
}