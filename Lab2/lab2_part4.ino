const int buttonPin = 2;    
const int outputPin = 3;    

int counter = 0;            
int buttonState = 0;        
int lastButtonState = 0;    
bool finished = false;     // Track if we are done

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(outputPin, OUTPUT);      
  Serial.begin(9600);              
  Serial.println("\nCounter Started. Reach 5 to finish!");
}

void loop() {
  // If finished is true, the rest of the code is ignored
  if (finished) {
    return; 
  }

  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      counter++;
      Serial.print("Count: ");
      Serial.println(counter);

      // Flash LED for feedback
      digitalWrite(outputPin, HIGH);
      Serial.println("Button Pressed");
      delay(500);
      while (digitalRead(buttonPin) == LOW);
      delay(100); 
      digitalWrite(outputPin, LOW);
      Serial.println("Button Released");
      delay(500);
      // CHECK IF COUNTER REACHED 5
      if (counter >= 5) {
        Serial.println("ALL DONE!");
        
        // Optional: Keep the LED on to show it's finished
        digitalWrite(outputPin, LOW); 
        
        finished = true; // This stops the counter logic
      }
    }
    delay(250); // Debounce delay
  }
  lastButtonState = buttonState;
}