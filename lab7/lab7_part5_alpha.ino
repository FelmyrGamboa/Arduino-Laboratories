#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// --- LCD CONFIG ---
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- KEYPAD CONFIG ---
const byte ROWS = 4; 
const byte COLS = 3; 
char keys[ROWS][COLS] = {
  {'1','2','3'}, // 1, 2, 3 used for Song Selection
  {'4','5','6'}, // 4 = DON, 5 = MENU/BACK, 6 = KA
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {10, 9, 8, 7}; 
byte colPins[COLS] = {6, 5, 4}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- PINS & CONSTANTS ---
const int BUZZER_PIN = 2;
#define NOTE_DON 1   
#define NOTE_KA  2   

// Musical Frequencies
#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define A4  440
#define B4  494
#define C5  523

// --- GAME STRUCTURES ---
struct Note {
  unsigned long targetTime;
  int frequency;
  int type; // Randomly assigned as DON or KA
  bool processed;
  bool soundPlayed;
};

// Max notes per song
const int MAX_NOTES = 20;
Note currentSong[MAX_NOTES];
int currentSongLength = 0;

enum GameState { MENU, COUNTDOWN, PLAYING, GAMEOVER };
GameState currentState = MENU;

unsigned long gameStartTime = 0;
int score = 0;

// --- SONG DATA ---
void loadSong(int choice) {
  randomSeed(analogRead(A1)); // Seed randomness using floating pin
  score = 0;
  
  if (choice == 1) { // Twinkle Twinkle (Simple)
    int mel[] = {C4, C4, G4, G4, A4, A4, G4, F4, F4, E4, E4, D4, D4, C4};
    currentSongLength = 14;
    for(int i=0; i<currentSongLength; i++) {
      currentSong[i] = { (unsigned long)(2000 + (i * 800)), mel[i], random(1, 3), false, false };
    }
  } 
  else if (choice == 2) { // Jingle Bells (Faster)
    int mel[] = {E4, E4, E4, E4, E4, E4, E4, G4, C4, D4, E4};
    currentSongLength = 11;
    for(int i=0; i<currentSongLength; i++) {
      currentSong[i] = { (unsigned long)(2000 + (i * 500)), mel[i], random(1, 3), false, false };
    }
  }
  else if (choice == 3) { // Challenge Scale
    currentSongLength = 8;
    int mel[] = {C4, D4, E4, F4, G4, A4, B4, C5};
    for(int i=0; i<currentSongLength; i++) {
      currentSong[i] = { (unsigned long)(2000 + (i * 400)), mel[i], random(1, 3), false, false };
    }
  }
}

// --- CUSTOM CHARS ---
byte charDon[8] = {0b00000, 0b01110, 0b11111, 0b11111, 0b11111, 0b01110, 0b00000, 0b00000}; 
byte charKa[8]  = {0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000, 0b00000}; 

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, charDon);
  lcd.createChar(2, charKa);
  showMenu();
}

void loop() {
  char key = keypad.getKey();

  switch (currentState) {
    case MENU:
      if (key == '1') { loadSong(1); currentState = COUNTDOWN; }
      if (key == '2') { loadSong(2); currentState = COUNTDOWN; }
      if (key == '3') { loadSong(3); currentState = COUNTDOWN; }
      break;

    case COUNTDOWN:
      runCountdown();
      break;

    case PLAYING:
      updateGame(key);
      break;

    case GAMEOVER:
      if (key == '5') showMenu();
      break;
  }
}

void runCountdown() {
  for (int i = 3; i > 0; i--) {
    lcd.clear();
    lcd.setCursor(7, 0); lcd.print(i);
    tone(BUZZER_PIN, 440, 100);
    delay(1000);
  }
  gameStartTime = millis();
  currentState = PLAYING;
}

void updateGame(char key) {
  unsigned long currentTime = millis() - gameStartTime;
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(">"); // Hit zone

  bool allNotesDone = true;

  for (int i = 0; i < currentSongLength; i++) {
    if (!currentSong[i].processed) {
      allNotesDone = false;
      long timeDiff = (long)currentSong[i].targetTime - (long)currentTime;
      int xPos = timeDiff / 150; // Scroll speed

      // Display note
      if (xPos >= 0 && xPos < 16) {
        lcd.setCursor(xPos, 0);
        lcd.write(currentSong[i].type);
      }

      // Play the musical note when it hits the target time
      if (timeDiff <= 0 && !currentSong[i].soundPlayed) {
        tone(BUZZER_PIN, currentSong[i].frequency, 100);
        currentSong[i].soundPlayed = true;
      }

      // SUDDEN DEATH: If note passes without being hit
      if (timeDiff < -150) { 
        triggerGameOver("MISS! GAME OVER");
        return;
      }
    }
  }

  // Handle Input
  if (key == '4' || key == '6') {
    int hitType = (key == '4') ? NOTE_DON : NOTE_KA;
    checkHit(currentTime, hitType);
  }

  lcd.setCursor(0, 1);
  lcd.print("Score:"); lcd.print(score);

  if (allNotesDone) {
    delay(1000);
    triggerGameOver("SONG CLEAR!");
  }
}

void checkHit(unsigned long currentTime, int hitType) {
  for (int i = 0; i < currentSongLength; i++) {
    if (!currentSong[i].processed) {
      long diff = abs((long)currentTime - (long)currentSong[i].targetTime);
      
      if (diff < 200) { // Within hit window
        if (hitType == currentSong[i].type) {
          currentSong[i].processed = true;
          score += 100;
          return;
        } else {
          // SUDDEN DEATH: Wrong button
          triggerGameOver("WRONG! GAME OVER");
          return;
        }
      }
    }
  }
}

void triggerGameOver(String msg) {
  currentState = GAMEOVER;
  tone(BUZZER_PIN, 150, 500);
  lcd.clear();
  lcd.print(msg);
  lcd.setCursor(0, 1);
  lcd.print("Score:"); lcd.print(score);
  lcd.print("  [5]");
}

void showMenu() {
  currentState = MENU;
  lcd.clear();
  lcd.print("Pick: 1, 2, or 3");
  lcd.setCursor(0, 1);
  lcd.print("1:Twnkl 2:Jngl 3:Scl");
}
