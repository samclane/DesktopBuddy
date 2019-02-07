/*
 * MyFace.cpp
 *
 *  Created on: Jan 21, 2019
 *      Author: SawyerPC
 */

#include "MyFace.h"

MyFace::MyFace(const int din, const int clk, const int cs) :
		lc(LedControl(din, clk, cs)) {
	currentEyes = OPEN;
	currentFace = HAPPY;
	memcpy(currentMatrix, IMAGES[HAPPY], sizeof(byte)*8);
	/*
	 The MAX72XX is in power-saving mode on startup,
	 we have to do a wakeup call
	 */
	lc.shutdown(0, false);
	/* Set the brightness to a medium values */
	lc.setIntensity(0, 1);
	/* and clear the display */
	lc.clearDisplay(0);

	drawAll(IMAGES[HAPPY]);
}

MyFace::MyFace() :
		lc(LedControl(NULL, NULL, NULL)) {
	// Dummy default constructor
}

MyFace::~MyFace() {
	drawAll(IMAGES[ERROR]);
}

void MyFace::drawAll(const byte image[]) {
	// Write a 8x8 boolean 2D-Array to the LedMatrix
	for (int i = 0; i < 8; i++) {
		lc.setRow(0, i, image[i]);
	}
	memcpy(this->currentMatrix, image, sizeof(byte)*8);
}

void MyFace::drawEyes(EYES index) {
	switch (index) {
	case OPEN:
		openEyes();
		break;
	case CLOSED:
		closeEyes();
		break;
	case XES:
		xEyes();
		break;
	}
}

void MyFace::blinkEyes(const int duration, const int depth = 0) {
	// Serial.println("blink");
	// Hard-coded blinking eyes "sprite"
	EYES tempEyes = currentEyes;
	closeEyes();
	delay(duration);
	drawEyes(tempEyes);
	// Chance to flutter eyes. Chance decreases. as recursive depth increases
	if (random(0, 3 + depth) == 2) {
		delay(duration / 10);
		blinkEyes(duration, depth + 1);
	}
}

void MyFace::closeEyes() {
	const byte closedEyes[8] = {
		0, 0,
		B11000011
	};
	this->transform(closedEyes, MASKS[BOTH_EYES]);
	currentEyes = CLOSED;
}

void MyFace::openEyes() {
	// Serial.println("open-eyes");b
	const byte openEyes[8] = {
			0,
			B11000011,
			B11000011
	};
	this->transform(openEyes, MASKS[BOTH_EYES]);
	currentEyes = OPEN;
}

void MyFace::xEyes() {
	// Serial.println("x-eyes");
	const byte xEyes[8] = {
			B10100101,
			B01000010,
			B10100101
	};
	this->transform(xEyes, MASKS[BOTH_EYES]);
	currentEyes = XES;
}

void MyFace::smile() {
	// Serial.println("smile");
	const byte smile[8] = {
			0,0,0,0,0,
			B01000010,
			B00111100
	};
	this->transform(smile, MASKS[MOUTH]);
}

void MyFace::neutralFace() {
	// Serial.println("neutral");
	const byte neutral[8] = {
			0,0,0,0,0,0,
			B00111100
	};
	this->transform(neutral, MASKS[MOUTH]);
}

void MyFace::frown() {
	// Serial.println("frown");
	const byte frown[8] = {
		0,0,0,0,0,0,
		B00111100,
		B01000010
	};
	this->transform(frown, MASKS[MOUTH]);
}

void MyFace::curlMouth(const bool curlRight) {
	// Serial.print("curl");
	const byte curlR[8] = {
			0,0,0,0,0,
			B00000010,
			B01111100
	};
	const byte curlL[8] = {
			0,0,0,0,0,
			B01000000,
			B00111100
	};
	if (curlRight) {
		// Serial.println("_R");
		this->transform(curlR, MASKS[MOUTH]);

	} else {  // curl left
		// Serial.println("_L");
		this->transform(curlL, MASKS[MOUTH]);
	}
}

void MyFace::animateFace() {
	static unsigned long lastBlink = 0L;
	static unsigned long lastMouth = 0L;
	if (millis() - lastBlink >= BLINK_PERIOD_MS) {
		// Add a random number so blinks aren't regular
		lastBlink = millis() - random(ANIMATION_FUZZ);
		blinkEyes(BLINK_LENGTH);
	}
	if (millis() - lastMouth >= MOUTH_PERIOD_MS) {
		lastMouth = millis() - random(ANIMATION_FUZZ);
		if (random(0, 2)) {
			smile();
		} else {
			curlMouth(random(0, 2));
		}
	}
}


void MyFace::transform(const byte desired[8], const byte mask[8]) {
	int i, j;
	byte output[8];
	memcpy(output, this->currentMatrix, sizeof(byte)*8);
	for (i=0; i<8; i++) {
		for (j=7; j>=0; j--) {
			if (bitRead(mask[i], j)) {
				output[i] ^= (-bitRead(desired[i], j) ^ output[i]) & (1UL << j);
			} // else keep the current value
			Serial.print(bitRead(output[i], j) ? "X" : "-");
		}
		Serial.println();
	}
	drawAll(output);
	Serial.println("________");
}
