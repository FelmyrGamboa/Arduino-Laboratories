#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// ---------- Pin assignments ----------
const int sensorPins[] = {31, 33, 35, 37, 39, 41};      // IR sensors
const int microSwitchPins[] = {43, 45, 47};              // Micro switches
const int lightPins[]  = {22, 24, 26, 28, 30};           // Target/triangle LEDs
const int microLEDPins[] = {23, 25, 27};                 // Dedicated LEDs for micro switches
const int servoPin1    = 9;    // Eject servo 1 (1.5s hold)
const int servoPin2    = 8;    // Eject servo 2 (1s hold)
const int sweepServoPin = 10;  // SG-5010 sweep (only during PLAYING)
const int segA = 32, segB = 34, segC = 36, segD = 38, segE = 40, segF = 42, segG = 44; // 7‑seg
const int controlBtn = 6;     // Single start/reset button (active LOW)
const int BUZZER_PIN = 7;      // Passive buzzer

// ---------- Servo positions ----------
const int SERVO_HOME = 90;
const int SERVO_KICK = 0;

// ---------- Sweep settings ----------
const int SWEEP_MIN = 0;
const int SWEEP_MAX = 180;
const int SWEEP_STEP = 2;
const int SWEEP_INTERVAL = 20;

// ---------- Scoring ----------
const int POINTS_31 = 100, POINTS_33 = 100;
const int POINTS_35 = 200, POINTS_37 = 300, POINTS_39 = 400;
const int BONUS_ALL_TARGETS = 1000;

const unsigned long DEBOUNCE_MS = 200;
const unsigned long BUTTON_DEBOUNCE = 200;

enum GameState { IDLE, PLAYING, GAME_OVER };
GameState state = IDLE;

Servo servo1, servo2, sweepServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte charDon[8]  = {0b00000,0b01110,0b11111,0b11111,0b11111,0b01110,0b00000,0b00000};
byte charKa[8]   = {0b00000,0b01110,0b10001,0b10001,0b10001,0b01110,0b00000,0b00000};
byte charLeft[8] = {0b00111,0b01111,0b11111,0b11111,0b11111,0b01111,0b00111,0b00000};
byte charRight[8]= {0b11100,0b11110,0b11111,0b11111,0b11111,0b11110,0b11100,0b00000};

unsigned long score = 0;
int remainingBalls = 3;
bool lit[5] = {false};
unsigned long lastIRTrigger[6] = {0}, lastMicroTrigger[3] = {0};
int lastIRState[6], lastMicroState[3];

int sweepAngle = SWEEP_MIN;
bool sweepUp = true;
unsigned long lastSweepTime = 0;

unsigned long lastButtonTime = 0;

// 7-segment (common anode)
const byte digitPatterns[10] = {
  0b00111111, 0b00000110, 0b01011011, 0b01001111,
  0b01100110, 0b01101101, 0b01111101, 0b00000111,
  0b01111111, 0b01101111
};

// ---------- Background music (Tetris) ----------
struct Note {
  int freq;
  int duration;
};
const Note bgMelody[] = {
  {659, 150}, {494, 150}, {523, 150}, {587, 150},
  {659, 150}, {587, 150}, {523, 150}, {494, 150},
  {440, 150}, {440, 150}, {523, 150}, {659, 150},
  {587, 150}, {523, 150}, {494, 150}, {494, 150},
  {523, 150}, {587, 150}, {659, 150}, {523, 150},
  {440, 150}, {440, 300}
};
const int bgMelodyLength = sizeof(bgMelody)/sizeof(bgMelody[0]);
int bgNoteIndex = 0;
unsigned long bgNoteStartTime = 0;
bool bgMusicPlaying = false;

int savedFreq = 0;
unsigned long savedDuration = 0;
unsigned long savedElapsed = 0;
bool musicSaved = false;

// ---------- Non‑blocking SFX sequencer ----------
struct SfxNote {
  int freq;
  int duration;
  int pause;
};

const SfxNote sfxTriangle[] = {{600, 800, 0}};
const int sfxTriangleLen = 1;

const SfxNote sfxBoink[] = {{220, 300, 0}, {200, 300, 0}};
const int sfxBoinkLen = 2;

const SfxNote sfxDrain[] = {
  {330, 500, 20}, {294, 500, 20}, {262, 500, 20}, {196, 500, 0}
};
const int sfxDrainLen = 4;

const SfxNote sfxBonus[] = {
  {523, 500, 100}, {659, 500, 100}, {784, 500, 100}, {1047, 200, 0}
};
const int sfxBonusLen = 4;

