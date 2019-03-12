/*
 * MyNeck.h
 *
 *  Created on: Mar 4, 2019
 *      Author: SawyerPC
 */

#ifndef MYNECK_H_
#define MYNECK_H_

#include "Arduino.h"
#include <Servo.h>

/* Animation frequency */
const unsigned long TOP_PERIOD_MS = 1499L;
const unsigned long BOTTOM_PERIOD_MS = 1201L;
const unsigned long ANIMATION_FUZZ_NECK = 1e2L; // MS to fudge the animation to make it more realistic
const unsigned long TOP_RANGE = 1L;
const unsigned long BOTTOM_RANGE = 1L;

class MyNeck {
private:

public:
	Servo sTop; //tilt
	Servo sBot; //pan
	MyNeck(const int sTopPort, const int sBotPort);
	int sTopPos;
	int sBotPos;
	void resetPose();
	void animateNeck();
};

#endif /* MYNECK_H_ */
