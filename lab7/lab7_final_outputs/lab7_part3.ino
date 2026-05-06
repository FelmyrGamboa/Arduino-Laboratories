#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

enum State {
  WAITING_FIRST,
  WAITING_SECOND,
  RESULT_SHOWN
};

State currentState = WAITING_FIRST;

String numFirst   = "";
String numSecond  = "";
char   op         = '\0';
char   preview    = '\0';
bool   operatorConfirmed = false;

unsigned long starPressTime  = 0;
bool starIsPressed           = false;
bool oneDeleteDone           = false;
bool clearDone               = false;
// rapidDeleteActive and lastRapidDeleteTime removed

double calculate(double a, char op, double b) {
  switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return (b != 0) ? a / b : NAN;
    default : return a;
  }
}

// Format a number string to exactly 2 decimal places
String formatNumberDisplay(String s) {
  if (s.length() == 0) return "0.00";
  double val = s.toDouble();
  double rounded = round(val * 100.0) / 100.0;
  char buf[20];
  dtostrf(rounded, 0, 2, buf);   // e.g., "12.00"
  String out(buf);
  out.trim();                     // remove leading spaces
  return out;
}

// Format a computed result to exactly 2 decimal places
String formatResult(double val) {
  if (isnan(val)) return "Error: Div/0";
  if (isinf(val)) return "Overflow!";
  double rounded = round(val * 100.0) / 100.0;
  char buf[20];
  dtostrf(rounded, 0, 2, buf);
  String s(buf);
  s.trim();
  if (s.length() > 16) s = s.substring(0, 16);
  return s;
}

void resetCalculator() {
  numFirst    = "";
  numSecond   = "";
  op          = '\0';
  preview     = '\0';
  operatorConfirmed = false;
  currentState = WAITING_FIRST;
  lcd.clear();
  updateDisplay();
}

void insertDecimal() {
  String* buf = (currentState == WAITING_SECOND && operatorConfirmed) ? &numSecond : &numFirst;
  if (buf->length() == 0)
    *buf = "0.";
  else if (buf->indexOf('.') == -1)
    *buf += '.';
  updateDisplay();
}

void cycleOperator() {
  static char ops[] = {'+', '-', '*', '/'};
  char current = (preview != '\0') ? preview : op;
  if (current == '\0') current = '+';
  int idx = 0;
  for (int i = 0; i < 4; i++) {
    if (ops[i] == current) { idx = i; break; }
  }
  idx = (idx + 1) % 4;
  preview = ops[idx];
  updateDisplay();
}

void deleteLast() {
  if (currentState == RESULT_SHOWN) return;

  if (preview != '\0') {
    preview = '\0';
    updateDisplay();
    return;
  }

  String* buf = nullptr;
  if (currentState == WAITING_FIRST) {
    buf = &numFirst;
  } else if (currentState == WAITING_SECOND) {
    if (operatorConfirmed) buf = &numSecond;
    else return;
  }

  if (buf && buf->length() > 0) {
    buf->remove(buf->length() - 1);
    updateDisplay();
  }
}

void executeHash() {
  if (currentState == RESULT_SHOWN) {
    resetCalculator();
    return;
  }

  if (currentState == WAITING_FIRST) {
    if (numFirst.length() > 0) {
      op = '+';
      currentState = WAITING_SECOND;
      operatorConfirmed = false;
      preview = '\0';
      updateDisplay();
    }
    return;
  }

  if (currentState == WAITING_SECOND) {
    if (!operatorConfirmed) {
      if (preview != '\0') op = preview;
      preview = '\0';
      operatorConfirmed = true;
      updateDisplay();
      return;
    }

    if (operatorConfirmed && numSecond.length() > 0) {
      double a = numFirst.toDouble();
      double b = numSecond.toDouble();
      double result = calculate(a, op, b);
      String resStr = String(result);

      // String firstDisp  = formatNumberDisplay(numFirst);
      String firstDisp = numFirst;
      // String secondDisp = formatNumberDisplay(numSecond);
      String secondDisp = numSecond;
      String resultDisp = formatNumberDisplay(resStr);   // already 2 decimals

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(firstDisp + op + secondDisp + "=");
      lcd.setCursor(0, 1);
      lcd.print(resultDisp);

      numFirst = resultDisp;   // store as formatted string for chaining
      numSecond = "";
      op = '\0';
      currentState = RESULT_SHOWN;
      preview = '\0';
      operatorConfirmed = false;
    }
  }
}

void addDigit(char digit) {
  if (currentState == RESULT_SHOWN) {
    resetCalculator();
    currentState = WAITING_FIRST;
  }

  if (currentState == WAITING_SECOND && !operatorConfirmed) {
    return;
  }

  String* buf = (currentState == WAITING_SECOND && operatorConfirmed) ? &numSecond : &numFirst;
  *buf += digit;
  preview = '\0';
  updateDisplay();
}

void updateDisplay() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);

  if (currentState != RESULT_SHOWN) {
    if (currentState == WAITING_FIRST) {
      lcd.print(numFirst.length() ? numFirst : "0");
    }
    else if (currentState == WAITING_SECOND) {
      // String firstDisp = formatNumberDisplay(numFirst);
      String firstDisp = numFirst;
      lcd.print(firstDisp);

      char showOp = (!operatorConfirmed) ? ((preview != '\0') ? preview : op) : op;
      if (showOp != '\0') {
        lcd.print(showOp);
        if (operatorConfirmed) {
          lcd.print(numSecond);   // raw second number while typing
        }
      }
    }
  }

  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void setup() {
  lcd.init();
  lcd.backlight();
  resetCalculator();
}

void loop() {
  if (keypad.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {
      if (keypad.key[i].stateChanged) {
        char key = keypad.key[i].kchar;

        if (key == '*') {
          if (keypad.key[i].kstate == PRESSED) {
            starPressTime = millis();
            starIsPressed = true;
            oneDeleteDone = false;
            clearDone = false;
            // rapidDeleteActive removed
          }
          else if (keypad.key[i].kstate == RELEASED) {
            if (starIsPressed) {
              unsigned long duration = millis() - starPressTime;
              if (!oneDeleteDone) {
                // short press: insert decimal or cycle operator (as before)
                if (currentState == RESULT_SHOWN) {
                  // do nothing on short press in result mode
                }
                else if (currentState == WAITING_FIRST ||
                         (currentState == WAITING_SECOND && operatorConfirmed)) {
                  insertDecimal();
                }
                else if (currentState == WAITING_SECOND && !operatorConfirmed) {
                  cycleOperator();
                }
              }
              starIsPressed = false;
              oneDeleteDone = false;
              clearDone = false;
              // rapidDeleteActive removed
            }
          }
        }
        else if (key == '#' && keypad.key[i].kstate == PRESSED) {
          executeHash();
        }
        else if (key >= '0' && key <= '9' && keypad.key[i].kstate == PRESSED) {
          addDigit(key);
        }
      }
    }
  }

  // Hold timing for *
  if (starIsPressed) {
    unsigned long holdTime = millis() - starPressTime;

    // Single delete after 2 seconds
    if (!oneDeleteDone && holdTime >= 1000) {
      deleteLast();
      oneDeleteDone = true;
    }

    // Clear all after 4 seconds
    if (!clearDone && holdTime >= 3000) {
      resetCalculator();
      clearDone = true;
    }

    // Rapid delete removed entirely
  }
}
