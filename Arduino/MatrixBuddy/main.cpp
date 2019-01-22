/*
 * main.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: SawyerPC
 */


#include "main.h"

void setup() {
	voiceConnected = DISCONNECTED;

	Serial.begin(9600);
	pinMode(AVOID_SENSE, INPUT);

	randomSeed(analogRead(0));

	attachInterrupt(digitalPinToInterrupt(AVOID_SENSE), readAvoidSensor, CHANGE);
}

void readAvoidSensor() {
	static unsigned long lastDebounceTime = 0;
	static byte lastByteSent = 0x00;
	const static unsigned long debounceDelay = 100;
	if (millis() - lastDebounceTime > debounceDelay) {
		if (voiceConnected == DISCONNECTED) {
			if (face.currentEyes == OPEN) {
				face.xEyes();
			} else {
				face.openEyes();
			}
		} else {
			// We're connected to Discord; Send serial signal to PC for mute
			lastByteSent = ~lastByteSent;
			Serial.write(lastByteSent);
		}
		lastDebounceTime = millis();
	}
}

void avoidISR() {
	readAvoidSensor();
}

void animateFace() {
	static unsigned long lastBlink = 0L;
	static unsigned long lastMouth = 0L;
	if (millis() - lastBlink >= BLINK_PERIOD_MS) {
		// Add a random number so blinks aren't regular
		lastBlink = millis() - random(ANIMATION_FUZZ);
		face.blinkEyes(BLINK_LENGTH);
	}
	if (millis() - lastMouth >= MOUTH_PERIOD_MS) {
		lastMouth = millis() - random(ANIMATION_FUZZ);
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
	if (isDigit) {
		incomingByte = ch - '0';
	}
	// Draw the corresponding image (debug; will probably remove)
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
		default:
			face.drawAll(IMAGES[ERROR]);
		}
		voiceConnected = (ConnectionStatus) incomingByte;
	}
	// Unsupported protocol code; display error
	else {
		face.drawAll(IMAGES[ERROR]);
	}
}
