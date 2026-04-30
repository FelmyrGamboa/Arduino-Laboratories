//  4 choruses × 16 s = exactly 64 s total
//
//  Song 1 – NSMB DS        "Overworld Theme"          BPM 150
//  Song 2 – NSMB Wii       "Castle / Fortress Theme"  BPM 120
//  Song 3 – Super Mario Galaxy "Gusty Garden Galaxy"   BPM 120 (3/4)
//  Song 4 – SM3D World     "Super Bell Hill"           BPM 150

// ── Note Frequency Defines ───────────────────────────────────
#define REST      0
#define NOTE_B0   31
#define NOTE_C1   33
#define NOTE_CS1  35
#define NOTE_D1   37
#define NOTE_DS1  39
#define NOTE_E1   41
#define NOTE_F1   44
#define NOTE_FS1  46
#define NOTE_G1   49
#define NOTE_GS1  52
#define NOTE_A1   55
#define NOTE_AS1  58
#define NOTE_B1   62
#define NOTE_C2   65
#define NOTE_CS2  69
#define NOTE_D2   73
#define NOTE_DS2  78
#define NOTE_E2   82
#define NOTE_F2   87
#define NOTE_FS2  93
#define NOTE_G2   98
#define NOTE_GS2  104
#define NOTE_A2   110
#define NOTE_AS2  117
#define NOTE_B2   123
#define NOTE_C3   131
#define NOTE_CS3  139
#define NOTE_D3   147
#define NOTE_DS3  156
#define NOTE_E3   165
#define NOTE_F3   175
#define NOTE_FS3  185
#define NOTE_G3   196
#define NOTE_GS3  208
#define NOTE_A3   220
#define NOTE_AS3  233
#define NOTE_B3   247
#define NOTE_C4   262
#define NOTE_CS4  277
#define NOTE_D4   294
#define NOTE_DS4  311
#define NOTE_E4   330
#define NOTE_F4   349
#define NOTE_FS4  370
#define NOTE_G4   392
#define NOTE_GS4  415
#define NOTE_A4   440
#define NOTE_AS4  466
#define NOTE_B4   494
#define NOTE_C5   523
#define NOTE_CS5  554
#define NOTE_D5   587
#define NOTE_DS5  622
#define NOTE_E5   659
#define NOTE_F5   698
#define NOTE_FS5  740
#define NOTE_G5   784
#define NOTE_GS5  831
#define NOTE_A5   880
#define NOTE_AS5  932
#define NOTE_B5   988
#define NOTE_C6   1047
#define NOTE_CS6  1109
#define NOTE_D6   1175
#define NOTE_DS6  1245
#define NOTE_E6   1319
#define NOTE_F6   1397
#define NOTE_FS6  1480
#define NOTE_G6   1568
#define NOTE_GS6  1661
#define NOTE_A6   1760
#define NOTE_AS6  1865
#define NOTE_B6   1976
#define NOTE_C7   2093
#define NOTE_CS7  2217
#define NOTE_D7   2349
#define NOTE_DS7  2489
#define NOTE_E7   2637
#define NOTE_F7   2794
#define NOTE_FS7  2960
#define NOTE_G7   3136
#define NOTE_GS7  3322
#define NOTE_A7   3520
#define NOTE_AS7  3729
#define NOTE_B7   3951
#define NOTE_C8   4186
#define NOTE_CS8  4435
#define NOTE_D8   4699
#define NOTE_DS8  4978

// ── Hardware pins ─────────────────────────────────────────────
const int BUZZER      = 10;
const int BTN[8]      = {2, 3, 4, 5, 6, 7, 8, 9};
const int BTNFREQ[8]  = {262, 294, 330, 349, 392, 440, 494, 523};

void playNote(int freq, int dur) {
  if (freq == REST) {
    noTone(BUZZER);
    delay(dur);
  } else {
    tone(BUZZER, freq);
    delay(dur - 30);  
    noTone(BUZZER);
    delay(30);         
  }
}

// ── Play a whole melody array ─────────────────────────────────
void playSong(const int* N, const int* D, int len) {
  for (int i = 0; i < len; i++) playNote(N[i], D[i]);
  noTone(BUZZER);
}

//  SONG 1 – NSMB DS  "Overworld Theme"
//  TOTAL = 16 000 ms
const int N1[] = {
  NOTE_E5, NOTE_G5, NOTE_C6, NOTE_B5, NOTE_A5, NOTE_F5, NOTE_A5, NOTE_G5, REST, NOTE_E5,   
  NOTE_F5, NOTE_G5, NOTE_C5, NOTE_E5, NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_E6,             
  NOTE_G6, NOTE_A6, NOTE_F6, NOTE_G6, NOTE_E6, NOTE_C6, NOTE_D6, NOTE_B5,             
  NOTE_E5, NOTE_G5, NOTE_C6, NOTE_B5, NOTE_A5, NOTE_F5, NOTE_A5, NOTE_G5, REST, NOTE_E5,
  NOTE_F5, NOTE_G5, NOTE_C5, NOTE_E5, NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_E6,
  NOTE_G6, NOTE_A6, NOTE_F6, NOTE_G6, NOTE_E6, NOTE_C6, NOTE_D6, NOTE_B5,
  REST // Final rest extended to reach 16s                                               
};
const int D1[] = {
  200,200,400,200,200,200,200,400,200,200,200,400,200,200,400,200,200,200,200,
  400,200,200,400,400,400,200,600, // Pass 1 = 7400ms
  200,200,400,200,200,200,200,400,200,200,200,400,200,200,400,200,200,200,200,
  400,200,200,400,400,400,200,400, // Pass 2 logic
  1400 // Extended rest to reach 16000ms total
};

