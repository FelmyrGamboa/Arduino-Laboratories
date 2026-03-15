// Laboratory 3
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int swPins[] = {10, 11, 12, 13};

unsigned long previousMillis = 0;
int step = 0;
bool patternFinished = false; // Ensures Sw 2, 3, 4 only run once
int lastActiveSwitch = -1;    // Tracks which switch was last used to detect changes

void setup() {
  for (int i = 0; i < 8; i++) pinMode(ledPins[i], OUTPUT);
  // Using INPUT_PULLUP: LOW = ON, HIGH = OFF
  for (int i = 0; i < 4; i++) pinMode(swPins[i], INPUT_PULLUP);
}

void loop() {
  int activeCount = 0;
  int currentSw = -1;

  // 1. Count how many switches are ON
  for (int i = 0; i < 4; i++) {
    if (digitalRead(swPins[i]) == LOW) {
      activeCount++;
      currentSw = i; // Store the index of the switch (0 to 3)
    }
  }

  // 2. Condition: ONLY light up if EXACTLY one switch is on
  if (activeCount == 1) {
    // If we just switched to a new switch, reset the pattern logic
    if (currentSw != lastActiveSwitch) {
      resetPattern();
      lastActiveSwitch = currentSw;
    }
    
    // Execute the pattern for the active switch
    runPattern(currentSw + 1); 
  } 
  else {
    // If 0 or >1 switches are on, turn everything off and reset
    allOff();
    resetPattern();
    lastActiveSwitch = -1;
  }
}

void resetPattern() {
  step = 0;
  patternFinished = false;
  previousMillis = millis();
}

void allOff() {
  for (int i = 0; i < 8; i++) digitalWrite(ledPins[i], LOW);
}

void runPattern(int swNumber) {
  unsigned long currentMillis = millis();
  int interval = (swNumber == 4) ? 500 : 125; // Sw 4 is 500ms, others 125ms to fit 1s cycle

  // If Sw 2, 3, or 4 has finished its cycle, stay off
  if (patternFinished && swNumber != 1) return;

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    switch (swNumber) {
      case 1: pattern1(); break;
      case 2: pattern2(); break;
      case 3: pattern3(); break;
      case 4: pattern4(); break;
    }
  }
}

// SWITCH 1: L-R (1s) and R-L (1s) Loop
void pattern1() {
  allOff();
  if (step < 8) digitalWrite(ledPins[step], HIGH);        // Left to Right
  else digitalWrite(ledPins[15 - step], HIGH);           // Right to Left
  
  step++;
  if (step >= 16) step = 0; // Loop back to start
}

// SWITCH 2: Inward-Outward (Once)
void pattern2() {
  allOff();
  if (step < 4) { // Inward (1&8 to 4&5)
    digitalWrite(ledPins[step], HIGH);
    digitalWrite(ledPins[7 - step], HIGH);
  } else if (step < 8) { // Outward (4&5 to 1&8)
    int outIdx = 7 - step; 
    digitalWrite(ledPins[outIdx], HIGH);
    digitalWrite(ledPins[7 - outIdx], HIGH);
  } else {
    patternFinished = true;
    allOff();
  }
  step++;
}

// SWITCH 3: Inward-Outward Retain (Once)
void pattern3() {
  if (step == 0) allOff(); 

  if (step < 4) { // Filling Inward
    digitalWrite(ledPins[step], HIGH);
    digitalWrite(ledPins[7 - step], HIGH);
  } else if (step < 8) { // Removing Outward
    int outIdx = step - 4; // 0, 1, 2, 3
    digitalWrite(ledPins[3 - outIdx], LOW);
    digitalWrite(ledPins[4 + outIdx], LOW);
  } else {
    patternFinished = true;
    allOff();
  }
  step++;
}

// SWITCH 4: Specific Pairs (Once)
void pattern4() {
  // Pairs: 8&6, 5&7, 4&6, 3&5, 2&4, 1&3
  // Indices: {7,5}, {4,6}, {3,5}, {2,4}, {1,3}, {0,2}
  int pairs[][2] = {{7,5}, {4,6}, {3,5}, {2,4}, {1,3}, {0,2}};
  
  allOff();
  if (step < 6) {
    digitalWrite(ledPins[pairs[step][0]], HIGH);
    digitalWrite(ledPins[pairs[step][1]], HIGH);
    step++;
  } else {
    patternFinished = true;
    allOff();
  }
}



// const int outPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
// const int swPins[] = {10, 11, 12, 13};
// bool isDone = false;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9800);
//   for (int i=0; i<8; i++) {
//     pinMode(outPins[i], OUTPUT);
//   }
//   for (int i=0; i<4; i++) {
//     pinMode(swPins[i], INPUT_PULLUP);
//   }
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   if (digitalRead(swPins[0]) == LOW){
//     isDone = false;
//     pattern1();
//   } else if (digitalRead(swPins[1]) == LOW) {
//     isDone = false;
//     pattern2();
//   } else if (digitalRead(swPins[2]) == LOW) {
//     isDone = false;
//     pattern3();
//   } else if (digitalRead(swPins[3]) == LOW) {
//     isDone = false;
//     pattern4();
//   } else {
//     allLightsOff();
//   }
// }

// void allLightsOff() {
//   for (int j=0; j<8; j++) {
//     digitalWrite(outPins[j], LOW);
//   }
//   return;
// }

// void pattern1() {
//   if (!isDone) {
//     for (int j=0; j<8; j++) {
//       digitalWrite(outPins[j], HIGH);
//       delay(125);
//       digitalWrite(outPins[j], LOW);
//     }

//     for (int k=6; k>=0; k--) {
//       digitalWrite(outPins[k], HIGH);
//       delay(125);
//       digitalWrite(outPins[k], LOW);
//     }
//     isDone = true;
//   }
//   return;
// }

// void pattern2() {
//   if (!isDone) {
//     for (int j=0; j<8; j++) {
//       digitalWrite(outPins[j], HIGH);
//       digitalWrite(outPins[7-j], HIGH);
//       delay(63);
//       digitalWrite(outPins[j], LOW);
//       digitalWrite(outPins[7-j], LOW);
//     }

//     for (int k=2; k>=0; k--) {
//       digitalWrite(outPins[k], HIGH);
//       digitalWrite(outPins[7-k], HIGH);
//       delay(63);
//       digitalWrite(outPins[k], LOW);
//       digitalWrite(outPins[7-k], LOW);
//     }
//     isDone = true;
//   }
//   return;
// }

// void pattern3() {
//   if (!isDone) {
//     for (int j=0; j<4; j++) {
//       digitalWrite(outPins[j], HIGH);
//       digitalWrite(outPins[7-j], HIGH);
//       delay(63);
//     }

//     for (int k=3; k>=0; k--) {
//       digitalWrite(outPins[k], LOW);
//       digitalWrite(outPins[7-k], LOW);
//       delay(63);
//     }

//     isDone = true;
//   }
//   return;
// }

// void pattern4() {
//   if (!isDone) {
//     int ledPairs[][2] = {{5, 7}, {4, 6}, {3, 5}, {2, 4}, {1, 3}, {0, 2}};
//     for (int j=0; j<6; j++) {
//       digitalWrite(ledPairs[j][0], HIGH);
//       digitalWrite(ledPairs[j][1], HIGH);
//       delay(500);
//     }
//   }
//   return;
// }