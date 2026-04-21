// C++ code
//
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 3;

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};                    

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
                                            
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

String keyInput = "";

void setup()
{
  Serial.begin(9600);
  menuPrint();
}

void loop()
{
  char keyInput = 0;
  char input;
  
  while (keyInput == 0) {
   	keyInput = customKeypad.getKey();
  }
    
  if (keyInput == '*' || keyInput == '#') {
    Serial.println(keyInput); 
   	menuPrint();
  }
    
  else if (keyInput == '1') {
    input = 'Y';
    while (input == 'Y' || input == 'y') {
      runInput1();
      bool isDone = false;
      while (!isDone) {
        Serial.println(F("Do you want to try again? Y/N"));
        while (Serial.available() == 0) {}
        input = (char)Serial.read();
        delay(10);
        while (Serial.available()) Serial.read();
        
        if (input == 'Y' || input == 'y' || input == 'N' || input == 'n') {
          isDone = true;
        } else {
          Serial.println(F("Invalid input! Please enter only 'Y' or 'N'."));
        }
      }


    }
    menuPrint();
  }

  else if (keyInput == '2') {
    input = 'Y';
    while (input == 'Y' || input == 'y') {
      runInput2();
      bool isDone = false;
      while (!isDone) {
        Serial.println(F("Do you want to try again? Y/N"));
        while (Serial.available() == 0) {}
        input = (char)Serial.read();
        delay(10);
        while (Serial.available()) Serial.read();
        if (input == 'Y' || input == 'y' || input == 'N' || input == 'n') {
          isDone = true;
        } else {
          Serial.println(F("Invalid input! Please enter only 'Y' or 'N'."));
        }
      }
    }
    menuPrint();
  }

  else if (keyInput == '3') {
    input = 'Y';
    while (input == 'Y' || input == 'y') {
      runInput3();
      bool isDone = false;
      while (!isDone) {
        Serial.println(F("Do you want to try again? Y/N"));
        while (Serial.available() == 0) {}
        input = (char)Serial.read();
        delay(10);
        while (Serial.available()) Serial.read();
        if (input == 'Y' || input == 'y' || input == 'N' || input == 'n') {
          isDone = true;
        } else {
          Serial.println(F("Invalid input! Please enter only 'Y' or 'N'."));
        }
      }
    }
    menuPrint();
  }

  else if (keyInput == '4') {
    input = 'Y';
    while (input == 'Y' || input == 'y') {
      runInput4();
      bool isDone = false;
      while (!isDone) {
        Serial.println(F("Do you want to try again? Y/N"));
        while (Serial.available() == 0) {}
        input = (char)Serial.read();
        delay(10);
        while (Serial.available()) Serial.read();
        if (input == 'Y' || input == 'y' || input == 'N' || input == 'n') {
          isDone = true;
        } else {
          Serial.println(F("Invalid input! Please enter only 'Y' or 'N'."));
        }
      }
    }
    menuPrint();
  }

  else if (keyInput == '5') {
    input = 'Y';
    while (input == 'Y' || input == 'y') {
      runInput5();
      bool isDone = false;
      while (!isDone) {
        Serial.println(F("Do you want to try again? Y/N"));
        while (Serial.available() == 0) {}
        input = (char)Serial.read();
        delay(10);
        while (Serial.available()) Serial.read();
        if (input == 'Y' || input == 'y' || input == 'N' || input == 'n') {
          isDone = true;
        } else {
          Serial.println(F("Invalid input! Please enter only 'Y' or 'N'."));
        }
      }
    }
    menuPrint();
  }

  else if (keyInput == '6') {
    input = 'Y';
    while (input == 'Y' || input == 'y') {
      runInput6();
      bool isDone = false;
      while (!isDone) {
        Serial.println(F("Do you want to try again? Y/N"));
        while (Serial.available() == 0) {}
        input = (char)Serial.read();
        delay(10);
        while (Serial.available()) Serial.read();
        if (input == 'Y' || input == 'y' || input == 'N' || input == 'n') {
          isDone = true;
        } else {
          Serial.println(F("Invalid input! Please enter only 'Y' or 'N'."));
        }
      }
    }
    menuPrint();
  }

  else if (keyInput == '7') {
    input = 'Y';
    while (input == 'Y' || input == 'y') {
      runInput7();
      bool isDone = false;
      while (!isDone) {
        Serial.println(F("Do you want to try again? Y/N"));
        while (Serial.available() == 0) {}
        input = (char)Serial.read();
        delay(10);
        while (Serial.available()) Serial.read();
        if (input == 'Y' || input == 'y' || input == 'N' || input == 'n') {
          isDone = true;
        } else {
          Serial.println(F("Invalid input! Please enter only 'Y' or 'N'."));
        }
      }
    }
    menuPrint();
  }

  else if (keyInput == '8') {
    input = 'Y';
    while (input == 'Y' || input == 'y') {
      runInput8();
      bool isDone = false;
      while (!isDone) {
        Serial.println(F("Do you want to try again? Y/N"));
        while (Serial.available() == 0) {}
        input = (char)Serial.read();
        delay(10);
        while (Serial.available()) Serial.read();
        if (input == 'Y' || input == 'y' || input == 'N' || input == 'n') {
          isDone = true;
        } else {
          Serial.println(F("Invalid input! Please enter only 'Y' or 'N'."));
        }
      }
    }
    menuPrint();
  }

  else if (keyInput == '9') {
    input = 'Y';
    while (input == 'Y' || input == 'y') {
      runInput9();
      bool isDone = false;
      while (!isDone) {
        Serial.println(F("Do you want to try again? Y/N"));
        while (Serial.available() == 0) {}
        input = (char)Serial.read();
        delay(10);
        while (Serial.available()) Serial.read();
        if (input == 'Y' || input == 'y' || input == 'N' || input == 'n') {
          isDone = true;
        } else {
          Serial.println(F("Invalid input! Please enter only 'Y' or 'N'."));
        }
      }
    }
    menuPrint();
  }
  
  else { 
    Serial.println("INVALID CODE!!!");
    delay(500);
    menuPrint();
  }
}

