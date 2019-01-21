#ifndef MAIN_H_
#define MAIN_H_

#include "Arduino.h"

#include "MyFace.h"

enum ConnectionStatus {
	DISCONNECTED, CONNECTED, MUTED, DEAFENED, ConnectionStatusLength
};

/* Pinouts */
int DIN = 3;
int CLK = 6;
int CS = 5;
int SENSE = 2;

/* Animation frequency */
const long BLINK_RATE = 83657L;  // It's an ugly way to do timing, but it works
const long MOUTH_RATE = 43657L;
//const long BLINK_RATE = 13657L;  // Faster animations for demo
//const long MOUTH_RATE = 3657L;

/* Animation Duration */
const int BLINK_LENGTH = 150;

// Function Definitions

void avoidISR();
void readAvoidSensor();
void animateFace();

#endif
