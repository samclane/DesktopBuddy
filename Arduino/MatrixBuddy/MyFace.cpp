/*
 * MyFace.cpp
 *
 *  Created on: Jan 21, 2019
 *      Author: SawyerPC
 */

#include "MyFace.h"

MyFace::MyFace(const int din, const int clk, const int cs): lc(LedControl(din, clk, cs)) {
	currentEyes = OPEN;
	currentFace = HAPPY;
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

MyFace::~MyFace() {
	// TODO Auto-generated destructor stub
	this->drawAll(IMAGES[ERROR]);
}

void MyFace::drawAll(const byte image[]) {
	for (int i = 0; i < 8; i++) {
		lc.setRow(0, i, image[i]);
	}
}

void MyFace::blinkEyes(const int duration, const int depth = 0) {
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
		blinkEyes(duration, depth + 1);
	}
}

void MyFace::closeEyes() {
	lc.setRow(0, 0, 0x00);
	lc.setRow(0, 1, 0x00);
	lc.setRow(0, 2, B11000011);
	lc.setRow(0, 3, 0x00);
	currentEyes = CLOSED;
}

void MyFace::openEyes() {
	// Serial.println("open-eyes");
	lc.setRow(0, 0, 0x00);
	lc.setRow(0, 1, B11000011);
	lc.setRow(0, 2, B11000011);
	lc.setRow(0, 3, 0x00);
	currentEyes = OPEN;
}

void MyFace::xEyes() {
	// Serial.println("x-eyes");
	lc.setRow(0, 0, B10100101);
	lc.setRow(0, 1, B01000010);
	lc.setRow(0, 2, B10100101);
	currentEyes = XES;
}

void MyFace::smile() {
	// Serial.println("smile");
	lc.setRow(0, 5, B01000010);
	lc.setRow(0, 6, B00111100);
	lc.setRow(0, 7, 0x00);
}

void MyFace::neutralFace() {
	// Serial.println("neutral");
	lc.setRow(0, 5, 0x00);
	lc.setRow(0, 6, B01111110);
	lc.setRow(0, 7, 0x00);
}

void MyFace::frown() {
	// Serial.println("frown");
	lc.setRow(0, 5, 0x00);
	lc.setRow(0, 6, B00111100);
	lc.setRow(0, 7, B01000010);
}

void MyFace::curlMouth(const bool curlRight) {
	// Serial.print("curl");
	if (curlRight) {
		// Serial.println("_R");
		lc.setRow(0, 5, B00000010);
		lc.setRow(0, 6, B01111100);
	} else {  // curl left
		// Serial.println("_L");
		lc.setRow(0, 5, B01000000);
		lc.setRow(0, 6, B00111110);
	}
	lc.setRow(0, 7, 0x00);
}