void menuPrint() {
   Serial.println();
  Serial.println(F("=============================="));
  Serial.println(F("  <<<<<Choose your payter>>>>>"));
  Serial.println(F("=============================="));
  Serial.println(F("1. Pyramid of Stars"));
  Serial.println(F("2. Draw a Square"));
  Serial.println(F("3. Fibonacci Number Series"));
  Serial.println(F("4. Multiplication Table (up to x10)"));
  Serial.println(F("5. Surface Area of a Cone"));
  Serial.println(F("6. Print Louis Vuitton Logo"));
  Serial.println(F("7. Light 8 LEDs left to right (3s each)"));
  Serial.println(F("8. Blink left 4, then right 4 LEDs"));
  Serial.println(F("9. Error + Blink all LEDs for 2s"));
  Serial.println(F("------------------------------"));
  Serial.println(F("Press a key on the keypad..."));
}


void runInput1() {
  Serial.println("\nEnter the height of the pyramid:");
  while (Serial.available() == 0) {}
  int height = Serial.parseInt();

  if (height > 0) {
    Serial.print("Generating pyramid with height: ");
    Serial.println(height);
    Serial.println();
    for (int i = 1; i <= height; i++) {
      for (int j = 1; j <= (height - i); j++) {
        Serial.print(" ");
      }
      for (int k = 1; k <= (2 * i - 1); k++) {
        Serial.print("*");
      }
      Serial.println();
    }
  } else {
    Serial.println("Invalid input. Please enter a positive number.");
  }
  while (Serial.available() > 0) {Serial.read();  }
}

