#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Digital inputs for the two LDR circuits
const int sensorPin[2] = {10, 11};

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(sensorPin[0], INPUT);
  pinMode(sensorPin[1], INPUT);

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("SYSTEM READY");
  delay(1500);  // briefly show ready, then loop will take over
}

void loop() {
  // Read both LDRs – covered (dark) = LOW, uncovered (light) = HIGH
  bool ldr1Active = (digitalRead(sensorPin[0]) == LOW);
  bool ldr2Active = (digitalRead(sensorPin[1]) == LOW);

  // 1. IDLE STATE – both sensors in the same state (both covered or both uncovered)
  if (ldr1Active == ldr2Active) {
    lcd.setCursor(0, 0);
    lcd.print("  AWAITING LDR  ");
    lcd.setCursor(0, 1);
    lcd.print("   SENSORS...   ");
  }
  
  // 2. Only LDR1 covered → scroll WELCOME message
  else if (ldr1Active) {
    lcd.clear();
    String text = "WELCOME TO THE PUP-CPE DEPARTMENT";
    String padded = "                " + text + "                ";
    int totalLen = padded.length();

    for (int i = totalLen - 16; i >= 0; i--) {
      // Abort if LDR1 is no longer covered (light is back)
      if (digitalRead(sensorPin[0]) == HIGH) {
        lcd.clear();
        break;   // exit the scroll loop -> loop() restarts -> idle
      }
      // Abort if the other LDR suddenly becomes covered
      if (digitalRead(sensorPin[1]) == LOW) {
        lcd.clear();
        break;
      }
      lcd.setCursor(0, 0);
      lcd.print(padded.substring(i, i + 16));
      delay(300);
    }
  }
  
  // 3. Only LDR2 covered → scroll GOODBYE message
  else if (ldr2Active) {
    lcd.clear();
    String text = "GOODBYE, SEE YOU AGAIN NEXT TIME!!!!";
    String padded = "                " + text + "                ";
    int totalLen = padded.length();

    for (int i = totalLen - 16; i >= 0; i--) {
      // Abort if LDR2 is no longer covered
      if (digitalRead(sensorPin[1]) == HIGH) {
        lcd.clear();
        break;
      }
      // Abort if LDR1 suddenly becomes covered
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