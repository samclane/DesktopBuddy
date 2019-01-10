#include "LedControl.h"
#include "MatrixBuddy.h"

LedControl lc = LedControl(DIN, CLK, CS, 1);

byte incomingByte = 0;
EMOTES currentFace = HAPPY;
EYES currentEyes = OPEN;
long blinkCount = 0L;
long mouthCount = 0L;
ConnectionStatus voiceConnected = DISCONNECTED;

void setup() {
  Serial.begin(9600);
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,1);
  /* and clear the display */
  lc.clearDisplay(0);

  // Init main face
  drawAll(IMAGES[HAPPY]);
  currentFace = HAPPY;
  currentEyes = OPEN;

  randomSeed(analogRead(0));
}

void drawAll(const byte image[]) {
  for(int i=0;i<8;i++) {
    lc.setRow(0, i, image[i]);
  }
}

void blink(const int duration, const int depth=0) {
  Serial.println("blink");
  // Hardcoded blinking eyes "sprite"
  lc.setRow(0, 0, 0x00);
  lc.setRow(0, 1, 0x00);
  lc.setRow(0, 2, B11000011);
  lc.setRow(0, 3, 0x00);
  delay(duration);
  switch (currentEyes) {
    case OPEN:
      openEyes();
      break;
    case XES:
      xEyes();
      break;
    default:
      break;
  }
  if (random(0, 3 + depth) == 2) // Chance to flutter eyes
  {
    for(int i = 0; i <= depth;i++) // indicate blink depth
      Serial.print("+");
    delay(duration/10);
    blink(duration, depth+1);
  }
}

void openEyes() {
  Serial.println("open-eyes");
  lc.setRow(0, 0, 0x00);
  lc.setRow(0, 1, B11000011);
  lc.setRow(0, 2, B11000011);
  lc.setRow(0, 3, 0x00);
}

void xEyes() {
  Serial.println("x-eyes");
  lc.setRow(0, 0, B10100101);
  lc.setRow(0, 1, B01000010);
  lc.setRow(0, 2, B10100101);
  currentEyes = XES; 
}

void smile() {
  Serial.println("smile");
  lc.setRow(0, 5, B01000010);
  lc.setRow(0, 6, B00111100);
  lc.setRow(0, 7, 0x00);
}

void neutralFace() {
  Serial.println("neutral");
  lc.setRow(0, 5, 0x00);
  lc.setRow(0, 6, B01111110);
  lc.setRow(0, 7, 0x00);
}

void frown() {
  Serial.println("frown");
  lc.setRow(0, 5, 0x00);
  lc.setRow(0, 6, B00111100);
  lc.setRow(0, 7, B01000010);
}

void loop() {
  if (Serial.available()) {
    // Convert to int quickly
    char ch = Serial.read();
    Serial.println(ch);
    if (isDigit) {
      incomingByte = ch - '0';
    }
    // Draw the corresponding image
    if (incomingByte < ConnectionStatusLength) {
      switch (incomingByte) {
        case DISCONNECTED: // Reset Face
          drawAll(IMAGES[HAPPY]);
          currentFace = HAPPY;
          break;
        case CONNECTED:
          drawAll(IMAGES[PLAY]);
          break;
        case MUTED:
          drawAll(IMAGES[PAUSE]);
          break;
        case DEAFENED:
          drawAll(IMAGES[STOP]);
          break;
      }
      voiceConnected = (ConnectionStatus) incomingByte;
    }
    // Unsupported protocol code; display error
    else {
      drawAll(IMAGES[ERROR]);
    }
  }
  if (voiceConnected == DISCONNECTED) {
    blinkCount += random(1L,3L);  // Add a random number so blinks aren't regular
    if (blinkCount >= BLINK_RATE)
    {
      blinkCount = 0L;
      blink(BLINK_LENGTH);
    }
    mouthCount += random(1L,2L);
    if (mouthCount >= MOUTH_RATE) {
      mouthCount = 0L;
      if (random(0, 2)) {
        smile();
      }
      else {
        neutralFace();
      }
    }
  }
}

