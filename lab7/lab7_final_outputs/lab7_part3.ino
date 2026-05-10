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

// ── State ──────────────────────────────────────────
String fullExpr      = "";    // committed part, e.g. "3+4*"
String currentNumber = "";    // number being typed
char   pendingOp     = '\0';  // operator shown but not yet committed
bool   opSelected    = false; // true if we are showing an operator (pending)

// ── Result display after evaluation (no chaining) ──
bool   resultShowing = false; // true after a long-press # evaluation
String lastResult    = "";    // holds the result text while displayed

// ── Timing & presses ───────────────────────────────
unsigned long starPressTime = 0, hashPressTime = 0;
bool starPressed = false, hashPressed = false;
bool oneDeleteDone = false, clearDone = false;
const unsigned long HASH_LONG = 2000;  // <-- changed to 2 seconds

// ── PEMDAS evaluation (two‑stack algorithm) ────────
double applyOp(double a, double b, char op) {
  switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return (b == 0) ? NAN : a / b;
  }
  return 0;
}

int precedence(char op) {
  if (op == '+' || op == '-') return 1;
  if (op == '*' || op == '/') return 2;
  return 0;
}

double evaluate(String expr) {
  double nStack[20]; int nTop = 0;
  char   oStack[20]; int oTop = 0;
  int i = 0;
  while (i < expr.length()) {
    if (expr[i] == ' ') { i++; continue; }
    if (isdigit(expr[i]) || expr[i] == '.') {
      String num = "";
      while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.'))
        num += expr[i++];
      nStack[nTop++] = num.toDouble();
    }
    else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
      char c = expr[i++];
      while (oTop > 0 && precedence(oStack[oTop-1]) >= precedence(c)) {
        if (nTop < 2) return NAN;
        double b = nStack[--nTop];
        double a = nStack[--nTop];
        double r = applyOp(a, b, oStack[--oTop]);
        if (isnan(r)) return r;
        nStack[nTop++] = r;
      }
      oStack[oTop++] = c;
    }
    else i++;
  }
  while (oTop > 0) {
    if (nTop < 2) return NAN;
    double b = nStack[--nTop];
    double a = nStack[--nTop];
    double r = applyOp(a, b, oStack[--oTop]);
    if (isnan(r)) return r;
    nStack[nTop++] = r;
  }
  return (nTop == 1) ? nStack[0] : NAN;
}

// ── Formatting ─────────────────────────────────────
String fmtResult(double val) {
  if (isnan(val)) return "Error";
  if (isinf(val)) return "Overflow";
  char buf[20];
  dtostrf(val, 1, 2, buf);
  String s = buf;
  if (s.length() > 16) s = s.substring(0, 16);
  return s;
}

// ── LCD update ─────────────────────────────────────
void updateDisplay() {
  // If a result is being shown, don't overwrite it here
  if (resultShowing) return;

  lcd.clear();
  String line0;

  // Build the full line to show
  if (fullExpr.length() > 0)
    line0 = fullExpr;
  else if (currentNumber.length() > 0)
    line0 = "0"; // will be overwritten, but just in case

  // If we have an operator selected (pending) show it after the committed part
  if (opSelected)
    line0 += pendingOp;
  // Then add the current number being typed
  if (currentNumber.length() > 0)
    line0 += currentNumber;
  else if (line0.length() == 0)
    line0 = "0";              // empty expression → show "0"

  lcd.setCursor(0, 0);
  lcd.print(line0.substring(0, 16));
  // Second line stays blank (except after an evaluation)
}

// ── Core actions ───────────────────────────────────
void resetCalc() {
  fullExpr = "";
  currentNumber = "";
  pendingOp = '\0';
  opSelected = false;
  resultShowing = false;   // clear any stuck result flag
  lastResult = "";
  lcd.clear();
  updateDisplay();
}

void insertDecimal() {
  if (currentNumber.length() == 0)
    currentNumber = "0.";
  else if (currentNumber.indexOf('.') == -1)
    currentNumber += '.';
  updateDisplay();
}

void deleteLast() {
  // If a result is being shown, ignore deletes
  if (resultShowing) return;

  // If an operator is selected (pending), remove it
  if (opSelected) {
    opSelected = false;
    pendingOp = '\0';
    updateDisplay();
    return;
  }
  // Otherwise delete from current number
  if (currentNumber.length() > 0) {
    currentNumber.remove(currentNumber.length() - 1);
    updateDisplay();
    return;
  }
  // If current number empty, delete from committed expression
  if (fullExpr.length() > 0) {
    char last = fullExpr.charAt(fullExpr.length()-1);
    if (last == '+' || last == '-' || last == '*' || last == '/') {
      fullExpr.remove(fullExpr.length()-1);
    } else {
      fullExpr.remove(fullExpr.length()-1);
    }
    updateDisplay();
  }
}

