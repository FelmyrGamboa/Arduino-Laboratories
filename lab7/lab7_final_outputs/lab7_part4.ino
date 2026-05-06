#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int buzzerPin = 2;
const int ledPins[6] = {22, 24, 26, 28, 30, 32};  // 6 LEDs

enum ProgramState {
  ENTER_NAME,
  ENTER_PASSWORD,
  ACCESS_GRANTED,
  ACCESS_DENIED
};
ProgramState progState = ENTER_NAME;

String userName = "";
String inputBuffer = "";
int attempts = 0;
const String correctPassword = "1234";

unsigned long lastBlinkTime = 0;
bool blinkState = false;
const unsigned long blinkInterval = 500;

const int correctMelody[] = {262, 294, 330, 349, 392, 440, 494, 523};
const int correctDurations[] = {150, 150, 150, 150, 150, 150, 150, 300};
const int melodyLength = 8;

void updateDisplay();
void processInput();
void grantAccess();
void denyAccess();
void playToneSequence(const int* notes, const int* durations, int len);
void runLEDsLeftRight(int times);
void wrongPattern();

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  Serial.println("Enter your name and press Enter:");

  pinMode(buzzerPin, OUTPUT);
  for (int i = 0; i < 6; i++) pinMode(ledPins[i], OUTPUT);

  // Show initial prompt (like original)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER YOUR NAME:");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void loop() {
  // NAME ENTRY – from Serial Monitor, displays on LCD instantly
  if (progState == ENTER_NAME) {
    while (Serial.available() > 0 && progState == ENTER_NAME) {
      char key = Serial.read();

      if (key >= 32 && key <= 126) {               // printable ASCII
        inputBuffer += key;
        // Update only the second line – no full lcd.clear()
        lcd.setCursor(0, 1);
        lcd.print(inputBuffer);
        // Erase any leftover characters after the name
        lcd.print("                ");             // 16 spaces
        lcd.setCursor(inputBuffer.length(), 1);
      }
      else if ((key == 0x08 || key == 0x7F) && inputBuffer.length() > 0) { // backspace
        inputBuffer.remove(inputBuffer.length() - 1);
        lcd.setCursor(0, 1);
        lcd.print(inputBuffer);
        lcd.print("                ");
        lcd.setCursor(inputBuffer.length(), 1);
      }
      else if (key == 0x0D || key == 0x0A) {       // Enter (CR or LF)
        if (inputBuffer.length() > 0) {
          processInput();         // store name, go to password
        }
        inputBuffer = "";
      }
    }
  }
  // PASSWORD ENTRY – keypad with key‑press beep (unchanged)
  else if (progState == ENTER_PASSWORD) {
    if (keypad.getKeys()) {
      for (int i = 0; i < LIST_MAX; i++) {
        if (keypad.key[i].stateChanged && keypad.key[i].kstate == PRESSED) {
          char key = keypad.key[i].kchar;
          tone(buzzerPin, 1000, 50);   // beep on every press

          if (key >= '0' && key <= '9') {
            inputBuffer += key;
            updateDisplay();
          }
          else if (key == '*') {
            if (inputBuffer.length() > 0) {
              inputBuffer.remove(inputBuffer.length() - 1);
              updateDisplay();
            }
          }
          else if (key == '#') {
            if (inputBuffer.length() > 0) {
              noTone(buzzerPin);       // stop beep before processing
              processInput();
              inputBuffer = "";
            }
          }
        }
      }
    }
  }
  // ACCESS GRANTED / DENIED blinking messages (unchanged)
  else if (progState == ACCESS_GRANTED || progState == ACCESS_DENIED) {
    unsigned long now = millis();
    if (now - lastBlinkTime >= blinkInterval) {
      blinkState = !blinkState;
      lastBlinkTime = now;

      lcd.clear();
      if (progState == ACCESS_GRANTED) {
        if (blinkState) {
          lcd.setCursor(0, 0);
          lcd.print("WELCOME, " + userName + "!!!!");
        }
      }
      else if (progState == ACCESS_DENIED) {
        if (blinkState) {
          lcd.setCursor(0, 0);
          lcd.print("ACCESS DENIED!!!");
          lcd.setCursor(0, 1);
          lcd.print(String(attempts) + " ATTEMPT ALREADY");
        }
      }
    }
  }
}

// Restored to original behaviour – name is NOT shown on password screen
void updateDisplay() {
  lcd.clear();
  if (progState == ENTER_NAME) {
    lcd.setCursor(0, 0);
    lcd.print("ENTER YOUR NAME:");
    lcd.setCursor(0, 1);
    lcd.print(inputBuffer);
  }
  else if (progState == ENTER_PASSWORD) {
    lcd.setCursor(0, 0);
    lcd.print("ENTER PASSWORD:");      // exactly like the original
    lcd.setCursor(0, 1);
    String masked = "";
    for (unsigned int i = 0; i < inputBuffer.length(); i++) masked += '*';
    lcd.print(masked);
  }
}

void processInput() {
  if (progState == ENTER_NAME) {
    userName = inputBuffer;
    inputBuffer = "";
    attempts = 0;
    progState = ENTER_PASSWORD;
    Serial.print(userName);
    updateDisplay();   // shows original “ENTER PASSWORD:” now
  }
  else if (progState == ENTER_PASSWORD) {
    if (inputBuffer == correctPassword) {
      grantAccess();
    } else {
      attempts++;
      if (attempts < 3) {
        wrongPattern();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("INCORRECT! " + String(attempts) + "/3");
        delay(1500);
        progState = ENTER_PASSWORD;
        inputBuffer = "";
        updateDisplay();
      } else {
        denyAccess();
      }
    }
  }
}

void grantAccess() {
  progState = ACCESS_GRANTED;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME, " + userName + "!!!!");
  delay(1000);

  playToneSequence(correctMelody, correctDurations, melodyLength);
  runLEDsLeftRight(2);

  blinkState = true;
  lastBlinkTime = millis();
}

void denyAccess() {
  progState = ACCESS_DENIED;
  wrongPattern();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ACCESS DENIED!!!");
  lcd.setCursor(0, 1);
  lcd.print(String(attempts) + " ATTEMPT ALREADY");
  delay(2000);
  blinkState = false;
  lastBlinkTime = millis();
}

void playToneSequence(const int* notes, const int* durations, int len) {
  for (int i = 0; i < len; i++) {
    tone(buzzerPin, notes[i], durations[i]);
    delay(durations[i] * 1.2);
    noTone(buzzerPin);
  }
}

void runLEDsLeftRight(int times) {
  for (int t = 0; t < times; t++) {
    for (int i = 0; i < 6; i++) {
      digitalWrite(ledPins[i], HIGH);
      delay(80);
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void wrongPattern() {
  for (int i = 0; i < 3; i++) {
    tone(buzzerPin, 200, 200);
    for (int j = 0; j < 6; j++) digitalWrite(ledPins[j], HIGH);
    delay(200);
    for (int j = 0; j < 6; j++) digitalWrite(ledPins[j], LOW);
    delay(100);
  }
  noTone(buzzerPin);
}
