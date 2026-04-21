#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = { { '1', '2', '3' }, { '4', '5', '6' }, { '7', '8', '9' }, { '*', '0', '#' } };
byte rowPins[ROWS] = { 10, 11, 12, 13 };
byte colPins[COLS] = { A0, A1, A2 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int LED_COUNT = 8;
const int ledPins[LED_COUNT] = { 2, 3, 4, 5, 6, 7, 8, 9 };

int readInt() {
  int val = 0;
  do {
    String buf = "";
    Serial.print(F("> "));
    bool done = false;
    while (!done) {
      char k = keypad.getKey();
      if (k >= '0' && k <= '9') {
        buf += k;
        Serial.print(k);
      } else if (k == '#') {
        Serial.println();
        done = true;
      }

      if (!done && Serial.available()) {
        delay(20);
        while (Serial.available()) {
          char c = Serial.read();
          if (c >= '0' && c <= '9') {
            buf += c;
            Serial.print(c);
          }
        }
        Serial.println();
        done = true;
      }
    }
    val = buf.toInt();
    if (val <= 0) Serial.println(F("Invalid! Enter a number > 0:"));
  } while (val <= 0);
  return val;
}

float readFloat(const __FlashStringHelper* prompt) {
  Serial.println(prompt);
  String buf = "";
  bool hasDot = false;
  Serial.print(F("> "));
  bool done = false;
  while (!done) {
    char k = keypad.getKey();
    if (k >= '0' && k <= '9') {
      buf += k;
      Serial.print(k);
    } else if (k == '*' && !hasDot) {
      buf += '.';
      hasDot = true;
      Serial.print('.');
    } else if (k == '#') {
      Serial.println();
      done = true;
    }

    if (!done && Serial.available()) {
      delay(20);
      while (Serial.available()) {
        char c = Serial.read();
        if (c >= '0' && c <= '9') {
          buf += c;
          Serial.print(c);
        } else if (c == '.' && !hasDot) {
          buf += '.';
          hasDot = true;
          Serial.print('.');
        }
      }
      Serial.println();
      done = true;
    }
  }
  return buf.toFloat();
}

bool tryAgain() {
  delay(30);
  while (Serial.available()) Serial.read();  // flush leftover bytes from previous input

  Serial.println(F("Do you want to try again? Y/N"));
  while (true) {
    while (!Serial.available())
      ;
    delay(20);
    String line = "";
    while (Serial.available()) {
      char c = Serial.read();
      if (c != ' ' && c != '\t' && c != '\r' && c != '\n') line += c;
    }
    if (line.length() == 0) continue;
    if (line.length() == 1) {
      if (line[0] == 'Y' || line[0] == 'y') return true;
      if (line[0] == 'N' || line[0] == 'n') return false;
    }
    Serial.println(F("Invalid! Type ONLY Y or N, then press Enter:"));
  }
}

void ledsOff() {
  for (int i = 0; i < LED_COUNT; i++) digitalWrite(ledPins[i], LOW);
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < LED_COUNT; i++) pinMode(ledPins[i], OUTPUT);
  ledsOff();
}

