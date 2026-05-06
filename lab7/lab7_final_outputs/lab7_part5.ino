#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// --- LCD CONFIG ---
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- KEYPAD CONFIG ---
const byte ROWS = 4; 
const byte COLS = 3; 
char keys[ROWS][COLS] = {
  {'1','2','3'}, // 2 = DON, 8 = KA
  {'4','5','6'}, // 4 = HALF DON, 5 = MENU, 6 = HALF KA
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- PINS & CONSTANTS ---
const int BUZZER_PIN = 2;
#define NOTE_DON   1   
#define NOTE_KA    2   
#define NOTE_LEFT  3   // New Note
#define NOTE_RIGHT 4   // New Note

// Musical Frequencies
#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define A4  440
#define B4  494
#define C5  523

int menuIndex = 0;

const char* menuItems[] = {
  "1: Twinkle Star",
  "2: Jingle Bell",
  "3: Note Scale"
};

const int menuLength = 3;

struct Note {
  unsigned long targetTime;
  int frequency;
  int type; 
  bool processed;
};

const int MAX_NOTES = 25;
Note currentSong[MAX_NOTES];
int currentSongLength = 0;

enum GameState { MENU, COUNTDOWN, PLAYING, GAMEOVER };
GameState currentState = MENU;

unsigned long gameStartTime = 0;
int score = 0;

// --- CUSTOM CHARS ---
byte charDon[8] = {0b00000, 0b01110, 0b11111, 0b11111, 0b11111, 0b01110, 0b00000, 0b00000}; 
byte charKa[8]  = {0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000, 0b00000}; 
// Graphics for Left and Right halves
byte charLeft[8]  = {0b00111, 0b01111, 0b11111, 0b11111, 0b11111, 0b01111, 0b00111, 0b00000};
byte charRight[8] = {0b11100, 0b11110, 0b11111, 0b11111, 0b11111, 0b11110, 0b11100, 0b00000};

// --- SONG DATA ---
void loadSong(int choice) {
  randomSeed(analogRead(A1)); 
  score = 0;
  unsigned long startOffset = 800; 
  
  if (choice == 1) { // Twinkle Twinkle
    int mel[] = {C4, C4, G4, G4, A4, A4, G4, F4, F4, E4, E4, D4, D4, C4};
    currentSongLength = 14;
    for(int i=0; i<currentSongLength; i++) {
      // random(1, 5) picks 1(DON), 2(KA), 3(LEFT), or 4(RIGHT)
      currentSong[i] = { (unsigned long)(startOffset + (i * 1000)), mel[i], random(1, 5), false };
    }
  } 
  else if (choice == 2) { // Jingle Bells
    int mel[] = {E4, E4, E4, E4, E4, E4, E4, G4, C4, D4, E4};
    currentSongLength = 11;
    for(int i=0; i<currentSongLength; i++) {
      currentSong[i] = { (unsigned long)(startOffset + (i * 1000)), mel[i], random(1, 5), false };
    }
  }
  else if (choice == 3) { // Fast Scale
    currentSongLength = 8;
    int mel[] = {C4, D4, E4, F4, G4, A4, B4, C5};
    for(int i=0; i<currentSongLength; i++) {
      currentSong[i] = { (unsigned long)(startOffset + (i * 800)), mel[i], random(1, 5), false };
    }
  }
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, charDon);
  lcd.createChar(2, charKa);
  lcd.createChar(3, charLeft);
  lcd.createChar(4, charRight);
  showWelcome();
  delay(1000);
  showMenu();
}

void loop() {
  char key = keypad.getKey();

  switch (currentState) {
    case MENU:
      if (key == '#') { 
        if (menuIndex < menuLength - 1) {
          menuIndex++;
          tone(BUZZER_PIN, 500, 50);
          showMenu();
        }
      }
      if (key == '*') { 
        if (menuIndex > 0) {
          menuIndex--;
          tone(BUZZER_PIN, 250, 50);
          showMenu();
        }
      }
      if (key == '5') { 
        loadSong(menuIndex + 1);
        currentState = COUNTDOWN;
      }
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
    tone(BUZZER_PIN, 440, 50);
    delay(600); 
  }
  gameStartTime = millis();
  currentState = PLAYING;
}

void updateGame(char key) {
  unsigned long currentTime = millis() - gameStartTime;
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(">"); 

  bool allNotesDone = true;

  for (int i = 0; i < currentSongLength; i++) {
    if (!currentSong[i].processed) {
      allNotesDone = false;
      long timeDiff = (long)currentSong[i].targetTime - (long)currentTime;
      int xPos = timeDiff / 125; 

      if (xPos >= 0 && xPos < 16) {
        lcd.setCursor(xPos, 0);
        lcd.write(currentSong[i].type);
      }

      if (timeDiff < -100) { 
        triggerGameOver("MISS! GAME OVER");
        return;
      }
    }
  }

  // Handle Input for all 4 note types
  if (key != NO_KEY) {
    int hitType = 0;
    if (key == '2') hitType = NOTE_DON;
    else if (key == '8') hitType = NOTE_KA;
    else if (key == '4') hitType = NOTE_LEFT;
    else if (key == '6') hitType = NOTE_RIGHT;

    if (hitType != 0) checkHit(currentTime, hitType);
  }

  lcd.setCursor(0, 1);
  lcd.print("Score:"); lcd.print(score);

  if (allNotesDone) {
    delay(800);
    triggerGameOver("SONG CLEAR!");
  }
}

void checkHit(unsigned long currentTime, int hitType) {
  int bestIndex = -1;
  long bestDiff = 999999;

  for (int i = 0; i < currentSongLength; i++) {
    if (!currentSong[i].processed) {
      long diff = abs((long)currentTime - (long)currentSong[i].targetTime);
      if (diff < bestDiff) {
        bestDiff = diff;
        bestIndex = i;
      }
    }
  }

  if (bestIndex != -1 && bestDiff < 250) {
    if (hitType == currentSong[bestIndex].type) {
      tone(BUZZER_PIN, currentSong[bestIndex].frequency, 150);
      currentSong[bestIndex].processed = true;
      score += 100;
    } else {
      triggerGameOver("WRONG! GAME OVER");
    }
  }
}

void triggerGameOver(String msg) {
  currentState = GAMEOVER;
  menuIndex = 0;
  if (msg != "SONG CLEAR!") {
    tone(BUZZER_PIN, 100, 400); 
  } else {
    tone(BUZZER_PIN, 880, 200); 
  }
  lcd.clear();
  lcd.print(msg);
  lcd.setCursor(0, 1);
  lcd.print("Score:"); lcd.print(score);
  lcd.print("  [5]");
}

void showWelcome() {
  lcd.clear();
  lcd.print("Welcome to");
  lcd.setCursor(0,1);
  lcd.print("Arduino Taiko");

  int melody[] = {C4, D4, E4, F4, G4, A4, B4, C5};
  int duration[] = {150, 150, 150, 150, 200, 200, 200, 300}; 

  for (int i = 0; i < 8; i++) {
    tone(BUZZER_PIN, melody[i], duration[i]);
    delay(duration[i] + 50); 
  }
}

void showMenu() {
  currentState = MENU;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose Song:");
  lcd.setCursor(0, 1);
  lcd.print(">");
  lcd.print(menuItems[menuIndex]);
}