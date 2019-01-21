#include "main.h"

volatile ConnectionStatus voiceConnected = DISCONNECTED;

MyFace face = MyFace(DIN, CLK, CS);

void setup() {
	Serial.begin(9600);
	pinMode(SENSE, INPUT);

	randomSeed(analogRead(0));

	attachInterrupt(digitalPinToInterrupt(SENSE), readAvoidSensor, FALLING);
}


void readAvoidSensor() {
	static unsigned long lastDebounceTime = 0;
	const unsigned long debounceDelay = 750;
	if (millis() - lastDebounceTime > debounceDelay) {
		if (voiceConnected == DISCONNECTED) {
			if (face.currentEyes == OPEN) {
				face.xEyes();
			} else {
				face.openEyes();
			}
		} else {
			Serial.write(0);
		}
		lastDebounceTime = millis();
	}
}

void avoidISR() {
	readAvoidSensor();
}

void animateFace() {
	static unsigned long blinkCount = 0L;
	static unsigned long mouthCount = 0L;
	blinkCount += random(1L, 3L); // Add a random number so blinks aren't regular
	if (blinkCount >= BLINK_RATE) {
		blinkCount = 0L;
		face.blinkEyes(BLINK_LENGTH);
	}
	mouthCount += random(1L, 2L);
	if (mouthCount >= MOUTH_RATE) {
		mouthCount = 0L;
		if (random(0, 2)) {
			face.smile();
		} else {
			face.curlMouth(random(0, 2));
		}
	}
}

void loop() {
	if (voiceConnected == DISCONNECTED) {
		animateFace();
	}
}

void serialEvent() {
	byte incomingByte = 0;
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
			face.drawAll(IMAGES[HAPPY]);
			face.currentFace = HAPPY;
			break;
		case CONNECTED:
			face.drawAll(IMAGES[PLAY]);
			break;
		case MUTED:
			face.drawAll(IMAGES[PAUSE]);
			break;
		case DEAFENED:
			face.drawAll(IMAGES[STOP]);
			break;
		}
		voiceConnected = (ConnectionStatus) incomingByte;
	}
	// Unsupported protocol code; display error
	else {
		face.drawAll(IMAGES[ERROR]);
	}
}
