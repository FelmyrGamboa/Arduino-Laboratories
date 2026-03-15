// Laboratory 3
const int outPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int swPins[] = {A0, A1, A2, A3};
bool isDone = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9800);
  for (int i=0; i<8; i++) {
    pinMode(outPins[i], OUTPUT);
  }
  for (int i=0; i<4; i++) {
    pinMode(swPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (analogRead(swPins[0]) == LOW){
    isDone = false;
    pattern1();
  } else if (analogRead(swPins[1] == LOW)) {
    isDone = false;
    pattern2();
  } else if (analogRead(swPins[2] == LOW)) {
    isDone = false;
    pattern3();
  } else if (analogRead(swPins[3]) == LOW) {
    isDone = false;
    pattern4();
  } else {
    allLightsOff();
  }
}

void allLightsOff() {
  
}

void pattern1() {
  if (!isDone) {
    for (int j=0; j<8; j++) {
      digitalWrite(outPins[j], HIGH);
      delay(125);
      digitalWrite(outPins[j], LOW);
    }

    for (int k=6; k>=0; k--) {
      digitalWrite(outPins[k], HIGH);
      delay(125);
      digitalWrite(outPins[k], LOW);
    }
    isDone = true;
  }
}

void pattern2() {
  if (!isDone) {
    for (int j=0; j<8; j++) {
      digitalWrite(outPins[j], HIGH);
      digitalWrite(outPins[7-j], HIGH);
      delay(62.5);
      digitalWrite(outPins[j], LOW);
      digitalWrite(outPins[7-j], LOW);
    }

    for (int k=2; k>=0; k--) {
      digitalWrite(outPins[k], HIGH);
      digitalWrite(outPins[7-k], HIGH);
      delay(62.5);
      digitalWrite(outPins[k], LOW);
      digitalWrite(outPins[7-k], LOW);
    }
    isDone = true;
  }
}

void pattern3() {
  if (!isDone) {
    for (int j=0; j<4; j++) {
      digitalWrite(outPins[j], HIGH);
      digitalWrite(outPins[7-j], HIGH);
      delay(62.5);
    }

    for (int j=3; j>=0; j--) {
      digitalWrite(outPins[j], LOW);
      digitalWrite(outPins[7-j], LOW);
      delay(62.5);
    }

    isDone = true;
  }
}

void pattern4() {
  if (!isDone) {
    int ledPairs[][2] = {{5, 7}, {4, 6}, {3, 5}, {2, 4}, {1, 3}, {0, 2}};
    for (int j=0; j<6; j++) {
      digitalWrite(ledPairs[j][0], HIGH);
      digitalWrite(ledPairs[j][1], HIGH);
      delay(500);
    }
  }
}