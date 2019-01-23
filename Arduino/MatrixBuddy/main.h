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
int AVOID_SENSE = 2;

// Variables
volatile byte AVOID_ISR_FLAG;
volatile ConnectionStatus voiceConnected;
MyFace face;

// Function Definitions

void avoidISR();
void readAvoidSensor();
void animateFace();

#endif
