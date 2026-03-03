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
  Serial.println("Counter Started. Reach 5 to finish!");
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
      delay(100); 
      digitalWrite(outputPin, LOW);

      // CHECK IF COUNTER REACHED 5
      if (counter >= 5) {
        Serial.println("ALL DONE!");
        
        // Optional: Keep the LED on to show it's finished
        digitalWrite(outputPin, LOW); 
        
        finished = true; // This stops the counter logic
      }
    }
    delay(50); // Debounce delay
  }
  lastButtonState = buttonState;
}