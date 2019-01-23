/*
 * MyFace.h
 *
 *  Created on: Jan 21, 2019
 *      Author: SawyerPC
 */

#ifndef MYFACE_H_
#define MYFACE_H_
#include "Arduino.h"
#include "LedControl.h"

const byte IMAGES[][8] = {
  { // HAPPY
    B00000000,
    B11000011,
    B11000011,
    B00000000,
    B00000000,
    B01000010,
    B00111100,
    B00000000
  }, { // ERROR
    B10000001,
    B01000010,
    B00100100,
    B00011000,
    B00011000,
    B00100100,
    B01000010,
    B10000001
  }, { // PLAY
    B00000000,
    B01100000,
    B01111000,
    B01111110,
    B01111111,
    B01111110,
    B01111000,
    B01100000
  }, { // STOP
    B00000000,
    B00000000,
    B00111100,
    B00111100,
    B00111100,
    B00111100,
    B00000000,
    B00000000
  }, { // PAUSE
    B00000000,
    B00000000,
    B01100110,
    B01100110,
    B01100110,
    B01100110,
    B00000000,
    B00000000
  }
};
const int IMAGES_LEN = sizeof(IMAGES) / 8;

enum EMOTES {
	HAPPY, ERROR, PLAY, STOP, PAUSE
};

enum EYES {
	OPEN, CLOSED, XES
};

/* Animation frequency */
const unsigned long BLINK_PERIOD_MS = 4000L;
const unsigned long MOUTH_PERIOD_MS = 10000L;
const unsigned long ANIMATION_FUZZ = 500L; // MS to fudge the animation to make it more realistic
const unsigned long BLINK_LENGTH = 150L; // blink animation average duration

class MyFace {
private:
	LedControl lc;
public:
	volatile EYES currentEyes;
	volatile EMOTES currentFace;
	MyFace(const int din, const int clk, const int cs);
	MyFace();
	virtual ~MyFace();
	void drawAll(const byte image[]);
	void drawEyes(EYES index);
	void blinkEyes(const int duration, const int depth = 0);
	void closeEyes();
	void openEyes();
	void xEyes();
	void smile();
	void neutralFace();
	void frown();
	void curlMouth(const bool curlRight);
	void animateFace();
};

#endif /* MYFACE_H_ */
