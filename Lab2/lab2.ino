float base = 0;
float height = 0;
float volume = 0;
int ledPin = 2;
int onDelay = 0;
int offDelay = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("=== Cone Volume Calculator ===");
  pinMode(ledPin, OUTPUT);
}

void calcu_vol(float base_val,float height_val) {
  volume = (base_val * height_val)/3;
  Serial.println(volume);
};

void loop() {
  Serial.println("Enter base area value:");
  while (Serial.available() == 0) {}
  base = Serial.parseFloat();
  while (Serial.available() > 0) {
    Serial.read();
  }
  
  while (base <= 0) {
    Serial.println("Invalid Base Area Value");
    delay(1000);
    Serial.println("Enter base area value:");
    while (Serial.available() == 0) {}
    base = Serial.parseFloat();
    while (Serial.available() > 0) {
    Serial.read();
    } 
  }

  String base_mess = String(base) + "cm";
  Serial.println(base_mess);
  delay(1000);

  while (Serial.available() > 0) {
  Serial.read();
  }
  
  Serial.println("Enter height value:");
  while (Serial.available() == 0) {}
  height = Serial.parseFloat();
  while (Serial.available() > 0) {
    Serial.read();
  }

  while (height <= 0) {
    Serial.println("Invalid Height Value");
    delay(1000);
    Serial.println("Enter hieght value:");
    while (Serial.available() == 0) {}
    height = Serial.parseFloat();
    while (Serial.available() > 0) {
    Serial.read();
    } 
  }

  String height_mess = String(height) + "cm";
  Serial.println(height_mess);
  delay(1000);

  while (Serial.available() > 0) {
  Serial.read();
  }

  calcu_vol(base, height);
  String vol_mess = "Volume =" + String(volume) + "cm";
  Serial.println(vol_mess);
  
  if (volume < 100) {
    Serial.println("Flashing: 5s ON, 5s OFF, 3 times");
    onDelay = 5000;
    offDelay = 5000;
  }
  else if (100 <= volume && volume <= 500){
    Serial.println("Flashing: 3s ON, 5s OFF, 3 times");
    onDelay = 3000;
    offDelay = 5000;
  }
  else if (volume > 500){
    Serial.println("Flashing: 1s ON, 3s OFF, 3 times");
    onDelay = 1000;
    offDelay = 3000;
  }
  else {}

  for (int i = 0; i < 3; i++){
    digitalWrite(ledPin,HIGH);
    delay(onDelay);
    digitalWrite(ledPin,LOW);
    delay(offDelay);
  }
}
