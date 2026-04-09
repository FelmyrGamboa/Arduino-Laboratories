int irPin[] = {9, 10, 11, 12, 13};
int ledPin[] = {40, 41, 42, 43, 44};
int segPin[] = {2, 3, 4, 5, 6, 7, 8};

int roadPintrig[] = {48, 50};
int roadPinecho[] = {49, 51};
int outPin = 52;

int lastAvailableSlots = 5; 

int num[6][7] = {
  {0,0,0,0,0,0,1}, // 0
  {1,0,0,1,1,1,1}, // 1
  {0,0,1,0,0,1,0}, // 2
  {0,0,0,0,1,1,0}, // 3
  {1,0,0,1,1,0,0}, // 4
  {0,1,0,0,1,0,0}, // 5
};

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 7; i++) pinMode(segPin[i], OUTPUT);
  for(int i = 0; i < 5; i++) {
    pinMode(irPin[i], INPUT);
    pinMode(ledPin[i], OUTPUT);
  }
  for(int i = 0; i < 2; i++) {
    pinMode(roadPintrig[i], OUTPUT);
    pinMode(roadPinecho[i], INPUT);
  }
  pinMode(outPin, OUTPUT);
}

int getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000); 
  if (duration == 0) return 100;
  return duration * 0.034 / 2;
}

void updateDisplay(int count) {
  if (count < 0) count = 0;
  if (count > 5) count = 5;
  for (int i = 0; i < 7; i++) digitalWrite(segPin[i], num[count][i]);
}

void loop() {
  int availableSlots = 5; 
  for (int i = 0; i < 5; i++) {
    if (digitalRead(irPin[i]) == LOW) { 
      availableSlots--;          
      digitalWrite(ledPin[i], LOW); 
    } else {
      digitalWrite(ledPin[i], HIGH);
    }
  }
  updateDisplay(availableSlots);

  if (lastAvailableSlots == 0 && availableSlots > 0) {
    delay(500); 
  }
  lastAvailableSlots = availableSlots; 

  if (availableSlots > 0) {
    int dist1 = getDistance(roadPintrig[0], roadPinecho[0]);
    delay(10); 
    int dist2 = getDistance(roadPintrig[1], roadPinecho[1]);

    if ((dist1 > 0 && dist1 <= 30) || (dist2 > 0 && dist2 <= 15)) {
      digitalWrite(outPin, HIGH);
      Serial.print("ROAD: VEHICLE DETECTED! ON. ");
    } 
    else {
      if (digitalRead(outPin) == HIGH) {
        Serial.println("Clear! Waiting 2 seconds before turning off...");
        delay(2000); 
      }
      digitalWrite(outPin, LOW);
      Serial.print("ROAD: CLEAR. ");
    }
    
    Serial.print("S1: "); Serial.print(dist1);
    Serial.print("cm | S2: "); Serial.print(dist2);
    Serial.print("cm | ");
  } 
  else {
    digitalWrite(outPin, LOW);
    Serial.print("PARKING FULL: Walkway Disabled. ");
  }

  Serial.print("Slots: "); Serial.println(availableSlots);
  delay(100); 
}