// 3‑second classic "Game Over" descending melody
const SfxNote sfxGameOver[] = {
  {523, 300, 80},  // C5
  {494, 300, 80},  // B4
  {440, 300, 80},  // A4
  {392, 300, 80},  // G4
  {349, 300, 80},  // F4
  {330, 300, 80},  // E4
  {294, 300, 80},  // D4
  {262, 300, 0}    // C4
};
const int sfxGameOverLen = 8;

const SfxNote* currentSfx = nullptr;
int sfxLength = 0;
int sfxStep = 0;
unsigned long sfxStepStart = 0;
bool sfxPlaying = false;

bool gameOverSfxActive = false;

bool bonusPendingCleanup = false;
bool blinkState = false;
unsigned long lastBlinkToggle = 0;

enum ServeState { IDLE_SERVE, WAIT1, KICK, RETRACT2, RETRACT1, DONE };
ServeState serveState = IDLE_SERVE;
unsigned long serveTimer = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 6; i++) {
    pinMode(sensorPins[i], INPUT_PULLUP);
    lastIRState[i] = HIGH;
  }
  for (int i = 0; i < 3; i++) {
    pinMode(microSwitchPins[i], INPUT_PULLUP);
    lastMicroState[i] = HIGH;
  }
  for (int i = 0; i < 5; i++) {
    pinMode(lightPins[i], OUTPUT);
    digitalWrite(lightPins[i], LOW);
  }
  for (int i = 0; i < 3; i++) {
    pinMode(microLEDPins[i], OUTPUT);
    digitalWrite(microLEDPins[i], LOW);
  }

  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  sweepServo.attach(sweepServoPin);
  servo1.write(SERVO_HOME);
  servo2.write(SERVO_HOME);
  sweepServo.write(90);

  pinMode(segA, OUTPUT); pinMode(segB, OUTPUT); pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT); pinMode(segE, OUTPUT); pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  showDigit(remainingBalls);

  pinMode(controlBtn, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  lcd.init(); lcd.backlight(); lcd.clear();
  lcd.createChar(1, charDon); lcd.createChar(2, charKa);
  lcd.createChar(3, charLeft); lcd.createChar(4, charRight);

  updateDisplayForState();
}

void loop() {
  handleButton();
  updateSfxSequencer();

  if (state == PLAYING) {
    updateSweep();
    updateBackgroundMusic();
    updateServeBall();

    if (bonusPendingCleanup) {
      if (millis() - lastBlinkToggle >= 200) {
        blinkState = !blinkState;
        lastBlinkToggle = millis();
        digitalWrite(lightPins[2], blinkState);
        digitalWrite(lightPins[3], blinkState);
        digitalWrite(lightPins[4], blinkState);
        digitalWrite(microLEDPins[0], blinkState);
        digitalWrite(microLEDPins[1], blinkState);
        digitalWrite(microLEDPins[2], blinkState);
      }
    }

    for (int i = 0; i < 6; i++) {
      int reading = digitalRead(sensorPins[i]);
      if (reading == LOW && lastIRState[i] == HIGH && (millis() - lastIRTrigger[i] > DEBOUNCE_MS)) {
        lastIRTrigger[i] = millis();
        handleSensor(i);
      }
      lastIRState[i] = reading;
    }
    for (int i = 0; i < 3; i++) {
      int reading = digitalRead(microSwitchPins[i]);
      if (reading == LOW && lastMicroState[i] == HIGH && (millis() - lastMicroTrigger[i] > DEBOUNCE_MS)) {
        lastMicroTrigger[i] = millis();
        handleSensor(i + 2);
      }
      lastMicroState[i] = reading;
    }
  } else {
    sweepServo.write(90);
    servo1.write(SERVO_HOME);
    servo2.write(SERVO_HOME);
    bgMusicPlaying = false;
    if (!gameOverSfxActive) {
      noTone(BUZZER_PIN);
      sfxPlaying = false;
    }
  }
}

void handleButton() {
  unsigned long now = millis();
  if (digitalRead(controlBtn) == LOW && (now - lastButtonTime > BUTTON_DEBOUNCE)) {
    lastButtonTime = now;
    if (state == IDLE) startGame();
    else if (state == GAME_OVER) performReset();
  }
}

void performReset() {
  score = 0; remainingBalls = 3;
  for (int i = 0; i < 5; i++) lit[i] = false;
  turnOffAllLights();
  showDigit(remainingBalls);
  state = IDLE;
  servo1.write(SERVO_HOME); servo2.write(SERVO_HOME); sweepServo.write(90);
  bgMusicPlaying = false;
  noTone(BUZZER_PIN);
  sfxPlaying = false;
  serveState = IDLE_SERVE;
  bonusPendingCleanup = false;
  gameOverSfxActive = false;
  updateDisplayForState();
}

