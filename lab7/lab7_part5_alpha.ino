#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// --- LCD CONFIGURATION ---
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- KEYPAD CONFIGURATION ---
const byte ROWS = 4; 
const byte COLS = 3; 
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'}, // 4 = DON, 5 = SELECT, 6 = KA
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {10, 9, 8, 7}; 
byte colPins[COLS] = {6, 5, 4}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- HARDWARE PINS ---
const int BUZZER_PIN = 2; 

// --- GAME CONSTANTS ---
#define NOTE_DON 1   
#define NOTE_KA  2   

// Timing Windows (ms)
const int WINDOW_PERFECT = 80;
const int WINDOW_GOOD    = 180;
const int SCROLL_SPEED   = 50; 

// --- CUSTOM CHARACTERS ---
byte charDon[8] = {0b00000, 0b01110, 0b11111, 0b11111, 0b11111, 0b01110, 0b00000, 0b00000}; 
byte charKa[8]  = {0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000, 0b00000}; 

// --- GAME STATE ---
enum GameState { START, COUNTDOWN, PLAYING, GAMEOVER };
GameState currentState = START;

struct Note {
  unsigned long targetTime;
  int type;
  bool processed;
};

Note song[] = {
  {2000, NOTE_DON, false}, {2600, NOTE_DON, false}, {3200, NOTE_KA, false},
  {4000, NOTE_DON, false}, {4400, NOTE_KA, false},  {4800, NOTE_DON, false},
  {6000, NOTE_KA, false},  {6300, NOTE_KA, false},  {6600, NOTE_DON, false},
  {7500, NOTE_DON, false}, {8500, NOTE_KA, false},  {10000, NOTE_DON, false}
};
const int songLength = sizeof(song) / sizeof(song[0]);

unsigned long gameStartTime = 0;
int score = 0;
int combo = 0;
String lastFeedback = "";

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, charDon);
  lcd.createChar(2, charKa);
  showStartScreen();
}

void loop() {
  char key = keypad.getKey(); // Non-blocking key detection

  switch (currentState) {
    case START:
      if (key == '5') startGame();
      break;
    
    case COUNTDOWN:
      runCountdown();
      break;
      
    case PLAYING:
      updateGame(key);
      break;
      
    case GAMEOVER:
      if (key == '5') resetGame();
      break;
  }
}

void startGame() {
  score = 0;
  combo = 0;
  lastFeedback = "READY?";
  for(int i=0; i<songLength; i++) song[i].processed = false;
  currentState = COUNTDOWN;
}

void runCountdown() {
  for (int i = 3; i > 0; i--) {
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print(i);
    tone(BUZZER_PIN, 440, 100);
    delay(1000);
  }
  gameStartTime = millis();
  currentState = PLAYING;
}

void updateGame(char pressedKey) {
  unsigned long currentTime = millis() - gameStartTime;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">"); // Hit Zone

  // Note Rendering
  for (int i = 0; i < songLength; i++) {
    if (!song[i].processed) {
      long timeDiff = (long)song[i].targetTime - (long)currentTime;
      int xPos = timeDiff / SCROLL_SPEED;
      
      if (xPos >= 0 && xPos < 16) {
        lcd.setCursor(xPos, 0);
        lcd.write(song[i].type);
      }
      
      if (timeDiff < -WINDOW_GOOD) {
        song[i].processed = true;
        handleHit(0); 
      }
    }
  }

  // Keypad Input Logic
  if (pressedKey) {
    if (pressedKey == '4') checkTiming(currentTime, NOTE_DON);
    if (pressedKey == '6') checkTiming(currentTime, NOTE_KA);
  }

  lcd.setCursor(0, 1);
  lcd.print(lastFeedback);
  lcd.setCursor(11, 1);
  lcd.print("C:"); lcd.print(combo);

  if (currentTime > song[songLength-1].targetTime + 1500) {
    currentState = GAMEOVER;
    showGameOver();
  }
  
  delay(20); 
}

void checkTiming(unsigned long currentTime, int hitType) {
  for (int i = 0; i < songLength; i++) {
    if (!song[i].processed) {
      long diff = abs((long)currentTime - (long)song[i].targetTime);
      if (diff <= WINDOW_GOOD) {
        song[i].processed = true;
        if (hitType != song[i].type) handleHit(0);
        else if (diff <= WINDOW_PERFECT) handleHit(2);
        else handleHit(1);
        return;
      }
    }
  }
}

void handleHit(int quality) {
  if (quality == 2) { 
    score += 100; combo++; lastFeedback = "PERFECT!";
    tone(BUZZER_PIN, 600, 60);
  } else if (quality == 1) { 
    score += 50; combo++; lastFeedback = "GOOD";
    tone(BUZZER_PIN, 450, 60);
  } else { 
    combo = 0; lastFeedback = "MISS";
    tone(BUZZER_PIN, 150, 150);
  }
}

void showStartScreen() {
  lcd.clear();
  lcd.setCursor(3, 0); lcd.print("TAIKO 4x3");
  lcd.setCursor(0, 1); lcd.print("Press 5 to Start");
}

void showGameOver() {
  lcd.clear();
  lcd.print("FINAL SCORE:");
  lcd.setCursor(0, 1); lcd.print(score);
  lcd.print(" - Press 5");
}

void resetGame() {
  currentState = START;
  showStartScreen();
}
