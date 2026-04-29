int buzzerPin = 10;
int tonePins[] = {2, 3, 4, 5, 6, 7, 8, 9};
int notes[] = {262, 294, 330, 349, 392, 440, 494, 523};

void setup()
{
  pinMode(buzzerPin, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(tonePins[i], INPUT_PULLUP);
  }
}

void loop()
{
  int activeNote = -1;

  for (int i = 0; i < 8; i++) {
    if (digitalRead(tonePins[i]) == LOW) {
      activeNote = notes[i];
      break; 
    }
  }

  if (activeNote != -1) {
    tone(buzzerPin, activeNote);
  } else {
    noTone(buzzerPin);
  }
}