void runInput2() {
  Serial.println("\nEnter the side length (height/width) of the square:");
  while (Serial.available() == 0) {}
  int side = Serial.parseInt();

  if (side > 0) {
    Serial.print("Generating a ");
    Serial.print(side);
    Serial.println("x");
    Serial.print(side);
    Serial.println(" square:");
    Serial.println();

    for (int i = 0; i < side; i++) {
      for (int j = 0; j < side; j++) {
        Serial.print("* ");
      Serial.println();
      }
    }
  } else {
    Serial.println("Please enter a valid positive number.");
  }
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void runInput3() {
  Serial.println("\nEnter the number of terms you want to see:");
  while (Serial.available() == 0) {}
  int n = Serial.parseInt();

  if (n <= 0) {
    Serial.println("Please enter a positive number greater than 0.");
  } else {
    Serial.print("Fibonacci Series (");
    Serial.print(n);
    Serial.println(" terms):");

    unsigned long t1 = 0;
    unsigned long t2 = 1; 
    unsigned long nextTerm = 0;

    for (int i = 1; i <= n; i++) {
      if (i == 1) {
        Serial.print(t1);
      } else if (i == 2) {
        Serial.print(t2);
      } else {
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
        Serial.print(nextTerm);
      }
      if (i < n) {
        Serial.print(", ");
      }
    }
    Serial.println(); 
  }
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void runInput4() {
  Serial.println("\nEnter a number:");
  long num;
  bool valid = false;

  while (!valid) {
    while (Serial.available() == 0) {}
    String input = Serial.readStringUntil('\n');
    input.trim();
    bool isNumeric = true;
    for (int i = 0; i < input.length(); i++) {
      if (!isDigit(input[i])) {
        isNumeric = false;
        break;
      }
    }
    if (isNumeric && input.length() > 0) {
      num = input.toInt();
      if (num > 0) {
        valid = true;
      } else {
        Serial.println("Invalid input! Please enter a positive number.");
        Serial.println("\nEnter a number:");
      }
    } else {
      Serial.println("Invalid input! No letters or symbols allowed.");
      Serial.println("\nEnter a number:");
    }
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
  Serial.println();
  Serial.print("Multiplication Table for: ");
  Serial.println(num);
  Serial.println("=============================");
  for (int i = 1; i <= 10; i++) {
    Serial.print(num);
    Serial.print(" x ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(num * i);
  }
  Serial.println("=============================");
}

void runInput5() {
  float radius = 0, height = 0, slantHeight, surfaceArea;

  // 1. Get Radius (Validated)
  while (true) {
    Serial.println(F("\nEnter a POSITIVE Radius (r):"));
    while (Serial.available() == 0) {}

    String input = Serial.readStringUntil('\n');
    input.trim();

    bool isValid = true;

    if (input.length() == 0) isValid = false;

    for (int i = 0; i < input.length(); i++) {
      if (!(isDigit(input[i]) || input[i] == '.')) {
        isValid = false;
        break;
      }
    }

    if (isValid) {
      radius = input.toFloat();

      if (radius > 0) {
        Serial.print(F("Radius accepted: "));
        Serial.println(radius);
        break;
      }
    }

    Serial.println(F(">> Invalid Input! Please enter a positive number (no symbols/letters). <<"));
  }

  // 2. Get Height (Validated)
  while (true) {
    Serial.println(F("\nEnter a POSITIVE Vertical Height (h):"));
    while (Serial.available() == 0) {}

    String input = Serial.readStringUntil('\n');
    input.trim();

    bool isValid = true;

    if (input.length() == 0) isValid = false;

    for (int i = 0; i < input.length(); i++) {
      if (!(isDigit(input[i]) || input[i] == '.')) {
        isValid = false;
        break;
      }
    }

    if (isValid) {
      height = input.toFloat();

      if (height > 0) {
        Serial.print(F("Height accepted: "));
        Serial.println(height);
        break;
      }
    }

    Serial.println(F(">> Invalid Input! Please enter a positive number (no symbols/letters). <<"));
  }

  // 3. Calculations
  slantHeight = sqrt(pow(radius, 2) + pow(height, 2));
  surfaceArea = PI * radius * (radius + slantHeight);

  // 4. Output Results
  Serial.println(F("\n--------------------------------"));
  Serial.print(F("Slant Height (s): "));
  Serial.println(slantHeight);
  Serial.print(F("Total Surface Area: "));
  Serial.print(surfaceArea);
  Serial.println(F(" sq units"));
  Serial.println(F("--------------------------------"));
}

void runInput6() {

}

void runInput7() {

}

void runInput8() {

}

void runInput9() {
  
}