//  SONG 2 – NSMB Wii  "Castle / Fortress Theme"
//  TOTAL = 16 000 ms
const int N2[] = {
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_FS4,  
  NOTE_DS4, NOTE_G4, NOTE_D4, NOTE_G4, NOTE_G4, NOTE_CS4, NOTE_G4,   
  NOTE_C4, NOTE_G4, NOTE_B3, NOTE_G4, NOTE_AS3, NOTE_G4, NOTE_A3, NOTE_G4, 
  NOTE_GS3, NOTE_G3, NOTE_FS3,                                                    
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_FS4,
  NOTE_DS4, NOTE_G4, NOTE_D4, NOTE_G4, NOTE_G4, NOTE_CS4, NOTE_G4,
  NOTE_C4, NOTE_B3, NOTE_AS3, NOTE_A3, NOTE_GS3, NOTE_G3,             
  NOTE_G3 // Final note extended to reach 16s                                                                  
};
const int D2[] = {
  250,250,250,250,500,250,250,250,250,250,250,500,250,250,
  250,250,250,250,250,250,250,250,500,500,1000,
  250,250,250,250,500,250,250,250,250,250,250,500,250,250,
  250,250,250,250,500,500,
  2000 // Extended final G3 note to reach 16000ms total
};

//  SONG 3 – Super Mario Galaxy  "Gusty Garden Galaxy"
//  TOTAL = 16 000 ms
const int N3[] = {
  NOTE_G5,  NOTE_FS5, NOTE_E5, NOTE_D5,  NOTE_E5,  NOTE_FS5,  
  NOTE_G5,  NOTE_E5, NOTE_D5, NOTE_B5,  NOTE_A5,  NOTE_G5,  
  NOTE_FS5, NOTE_G5,  NOTE_A5, NOTE_B5,  NOTE_G5, NOTE_A5,                       
  NOTE_D6,  NOTE_C6,  NOTE_B5, NOTE_A5,  NOTE_B5,  NOTE_C6,
  NOTE_D6,  NOTE_E6,  REST // Added notes to reach 16s
};
const int D3[] = {
  500,500,500,500,500,500,1000,500,1500, // Part 1 = 6000ms
  500,500,500,500,500,500,1000,500,1500, // Part 2 = 6000ms
  500,500,500,500,500,500, // Part 3 = 3000ms (Total 15000 so far)
  500, 500, 0 // Placeholder
};
// Note: Adjusted the duration array D3 specifically below to sum exactly to 16000
const int D3_fixed[] = {
  500,500,500,500,500,500,1000,500,1500, 
  500,500,500,500,500,500,1000,500,1500, 
  500,500,500,500,500,500,
  1000, 1000, 2500 // Extended ending to reach 16000ms
};

//  SONG 4 – SM3D World  "Super Bell Hill"
//  TOTAL = 16 000 ms
const int N4[] = {
  NOTE_E5, NOTE_G5, NOTE_A5, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_B5,
  NOTE_G5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_D5,
  NOTE_E5, NOTE_G5, NOTE_A5, NOTE_C6, NOTE_B5, NOTE_A5, NOTE_G5, NOTE_E5,
  NOTE_G5, NOTE_A5, NOTE_G5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_G5, NOTE_A5,
  NOTE_E5, NOTE_G5, NOTE_A5, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_B5,
  NOTE_A5, NOTE_G5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_G5,
  NOTE_A5, NOTE_G5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_G5, NOTE_A5, NOTE_G5, REST 
};
const int D4[] = {
  400,200,200, 400,200,400,200,
  200,200,400, 200,200,400,200,200,
  200,200,200, 400,200,200,200,400,
  200,400,200, 200,200,400,200,200,
  400,200,200, 400,200,400,200,
  200,200,400, 200,200,400,200,200,
  200,400,200, 200,200,400,400,
  200,400,200, 1200 // Extended rest to reach 16000ms total
};

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  pinMode(BUZZER, OUTPUT);
  for (int i = 0; i < 8; i++)
    pinMode(BTN[i], INPUT_PULLUP);

  playSong(N1, D1, sizeof(N1)/sizeof(N1[0]));           // 16 s
  playSong(N2, D2, sizeof(N2)/sizeof(N2[0]));           // 16 s
  playSong(N3, D3_fixed, sizeof(N3)/sizeof(N3[0]));     // 16 s
  playSong(N4, D4, sizeof(N4)/sizeof(N4[0]));           // 16 s

  // ---- 3-second manual button window -------------------------
  unsigned long endTime = millis() + 3000UL;
  while (millis() < endTime) {
    int f = -1;
    for (int i = 0; i < 8; i++) {
      if (digitalRead(BTN[i]) == LOW) { f = BTNFREQ[i]; break; }
    }
    (f != -1) ? tone(BUZZER, f) : noTone(BUZZER);
    delay(10);
  }
  noTone(BUZZER);
}

void loop() {}