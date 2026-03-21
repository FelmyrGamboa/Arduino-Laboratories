#define switchPin A2

// Left and Right segment pin arrays
int segR[] = {2, 3, 4, 5, 6, 7, 8};
int segL[] = {9, 10, 11, 12, 13, A0, A1};

#define digitR A4
#define digitL A3

// Updated for Common Anode: 0 is ON, 1 is OFF
// Added values 10-15 (A, b, C, d, E, F) to complete the 4-bit range
int num[16][7] = {
  {0,0,0,0,0,0,1}, // 0
  {1,0,0,1,1,1,1}, // 1
  {0,0,1,0,0,1,0}, // 2
  {0,0,0,0,1,1,0}, // 3
  {1,0,0,1,1,0,0}, // 4
  {0,1,0,0,1,0,0}, // 5
  {0,1,0,0,0,0,0}, // 6
  {0,0,0,1,1,1,1}, // 7
  {0,0,0,0,0,0,0}, // 8
  {0,0,0,0,1,0,0}, // 9
  {0,0,0,1,0,0,0}, // A (10)
  {1,1,0,0,0,0,0}, // b (11)
  {0,1,1,1,0,0,1}, // C (12)
  {1,0,0,0,0,1,0}, // d (13)
  {0,1,1,0,0,0,0}, // E (14)
  {0,1,1,1,0,0,0}  // F (15)
};

int count = 0;
unsigned long t = 0;

void setup() {
  for(int i=0; i<7; i++){
    pinMode(segR[i], OUTPUT);
    pinMode(segL[i], OUTPUT);
  }
  pinMode(digitR, OUTPUT);
  pinMode(digitL, OUTPUT);
  // Use INPUT_PULLUP if switch connects to GND
  pinMode(switchPin, INPUT_PULLUP); 
}

void show(int n, int seg[], int digit){
  // For Common Anode, Digit pin must be HIGH to turn on
  digitalWrite(digit, HIGH); 
  for(int i=0; i<7; i++) {
    digitalWrite(seg[i], num[n][i]);
  }
  delay(5); // Slight delay for visibility
  digitalWrite(digit, LOW); // Turn off to prevent ghosting
}

void loop() {
  if(millis() - t > 1000){
    t = millis();
    // Assuming Switch to GND: LOW = UP, HIGH = DOWN
    if(digitalRead(switchPin) == LOW) {
      count = (count + 1) % 16;
    } else {
      count = (count + 15) % 16;
    }
  }

  // Display the two digits
  // For values 0-9, it shows 00-09. For 10-15, it shows 10-15.
  show(count / 10, segR, digitL);
  show(count % 10, segL, digitR);
}


// // Pin connected to the switch
// const int switchPin = A5;

// // Segment pins: a, b, c, d, e, f, g
// const int segmentPins[] = {2, 3, 4, 5, 6, 7, 8};

// // 2D Array for numbers 0-9
// // Each row represents a number. Columns represent segments [a, b, c, d, e, f, g]
// // 1 = HIGH (ON), 0 = LOW (OFF)
// const byte digitPatterns[10][7] = {
//   {0 , 0, 0, 0, 0, 0, 1}, // 0
//   {1, 0, 0, 1, 1, 1, 1}, // 1
//   {0, 0, 1, 0, 0, 1, 0}, // 2
//   {0, 0, 0, 0, 1, 1, 0}, // 3
//   {1, 0, 0, 1, 1, 0, 0}, // 4
//   {0, 1, 0, 0, 1, 0, 0}, // 5
//   {0, 1, 0, 0, 0, 0, 0}, // 6
//   {0, 0, 0, 1, 1, 1, 1}, // 7
//   {0, 0, 0, 0, 0, 0, 0}, // 8
//   {0, 0, 0, 0, 1, 0, 0}  // 9
// };

// void setup() {
//   // Initialize switch with internal pull-up resistor
//   pinMode(switchPin, INPUT_PULLUP);
  
//   // Initialize all segment pins as OUTPUT
//   for (int i = 0; i < 7; i++) {
//     pinMode(segmentPins[i], OUTPUT);
//   }
// }

// // Function to update the display
// void displayNumber(int number) {
//   for (int segment = 0; segment < 7; segment++) {
//     digitalWrite(segmentPins[segment], digitPatterns[number][segment]);
//   }
// }

// // Function to clear the display
// void clearDisplay() {
//   for (int i = 0; i < 7; i++) {
//     digitalWrite(segmentPins[i], HIGH);
//   }
// }

// void loop() {
//   // If switch is ON (LOW because of INPUT_PULLUP)
//   if (digitalRead(switchPin) == LOW) {
//     for (int i = 0; i <= 9; i++) {
//       // Re-check switch inside the loop so it stops immediately if turned off
//       if (digitalRead(switchPin) == HIGH) {
//         clearDisplay();
//         break; 
//       }
      
//       displayNumber(i);
//       delay(500); // Wait 0.5 seconds
//     }
//   } else {
//     clearDisplay();
//   }
// }