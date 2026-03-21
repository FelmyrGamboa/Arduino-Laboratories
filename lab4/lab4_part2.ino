// Pin Definitions
const int seg[2][7] = {
  {2, 3, 4, 5, 6, 7, 8},        // Left Display Segments (A-G)
  {9, 10, 11, 12, 13, A0, A1}   // Right Display Segments (A-G)
};
const int ldr[2] = {A2, A3};    // LDR1 (Left), LDR2 (Right)

// Character Patterns (Common Anode: 0=ON, 1=OFF)
// Segments:            A B C D E F G
const byte charH     = 0b1001000;
const byte charE     = 0b0110000;
const byte charL     = 0b1110001;
const byte charO     = 0b0000001;
const byte charB     = 0b1100000; // lowercase b
const byte charY     = 0b1000100;
const byte char0     = 0b0000001; // Corrected for Common Anode '0'
const byte charBlank = 0b1111111;

int threshold = 500; 

void setup() {
  for (int d = 0; d < 2; d++) {
    for (int s = 0; s < 7; s++) {
      pinMode(seg[d][s], OUTPUT);
      digitalWrite(seg[d][s], HIGH); // Turn off all segments (Common Anode)
    }
  }
  pinMode(ldr[0], INPUT);
  pinMode(ldr[1], INPUT);
}

void writeToDisplay(int displayIdx, byte pattern) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(seg[displayIdx][i], bitRead(pattern, 6 - i));
  }
}

void loop() {
  int val1 = analogRead(ldr[0]);
  int val2 = analogRead(ldr[1]);

  bool ldr1Active = (val1 > threshold);
  bool ldr2Active = (val2 > threshold);

  // IF BOTH RECEIVE LIGHT OR BOTH DON'T, DISPLAY 00
  if (ldr1Active == ldr2Active) {
    writeToDisplay(0, char0);
    writeToDisplay(1, char0);
  } 
  
  // LDR 1 ONLY: HELLO (Reversed letters, Left to Right)
  else if (ldr1Active) {
    // Array order: O -> L -> L -> E -> H
    byte msg[] = {charO, charL, charL, charE, charH};
    int len = 5;
    
    for (int i = 0; i <= len; i++) {
      if (checkLDRChange(true)) return; 
      
      // Current letter on Left Display
      writeToDisplay(0, (i < len) ? msg[i] : charBlank); 
      // Previous letter shifts to Right Display
      writeToDisplay(1, (i > 0) ? msg[i-1] : charBlank);
      
      delay(400);
    }
  } 
  
  // LDR 2 ONLY: BYE (Right to Left)
  else if (ldr2Active) {
    byte msg[] = {charB, charY, charE};
    int len = 3;
    
    for (int i = 0; i <= len; i++) {
      if (checkLDRChange(false)) return; 
      
      // Current letter appears on Right Display
      writeToDisplay(1, (i < len) ? msg[i] : charBlank);
      // Moves to Left Display
      writeToDisplay(0, (i > 0) ? msg[i-1] : charBlank);
      
      delay(400);
    }
  }
}

bool checkLDRChange(bool lookingForLDR1) {
  int v1 = analogRead(ldr[0]);
  int v2 = analogRead(ldr[1]);
  bool l1 = v1 > threshold;
  bool l2 = v2 > threshold;
  
  if (lookingForLDR1) {
    return !(l1 && !l2); 
  } else {
    return !(!l1 && l2); 
  }
}