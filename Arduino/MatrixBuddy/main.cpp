/*
 * main.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: SawyerPC
 */

#include "main.h"

void setup() {
	voiceConnected = DISCONNECTED;
	face = MyFace(DIN, CLK, CS);

	Serial.begin(9600);
	pinMode(INTR_SENSE, INPUT);

	attachInterrupt(digitalPinToInterrupt(INTR_SENSE), senseISR, RISING);

	randomSeed(analogRead(0));

	face.drawAll(IMAGES[HAPPY]);
}

void processInterrupt() {
	static byte lastByteSent = 0x00;
	if (voiceConnected == DISCONNECTED) {
		if (face.currentEyes == OPEN) {
			face.xEyes();
		}
		else if (face.currentEyes == XES) {
			face.openEyes();
		}
	} else {
		// We're connected to Discord; Send serial signal to PC for mute
		lastByteSent = ~lastByteSent;
		Serial.write(lastByteSent);
	}
}

void senseISR() {
	static size_t lastDebounceTime = 0;
	const size_t debounceDelay = 200;
	if (millis() - lastDebounceTime > debounceDelay) {
		SENSE_ISR_FLAG = 0x01;
		lastDebounceTime = millis();
	}
}

void loop() {
	if (SENSE_ISR_FLAG > 0x00) {
		processInterrupt();
		SENSE_ISR_FLAG = 0x00;
	}
	else if (voiceConnected == DISCONNECTED) {
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
	switch (incomingByte) {
		case DISCONNECTED: // Reset Face
			face.drawAll(IMAGES[HAPPY]);
			face.currentFace = HAPPY;
			break;
		case CONNECTED:
			face.drawAll(IMAGES[PLAY]);
			face.currentFace = PLAY;
			break;
		case MUTED:
			face.drawAll(IMAGES[PAUSE]);
			face.currentFace = PAUSE;
			break;
		case DEAFENED:
			face.drawAll(IMAGES[STOP]);
			face.currentFace = STOP;
			break;
		default:
			Serial.print("Unused symbol: ");
			Serial.println(incomingByte, DEC);
	}
	voiceConnected = (ConnectionStatus) incomingByte;
}