// Short press of *: handle operator selection/cycle
void starShortAction() {
  if (resultShowing) return;   // result is displayed, ignore operator selection

  if (currentNumber.length() > 0) {
    fullExpr += currentNumber;
    currentNumber = "";
    opSelected = true;
    pendingOp = '+';          // default operator
    updateDisplay();
  }
  else if (opSelected) {
    const char ops[] = {'+', '-', '*', '/'};
    int i = 0;
    for (; i < 4; i++) if (ops[i] == pendingOp) break;
    pendingOp = ops[(i + 1) % 4];
    updateDisplay();
  }
  else if (fullExpr.length() > 0 && !opSelected) {
    opSelected = true;
    pendingOp = '+';
    updateDisplay();
  }
}

// Short press of #: decimal point
void hashShortAction() {
  if (resultShowing) return;   // result is displayed, ignore decimal
  insertDecimal();
}

// Long press of #: evaluate everything
void hashLongAction() {
  // Build the complete expression
  String expr = fullExpr;
  if (currentNumber.length() > 0)
    expr += currentNumber;
  // If no current number but an op is selected, the expression is incomplete – ignore the op
  if (expr.length() == 0) return;

  double result = evaluate(expr);
  String resStr = fmtResult(result);

  // Show the expression and result on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(expr.substring(0, 16));
  lcd.setCursor(0, 1);
  lcd.print(resStr);

  // Reset the calculator state – no chaining
  fullExpr = "";
  currentNumber = "";
  pendingOp = '\0';
  opSelected = false;

  // Keep the result displayed until the next key press
  resultShowing = true;
  lastResult = resStr;
}

// Digit press
void addDigit(char d) {
  // If an operator is currently selected, first commit it to the expression
  if (opSelected) {
    fullExpr += pendingOp;
    opSelected = false;
    pendingOp = '\0';
  }
  currentNumber += d;
  updateDisplay();
}

// ── Helper to clear the displayed result ───────────
void clearResultDisplay() {
  if (!resultShowing) return;
  resultShowing = false;
  lastResult = "";
  // Clear the LCD and show the initial "0"
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("0");
}

// ── Setup ──────────────────────────────────────────
void setup() {
  lcd.init();
  lcd.backlight();
  resetCalc();
}

// ── Loop ───────────────────────────────────────────
void loop() {
  if (keypad.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {
      if (keypad.key[i].stateChanged) {
        // If a result is being shown, clear it on any key press
        if (resultShowing && keypad.key[i].kstate == PRESSED) {
          clearResultDisplay();
          // After clearing, continue processing the key normally
        }

        char key = keypad.key[i].kchar;

        // ---------- * key ----------
        if (key == '*') {
          if (keypad.key[i].kstate == PRESSED) {
            starPressTime = millis();
            starPressed = true;
            oneDeleteDone = false;
            clearDone = false;
          }
          else if (keypad.key[i].kstate == RELEASED) {
            if (starPressed) {
              unsigned long dur = millis() - starPressTime;
              if (!oneDeleteDone) {
                starShortAction();
              }
              starPressed = false;
              oneDeleteDone = false;
              clearDone = false;
            }
          }
        }

        // ---------- # key ----------
        else if (key == '#') {
          if (keypad.key[i].kstate == PRESSED) {
            hashPressTime = millis();
            hashPressed = true;
          }
          else if (keypad.key[i].kstate == RELEASED) {
            if (hashPressed) {
              if (millis() - hashPressTime >= HASH_LONG)
                hashLongAction();
              else
                hashShortAction();
              hashPressed = false;
            }
          }
        }

        // ---------- digits 0-9 ----------
        else if (key >= '0' && key <= '9' && keypad.key[i].kstate == PRESSED) {
          addDigit(key);
        }
      }
    }
  }

  // ---------- Long press * (delete / reset) ----------
  if (starPressed) {
    unsigned long hold = millis() - starPressTime;
    if (!oneDeleteDone && hold >= 1000) {
      deleteLast();
      oneDeleteDone = true;
    }
    if (!clearDone && hold >= 3000) {
      resetCalc();
      clearDone = true;
    }
  }
}