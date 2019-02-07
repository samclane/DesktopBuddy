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


	// TODO Causes display lag when enabled
	attachInterrupt(digitalPinToInterrupt(INTR_SENSE), senseISR, RISING);


	randomSeed(analogRead(0));

	face.drawAll(IMAGES[HAPPY]);
}

void readInterruptSensor() {
	static unsigned long lastDebounceTime = 0;
	static byte lastByteSent = 0x00;
	const static unsigned long debounceDelay = 250;
	if (millis() - lastDebounceTime > debounceDelay) {
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
		lastDebounceTime = millis();
	}
}

void senseISR() {
	SENSE_ISR_FLAG = 0x01;
}

void loop() {
	if (SENSE_ISR_FLAG > 0x00) {
		readInterruptSensor();
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
		//case ConnectionStatusLength:
		//	face.transform(IMAGES[random(0, IMAGES_LEN)], MASKS[BOTH_EYES]);
		//	face.transform(IMAGES[random(0, IMAGES_LEN)], MASKS[MOUTH]);
		//	break;
		default:
			Serial.print("Unused symbol: ");
			Serial.println(incomingByte, DEC);
	}
	voiceConnected = (ConnectionStatus) incomingByte;
}
