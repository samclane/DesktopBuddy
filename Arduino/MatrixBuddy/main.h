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
volatile ConnectionStatus voiceConnected;
MyFace face = MyFace(DIN, CLK, CS); //TODO  I don't want to initialize this here but it's making me

// Function Definitions

void avoidISR();
void readAvoidSensor();
void animateFace();

#endif
