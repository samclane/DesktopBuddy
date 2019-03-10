/*
 * main.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: SawyerPC
 */

#include "main.h"

void setup() {
	Serial.begin(9600);

	pinMode(DIN, OUTPUT);
	pinMode(CLK, OUTPUT);
	pinMode(CS, OUTPUT);
	pinMode(SPEAKER, OUTPUT);
	pinMode(INTR_SENSE_1, INPUT_PULLUP);

	voiceConnected = DISCONNECTED;

	face = MyFace(DIN, CLK, CS);
	neck = &MyNeck(SERVO_TOP, SERVO_BOTTOM);

	attachInterrupt(digitalPinToInterrupt(INTR_SENSE_1), senseISR1, FALLING);

	randomSeed(analogRead(0));

	face.drawAll(IMAGES[HAPPY]);
	neck->resetPose();
}

void processInterrupt() {
	if (voiceConnected != DISCONNECTED) {
		/* 0: Toggle Mute */
		Serial.write(0x00);
		switch (voiceConnected) {
		case CONNECTED:
			voice.say(spGOODBYE);
			break;
		case MUTED:
		case DEAFENED:
			voice.say(spHELLO);
		}
	} else {
		if (face.currentEyes == OPEN) {
			face.xEyes();
		} else if (face.currentEyes == XES) {
			face.openEyes();
		}
	}

}

void senseISR1() {
	static unsigned long lastDebounceTime = 0;
	const unsigned long debounceDelay = 1000;
	if (millis() - lastDebounceTime > debounceDelay) {
		lastDebounceTime = millis();
		processInterrupt();
	}
}

void loop() {
	if (voiceConnected == DISCONNECTED) {
		neck->animateNeck();
		face.animateFace();  // Something about this function makes neck fail
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
		neck->resetPose();
		break;
	case MUTED:
		face.drawAll(IMAGES[PAUSE]);
		face.currentFace = PAUSE;
		neck->resetPose();
		break;
	case DEAFENED:
		face.drawAll(IMAGES[STOP]);
		face.currentFace = STOP;
		neck->resetPose();
		break;
	default:
		break;
	}
	voiceConnected = (ConnectionStatus) incomingByte;
}
