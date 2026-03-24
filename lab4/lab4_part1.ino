#define switchPin A2

int segR[] = {2, 3, 4, 5, 6, 7, 8};
int segL[] = {9, 10, 11, 12, 13, A0, A1};

#define digitR A4
#define digitL A3

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
  pinMode(switchPin, INPUT_PULLUP); 
}

void show(int n, int seg[], int digit){
  for(int i=0; i<7; i++) {
    digitalWrite(seg[i], num[n][i]);
  }
  delay(5); 
  digitalWrite(digit, LOW); 
}

void loop() {
  if(millis() - t > 1000){
    t = millis();
    if(digitalRead(switchPin) == LOW) {
      count = (count + 1) % 16;
    } else {
      count = (count + 15) % 16;
    }
  }

  show(count / 10, segR, digitL);
  show(count % 10, segL, digitR);
}