void loop() {
  Serial.println();
  Serial.println(F("=============================="));
  Serial.println(F("  <<<<<Choose your payter>>>>>"));
  Serial.println(F("=============================="));
  Serial.println(F("1. Pyramid of Stars"));
  Serial.println(F("2. Draw a Square"));
  Serial.println(F("3. Fibonacci Number Series"));
  Serial.println(F("4. Multiplication Table (up to x10)"));
  Serial.println(F("5. Surface Area of a Cone"));
  Serial.println(F("6. Print Louis Vuitton Logo"));
  Serial.println(F("7. Light 8 LEDs left to right (3s)"));
  Serial.println(F("8. Blink left 4, then right 4 LEDs"));
  Serial.println(F("9. Error + Blink all LEDs for 2s"));
  Serial.println(F("0. Display 0"));
  Serial.println(F("*. Display *"));
  Serial.println(F("#. Display #"));
  Serial.println(F("------------------------------"));
  Serial.println(F("Press a key on the keypad..."));

  char key = 0;
  while (key == 0) key = keypad.getKey();

  // ── 1 · Pyramid of Stars
  if (key == '1') {
    do {
      Serial.println(F("--- Pyramid of Stars ---"));
      Serial.println(F("Enter height, press # or Enter to confirm:"));
      int h = readInt();
      for (int i = 1; i <= h; i++) {
        for (int j = 1; j <= (h - i); j++) {
          Serial.print(" ");
        }
        for (int k = 1; k <= (2 * i - 1); k++) {
          Serial.print("*");
        }
        Serial.println();
      }
    } while (tryAgain());
  }

  // ── 2 · Draw a Square
  else if (key == '2') {
    do {
      Serial.println(F("--- Draw a Square ---"));
      Serial.println(F("Enter side size (> 0), press # or Enter to confirm:"));
      int n = readInt();
      for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++)
          Serial.print((row == 0 || row == n - 1 || col == 0 || col == n - 1) ? '*' : ' ');
        Serial.println();
      }
    } while (tryAgain());
  }

  // ── 3 · Fibonacci
  else if (key == '3') {
    do {
      Serial.println(F("--- Fibonacci Number Series ---"));
      Serial.println(F("How many numbers? Press # or Enter to confirm:"));
      int n = readInt();
      long a = 0, b = 1;
      for (int i = 0; i < n; i++) {
        Serial.print(a);
        if (i < n - 1) Serial.print(F(", "));
        long t = a + b;
        a = b;
        b = t;
      }
      Serial.println();
    } while (tryAgain());
  }

  // ── 4 · Multiplication Table
  else if (key == '4') {
    do {
      Serial.println(F("--- Multiplication Table ---"));
      Serial.println(F("Enter a number (> 0), press # or Enter to confirm:"));
      unsigned long n = readInt();
      for (int i = 1; i <= 10; i++) {
        Serial.print(n);
        Serial.print(F(" x "));
        Serial.print(i);
        Serial.print(F(" = "));
        Serial.println(n * i);
      }
    } while (tryAgain());
  }

  // ── 5 · Surface Area of a Cone
  else if (key == '5') {
    do {
      Serial.println(F("--- Surface Area of a Cone ---"));
      float r = readFloat(F("Enter radius r   (* or . = decimal, # or Enter = confirm):"));
      float l = readFloat(F("Enter slant h l  (* or . = decimal, # or Enter = confirm):"));
      float sa = PI * r * (r + l);
      Serial.print(F("SA = pi x "));
      Serial.print(r, 2);
      Serial.print(F(" x ("));
      Serial.print(r, 2);
      Serial.print(F(" + "));
      Serial.print(l, 2);
      Serial.print(F(") = "));
      Serial.println(sa, 4);
    } while (tryAgain());
  }

  // ── 6 · Louis Vuitton Logo
  else if (key == '6') {
    Serial.println(F("--- Louis Vuitton Logo ---\n"));
    Serial.println(F("  ******               *****"));
    Serial.println(F("   ****   ******        **"));
    Serial.println(F("    ****   ****        **"));
    Serial.println(F("     **** ****        **"));
    Serial.println(F("      *******        **"));
    Serial.println(F("       *****        **"));
    Serial.println(F("       ****        **"));
    Serial.println(F("      *******     **"));
    Serial.println(F("     **** ****   **"));
    Serial.println(F("    ****   **** **             **"));
    Serial.println(F("   ****     *****            ***"));
    Serial.println(F("  ****                      ***"));
    Serial.println(F(" *****************************"));
    Serial.println(F("****************************\n"));
    Serial.println(F("L O U I S   V U I T T O N"));
  }

  // ── 7 · LED Sweep Left to Right
  else if (key == '7') {
    Serial.println(F("--- LED Sweep: Left to Right ---"));
    ledsOff();
    for (int i = 0; i < LED_COUNT; i++) {
      digitalWrite(ledPins[i], HIGH);
      delay(375);
      digitalWrite(ledPins[i], LOW);
    }
    Serial.println(F("Done."));
  }

  // ── 8 · Blink Left 4 then Right 4
  else if (key == '8') {
    Serial.println(F("--- Blink Left 4, then Right 4 ---"));
    ledsOff();
    for (int left = 0; left < 4; left++) {
      digitalWrite(ledPins[left], HIGH);
    }
    delay(1000);
    for (int left = 0; left < 4; left++) {
      digitalWrite(ledPins[left], LOW);
    }
    for (int right = 4; right < 8; right++) {
      digitalWrite(ledPins[right], HIGH);
    }
    delay(1000);
    for (int right = 4; right < 8; right++) {
      digitalWrite(ledPins[right], LOW);
    }

    // for (int grp = 0; grp < 2; grp++) {
    //   Serial.println(grp == 0 ? F("Left 4...") : F("Right 4..."));
    //   int s = grp * 4;
    //   for (int blink = 0; blink < 3; blink++) {
    //     for (int i = s; i < s + 4; i++) digitalWrite(ledPins[i], HIGH);
    //     delay(400);
    //     for (int i = s; i < s + 4; i++) digitalWrite(ledPins[i], LOW);
    //     delay(400);
    //   }
    //   if (grp == 0) delay(300);
    
    Serial.println(F("Done."));
  }

  // ── 9 · Error + Blink All LEDs
  else if (key == '9') {
    Serial.println(F("!!!!!!!!!!!!!!!!!!!!!!!!\n!!!     E R R O R    !!!\n!!!!!!!!!!!!!!!!!!!!!!!!"));
    Serial.println(F("Blinking all LEDs for 2 seconds..."));
    ledsOff();
    for (int blink = 0; blink < 7; blink++) {
      for (int i = 0; i < LED_COUNT; i++) digitalWrite(ledPins[i], HIGH);
      delay(150);
      for (int i = 0; i < LED_COUNT; i++) digitalWrite(ledPins[i], LOW);
      delay(150);
    }
    Serial.println(F("Error acknowledged."));
  }

  // else if (key == '0') Serial.println(F("0"));
  else if (key == '*') Serial.println(F("*"));
  else if (key == '#') Serial.println(F("#"));
  else Serial.println(F("INVALID CODE!!!"));
}