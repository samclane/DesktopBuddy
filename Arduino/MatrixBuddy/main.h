/*
 * main.h
 *
 *  Created on: Jan 19, 2019
 *      Author: SawyerPC
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "Arduino.h"
#include "MyFace.h"

enum ConnectionStatus {
	DISCONNECTED, CONNECTED, MUTED, DEAFENED, ConnectionStatusLength
};

/* Pinouts */
int DIN = 4;
int CLK = 6;
int CS = 5;
int INTR_SENSE = 3;

// Variables
volatile ConnectionStatus voiceConnected;
MyFace face;

// Function Definitions

void senseISR();
void processInterrupt();
void animateFace();

#endif
