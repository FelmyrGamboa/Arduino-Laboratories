// Arrays for 5 parking slots
int irPin[] = {9, 10, 11, 12, 13};
int ledPin[] = {40, 41, 42, 43, 44};
int segPin[] = {2, 3, 4, 5, 6, 7, 8};

// Arrays for 2 Road Ultrasonic Sensors
int roadPintrig[] = {48, 50};
int roadPinecho[] = {49, 51};
int outPin = 52;

// 7-segment mapping (0 to 5)
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

  // Initialize 7-segment pins
  for(int i = 0; i < 7; i++){
    pinMode(segPin[i], OUTPUT);
  }

  // Initialize IR and LED pins for all 5 slots
  for(int i = 0; i < 5; i++){
    pinMode(irPin[i], INPUT);
    pinMode(ledPin[i], OUTPUT);
  }

  // Initialize Ultrasonic Pins and Output Pin
  for(int i = 0; i < 2; i++) {
    pinMode(roadPintrig[i], OUTPUT);
    pinMode(roadPinecho[i], INPUT);
  }
  pinMode(outPin, OUTPUT);
}

// Function to read distance from a specific ultrasonic sensor
int getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // Timeout after 30000us (~50cm range) to keep loop fast
  long duration = pulseIn(echo, HIGH, 30000); 
  if (duration == 0) return 100; // Return 100cm if nothing detected
  return duration * 0.034 / 2;
}

// Function to show a number on the 7-segment display
void updateDisplay(int count) {
  if (count < 0) count = 0;
  if (count > 5) count = 5;
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPin[i], num[count][i]);
  }
}

void loop() {
  // --- 1. PARKING SLOT LOGIC ---
  int availableSlots = 5; 
  for (int i = 0; i < 5; i++) {
    if (digitalRead(irPin[i]) == LOW) { 
      availableSlots--;          
      digitalWrite(ledPin[i], LOW); // Occupied
    } else {
      digitalWrite(ledPin[i], HIGH); // Free
    }
  }
  updateDisplay(availableSlots);

  // --- 2. DUAL ULTRASONIC ROAD LOGIC ---
  int dist1 = getDistance(roadPintrig[0], roadPinecho[0]);
  delay(10); // Tiny delay to prevent sensor cross-talk
  int dist2 = getDistance(roadPintrig[1], roadPinecho[1]);

  // Logic: Turn on outPin if EITHER sensor is 20cm or closer
  if ((dist1 > 0 && dist1 <= 40) || (dist2 > 0 && dist2 <= 15)) {
    digitalWrite(outPin, HIGH);
    Serial.print("ROAD: DETECTED! ");
  } else {
    digitalWrite(outPin, LOW);
    Serial.print("ROAD: CLEAR.    ");
  }

  // --- 3. DEBUGGING ---
  Serial.print("S1: "); Serial.print(dist1);
  Serial.print("cm | S2: "); Serial.print(dist2);
  Serial.print("cm | Slots: "); Serial.println(availableSlots);

  delay(100); 
}