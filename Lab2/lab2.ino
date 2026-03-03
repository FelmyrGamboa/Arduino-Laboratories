float base = 0;
float height = 0;
float volume = 0;
int ledPin = 2;
int onDelay = 0;
int offDelay = 0;
bool runCalc = true;
bool choicePrompt = false;
String userInput = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(100);
  Serial.println("\n=== Cone Volume Calculator ===");
  pinMode(ledPin, OUTPUT);
}

void calcuVol(float base_val,float height_val) {
  volume = (base_val * height_val)/3;

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
};

void clearBuffer() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void loop() {
  while (runCalc) {
    Serial.println("Enter base area value:");
    while (Serial.available() == 0) {}
    base = Serial.parseFloat();
    clearBuffer();

    while (base <= 0) {
      clearBuffer();
      Serial.println("Invalid Base Area Value");
      delay(1000);
      Serial.println("Enter base area value:");
      while (Serial.available() == 0) {}
      base = Serial.parseFloat();
    }

    String base_mess = "Base: " + String(base) + " cm²";
    Serial.println(base_mess);
    delay(1000);

    clearBuffer();

    Serial.println("Enter height value:");
    while (Serial.available() == 0) {}
    height = Serial.parseFloat();
    clearBuffer();

    while (height <= 0) {
      clearBuffer();
      Serial.println("Invalid Height Value");
      delay(1000);
      Serial.println("Enter height value:");
      while (Serial.available() == 0) {}
      height = Serial.parseFloat();
    }

    String height_mess = "Height: " + String(height) + " cm";
    Serial.println(height_mess);
    delay(1000);
    clearBuffer();
    

    calcuVol(base, height);
    String vol_mess = ">>> Volume = " + String(volume) + " cm³";
    Serial.println(vol_mess);

    for (int i = 0; i < 3; i++){
      digitalWrite(ledPin,HIGH);
      delay(onDelay);
      digitalWrite(ledPin,LOW);
      delay(offDelay);
    }

    while (!choicePrompt){
      clearBuffer();
      Serial.println("Do you want to find the volume of another cone?");
      Serial.println("Yes - ['0' | 'yes'] ; No - ['1' | 'no']");

      while (Serial.available() == 0) {}
      userInput = Serial.readString();
      userInput.trim();
      userInput.toLowerCase();

      if (userInput == "1" || userInput == "no" || userInput == "n") {
        Serial.println("\nGoodbye!");
        runCalc = false;
        break;
      } else if (userInput == "0" || userInput == "yes" || userInput == "y"){
        Serial.println("Restarting......\n");
        Serial.println("\n=== Cone Volume Calculator ===");
        break;
      } else {
        Serial.print("==================================\n");
        Serial.println("Invalid entry, try again.....");
        Serial.print("==================================\n");
      }
    }
  }
}