void startGame() {
  state = PLAYING;
  gameOverSfxActive = false;
  updateDisplayForState();
  sweepServo.write(sweepAngle);
  playStartJingle();
  bgMusicPlaying = true;
  bgNoteIndex = 0; bgNoteStartTime = millis();
  startNextMusicNote();
  serveBall();
}

void handleSensor(int idx) {
  switch (idx) {
    case 0: case 1:
      score += POINTS_31;
      lit[idx] = true;
      digitalWrite(lightPins[idx], HIGH);
      startSfx(sfxTriangle, sfxTriangleLen);
      break;
    case 2: case 3: case 4:
      if (!lit[idx]) {
        score += (idx==2 ? POINTS_35 : (idx==3 ? POINTS_37 : POINTS_39));
        lit[idx] = true;
        digitalWrite(lightPins[idx], HIGH);
        digitalWrite(microLEDPins[idx-2], HIGH);
      }
      startSfx(sfxBoink, sfxBoinkLen);
      break;
    case 5:
      handleDrain();
      return;
  }

  if (idx >= 2 && idx <= 4) {
    if (lit[2] && lit[3] && lit[4]) {
      score += BONUS_ALL_TARGETS;
      bonusPendingCleanup = true;
      blinkState = true; lastBlinkToggle = millis();
      startSfx(sfxBonus, sfxBonusLen);
      lcd.setCursor(0,0); lcd.print("ALL TARGET HITS!");
      lcd.setCursor(0,1); lcd.print("BONUS +1000     ");
      return;
    }
  }
  updateTopLineOnly();
}

void handleDrain() {
  turnOffAllLights();
  for (int i = 0; i < 5; i++) lit[i] = false;

  remainingBalls--;
  showDigit(remainingBalls);

  if (remainingBalls > 0) {
    lcd.setCursor(0,1);
    if (remainingBalls == 1) lcd.print("1 BALL LEFT!    ");
    else lcd.print(String(remainingBalls) + " BALLS LEFT!   ");
    startSfx(sfxDrain, sfxDrainLen);
    serveBall();
  } else {
    state = GAME_OVER;
    sweepServo.write(90);
    bgMusicPlaying = false;
    gameOverSfxActive = true;
    startSfx(sfxGameOver, sfxGameOverLen);
    updateDisplayForState();  // shows GAME OVER and PLAY AGAIN? PRESS
  }
  updateTopLineOnly();
}

void serveBall() {
  serveState = WAIT1;
  serveTimer = millis();
}

void updateServeBall() {
  if (serveState == IDLE_SERVE) return;
  unsigned long now = millis();
  switch (serveState) {
    case WAIT1:
      if (now - serveTimer >= 1000) {
        servo1.write(SERVO_KICK); servo2.write(SERVO_KICK);
        serveState = KICK; serveTimer = now;
      }
      break;
    case KICK:
      if (now - serveTimer >= 1000) {
        servo2.write(SERVO_HOME);
        serveState = RETRACT2; serveTimer = now;
      }
      break;
    case RETRACT2:
      if (now - serveTimer >= 500) {
        servo1.write(SERVO_HOME);
        serveState = RETRACT1; serveTimer = now;
      }
      break;
    case RETRACT1:
      if (now - serveTimer >= 400) {
        clearBottomLine();
        serveState = IDLE_SERVE;
      }
      break;
  }
}

void startNextMusicNote() {
  if (bgMusicPlaying) {
    int freq = bgMelody[bgNoteIndex].freq;
    unsigned long dur = bgMelody[bgNoteIndex].duration;
    if (freq > 0) tone(BUZZER_PIN, freq, dur);
    else noTone(BUZZER_PIN);
  }
}

void updateBackgroundMusic() {
  if (!bgMusicPlaying || state != PLAYING) return;
  if (sfxPlaying) return;

  unsigned long now = millis();
  if (now - bgNoteStartTime >= bgMelody[bgNoteIndex].duration) {
    bgNoteIndex = (bgNoteIndex + 1) % bgMelodyLength;
    bgNoteStartTime = now;
    startNextMusicNote();
  }
}

void startSfx(const SfxNote* notes, int length) {
  if (state == PLAYING && bgMusicPlaying && !sfxPlaying) {
    unsigned long now = millis();
    unsigned long elapsed = now - bgNoteStartTime;
    if (elapsed < bgMelody[bgNoteIndex].duration) {
      savedFreq = bgMelody[bgNoteIndex].freq;
      savedDuration = bgMelody[bgNoteIndex].duration;
      savedElapsed = elapsed;
      musicSaved = true;
    } else {
      musicSaved = false;
    }
  }
  noTone(BUZZER_PIN);
  currentSfx = notes; sfxLength = length; sfxStep = 0;
  sfxStepStart = millis(); sfxPlaying = true;

  if (sfxStep < sfxLength) {
    tone(BUZZER_PIN, currentSfx[sfxStep].freq, currentSfx[sfxStep].duration);
  }
}

