/*
 * main.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: SawyerPC
 */

#include "main.h"


void setup() {
	AVOID_ISR_FLAG = LOW;
	voiceConnected = DISCONNECTED;
	face = MyFace(DIN, CLK, CS);

	Serial.begin(9600);
	pinMode(AVOID_SENSE, INPUT);

	randomSeed(analogRead(0));

	attachInterrupt(digitalPinToInterrupt(AVOID_SENSE), avoidISR,
	CHANGE);

	face.drawAll(IMAGES[HAPPY]);
}

void readAvoidSensor() {
	static unsigned long lastDebounceTime = 0;
	static byte lastByteSent = 0x00;
	const static unsigned long debounceDelay = 150;
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
	AVOID_ISR_FLAG = HIGH;
	readAvoidSensor();
	AVOID_ISR_FLAG = LOW;
}

void loop() {
	if (voiceConnected == DISCONNECTED) {
		if (digitalRead(AVOID_SENSE) && face.currentEyes == XES && AVOID_ISR_FLAG == LOW) {
			Serial.println("Fixing eyes...");
			face.openEyes();
		}
		face.animateFace();
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
		}
		voiceConnected = (ConnectionStatus) incomingByte;
	}
	// Unsupported protocol code; display error
	else {
		Serial.print("Unused symbol: ");
		Serial.println(incomingByte, DEC);
	}
}
