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
int DIN = 3;
int CLK = 6;
int CS = 5;
int INTR_SENSE = 2;

// Variables
volatile byte SENSE_ISR_FLAG = 0;
volatile ConnectionStatus voiceConnected;
MyFace face;

// Function Definitions

void senseISR();
void processInterrupt();
void animateFace();

#endif
