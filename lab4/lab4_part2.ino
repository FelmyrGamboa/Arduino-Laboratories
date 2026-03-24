// Pin Definitions
const int seg[2][7] = {
  {2, 3, 4, 5, 6, 7, 8},        
  {9, 10, 11, 12, 13, A0, A1}   
};
const int ldr[2] = {A2, A3};    


const byte charH     = 0b1001000;
const byte charE     = 0b0110000;
const byte charL     = 0b1110001;
const byte charO     = 0b0000001;
const byte charB     = 0b1100000; 
const byte charY     = 0b1000100;
const byte char0     = 0b0000001; 
const byte charBlank = 0b1111111;

int threshold = 500; 

void setup() {
  for (int d = 0; d < 2; d++) {
    for (int s = 0; s < 7; s++) {
      pinMode(seg[d][s], OUTPUT);
      digitalWrite(seg[d][s], HIGH); 
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


  if (ldr1Active == ldr2Active) {
    writeToDisplay(0, char0);
    writeToDisplay(1, char0);
  } 
  
 
  else if (ldr1Active) {
    byte msg[] = {charO, charL, charL, charE, charH};
    int len = 5;
    
    for (int i = 0; i <= len; i++) {
      if (checkLDRChange(true)) return; 
      
      writeToDisplay(0, (i < len) ? msg[i] : charBlank); 
      writeToDisplay(1, (i > 0) ? msg[i-1] : charBlank);
      
      delay(400);
    }
  } 
  
  else if (ldr2Active) {
    byte msg[] = {charB, charY, charE};
    int len = 3;
    
    for (int i = 0; i <= len; i++) {
      if (checkLDRChange(false)) return; 
      
      writeToDisplay(1, (i < len) ? msg[i] : charBlank);
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