void updateSfxSequencer() {
  if (!sfxPlaying) return;

  unsigned long now = millis();
  unsigned long stepTime = currentSfx[sfxStep].duration + currentSfx[sfxStep].pause;
  if (now - sfxStepStart >= stepTime) {
    sfxStep++;
    if (sfxStep >= sfxLength) {
      sfxPlaying = false;
      noTone(BUZZER_PIN);

      if (gameOverSfxActive) {
        gameOverSfxActive = false;
      }

      if (state == PLAYING && bgMusicPlaying && musicSaved) {
        unsigned long remaining = (savedDuration > savedElapsed) ? (savedDuration - savedElapsed) : 0;
        if (savedFreq > 0 && remaining > 0) {
          tone(BUZZER_PIN, savedFreq, remaining);
          bgNoteStartTime = millis() - savedElapsed;
        } else {
          bgNoteStartTime = millis();
          bgNoteIndex = (bgNoteIndex + 1) % bgMelodyLength;
          startNextMusicNote();
        }
        musicSaved = false;
      } else if (state == PLAYING && bgMusicPlaying && !musicSaved) {
        bgNoteStartTime = millis();
        startNextMusicNote();
      }

      if (bonusPendingCleanup) {
        digitalWrite(lightPins[2], LOW);
        digitalWrite(lightPins[3], LOW);
        digitalWrite(lightPins[4], LOW);
        digitalWrite(microLEDPins[0], LOW);
        digitalWrite(microLEDPins[1], LOW);
        digitalWrite(microLEDPins[2], LOW);
        lit[2] = false; lit[3] = false; lit[4] = false;
        clearBottomLine();
        updateTopLineOnly();
        bonusPendingCleanup = false;
      }
    } else {
      tone(BUZZER_PIN, currentSfx[sfxStep].freq, currentSfx[sfxStep].duration);
      sfxStepStart = millis();
    }
  }
}

void playStartJingle() {
  noTone(BUZZER_PIN);
  int notes[] = {262, 330, 392, 523};
  for (int i = 0; i < 4; i++) {
    tone(BUZZER_PIN, notes[i], 150);
    delay(150);
  }
  noTone(BUZZER_PIN);
}

void turnOffAllLights() {
  for (int i = 0; i < 5; i++) digitalWrite(lightPins[i], LOW);
  for (int i = 0; i < 3; i++) digitalWrite(microLEDPins[i], LOW);
}

void showDigit(int num) {
  if (num < 0 || num > 9) return;
  byte pattern = ~digitPatterns[num];
  digitalWrite(segA, bitRead(pattern,0));
  digitalWrite(segB, bitRead(pattern,1));
  digitalWrite(segC, bitRead(pattern,2));
  digitalWrite(segD, bitRead(pattern,3));
  digitalWrite(segE, bitRead(pattern,4));
  digitalWrite(segF, bitRead(pattern,5));
  digitalWrite(segG, bitRead(pattern,6));
}

void clearBottomLine() { lcd.setCursor(0,1); lcd.print("                "); }

void updateTopLineOnly() {
  lcd.setCursor(0,0);
  if (state == PLAYING) {
    lcd.print("Score: ");
    lcd.print(score);
    lcd.print("       ");
  }
}

void updateDisplayForState() {
  lcd.setCursor(0,0);
  switch (state) {
    case IDLE:
      lcd.print("PRESS START!    ");
      clearBottomLine();
      break;
    case PLAYING:
      updateTopLineOnly();
      break;
    case GAME_OVER:
      lcd.print("GAME OVER!      ");
      lcd.setCursor(0,1);
      lcd.print("PLAY AGAIN? PRESS");  // exactly 16 chars
      break;
  }
}

void updateSweep() {
  if (millis() - lastSweepTime >= SWEEP_INTERVAL) {
    lastSweepTime = millis();
    if (sweepUp) {
      sweepAngle += SWEEP_STEP;
      if (sweepAngle >= SWEEP_MAX) { sweepAngle = SWEEP_MAX; sweepUp = false; }
    } else {
      sweepAngle -= SWEEP_STEP;
      if (sweepAngle <= SWEEP_MIN) { sweepAngle = SWEEP_MIN; sweepUp = true; }
    }
    sweepServo.write(sweepAngle);
  }
}
