#include "LedControl.h"
#include "MatrixBuddy.h"
#include <Arduino_FreeRTOS.h>


LedControl lc = LedControl(DIN, CLK, CS, 1);
byte incomingByte = 0;
EMOTES currentFace = HAPPY;
EYES currentEyes = OPEN;
long lastBlinkTime = 0L;
long lastMountTime = 0L;
ConnectionStatus voiceConnected;  // TODO: THIS NEEDS TO BE ACCOMPLISHED WITH TASK-COMMUNICATION
int prevAvoidReading = 0;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay =  75;

void vTaskDistanceSense(void *pvParameters);
void vTaskReadSerial(void *pvParameters);
void vTaskIdle(void *pvParameters);

void setup() {
  voiceConnected = DISCONNECTED;
  Serial.begin(9600);
  pinMode(SENSE, INPUT);
  /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
  */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 1);
  /* and clear the display */
  lc.clearDisplay(0);

  // Init main face
  drawAll(IMAGES[HAPPY]);
  currentFace = HAPPY;
  currentEyes = OPEN;

  randomSeed(analogRead(0));

  xTaskCreate(
    vTaskDistanceSense
    , (const portCHAR *)"Distance"
    , 128
    , NULL
    , 0
    , NULL);

  xTaskCreate( // todo: this task needs to be higher priority/triggered by Serial.available()
    vTaskReadSerial
    , (const portCHAR *)"SerialRead"
    , 128
    , NULL
    , 0
    , NULL);

  xTaskCreate(
    vTaskIdle
    , (const portCHAR *)"Idle"
    , 128
    , NULL
    , 0
    , NULL);
}

void drawAll(const byte image[]) {
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, image[i]);
  }
}

void blink(const int duration, const int depth = 0) {
  // Serial.println("blink");
  // Hardcoded blinking eyes "sprite"
  EYES tempEyes = currentEyes;
  closeEyes();
  delay(duration);
  switch (tempEyes) {
    case OPEN:
      openEyes();
      break;
    case XES:
      xEyes();
      break;
  }
  if (random(0, 3 + depth) == 2) // Chance to flutter eyes. Chance decr. as depth incr.
  {
    delay(duration / 10);
    blink(duration, depth + 1);
  }
}

void closeEyes() {
  lc.setRow(0, 0, 0x00);
  lc.setRow(0, 1, 0x00);
  lc.setRow(0, 2, B11000011);
  lc.setRow(0, 3, 0x00);
  currentEyes = CLOSED;
}

void openEyes() {
  // Serial.println("open-eyes");
  lc.setRow(0, 0, 0x00);
  lc.setRow(0, 1, B11000011);
  lc.setRow(0, 2, B11000011);
  lc.setRow(0, 3, 0x00);
  currentEyes = OPEN;
}

void xEyes() {
  // Serial.println("x-eyes");
  lc.setRow(0, 0, B10100101);
  lc.setRow(0, 1, B01000010);
  lc.setRow(0, 2, B10100101);
  currentEyes = XES;
}

void smile() {
  // Serial.println("smile");
  lc.setRow(0, 5, B01000010);
  lc.setRow(0, 6, B00111100);
  lc.setRow(0, 7, 0x00);
}

void neutralFace() {
  // Serial.println("neutral");
  lc.setRow(0, 5, 0x00);
  lc.setRow(0, 6, B01111110);
  lc.setRow(0, 7, 0x00);
}

void frown() {
  // Serial.println("frown");
  lc.setRow(0, 5, 0x00);
  lc.setRow(0, 6, B00111100);
  lc.setRow(0, 7, B01000010);
}

void curlMouth(const bool curlRight) {
    // Serial.print("curl");
    if (curlRight) {
        // Serial.println("_R");
        lc.setRow(0, 5, B00000010);
        lc.setRow(0, 6, B01111100);
    }
    else {  // curl left
        // Serial.println("_L");
        lc.setRow(0, 5, B01000000);
        lc.setRow(0, 6, B00111110);
    }
    lc.setRow(0, 7, 0x00);
}

void vTaskDistanceSense(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    int avoidReading = digitalRead(SENSE);
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (avoidReading != prevAvoidReading) {
        if (voiceConnected == DISCONNECTED) {
          if (!avoidReading) {
            xEyes();
          }
          else {
            openEyes();
          }
        }
        else {
          Serial.write(avoidReading);
        }
        prevAvoidReading = avoidReading;
        lastDebounceTime = millis();
      }
    }
  }
  vTaskDelete(NULL);
}

void vTaskReadSerial(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (Serial.available()) {
      // Convert to int quickly
      char ch = Serial.read();
      // Serial.println(ch);
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
  }
  vTaskDelete(NULL);
}

void vTaskIdle(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (voiceConnected == DISCONNECTED) {
      if (millis() - lastBlinkTime >= BLINK_RATE)
      {
        lastBlinkTime = millis() + random(-100L, 100L);
        blink(BLINK_LENGTH);
      }
      if (millis() - lastMountTime >= MOUTH_RATE) {
        lastMountTime = millis() + random(-100L, 100L);
        if (random(0, 2)) {
          smile();
        }
        else {
          curlMouth(random(0, 2));
        }
      }
    }
  }
}

void loop() {

}

