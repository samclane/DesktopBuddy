#include "MyNeck.h"

MyNeck::MyNeck(const int sTopPort, const int sBotPort) {
	sTopPos = 0;
	sBotPos = 0;
	sTop.attach(sTopPort);
	sBot.attach(sBotPort);
}

void MyNeck::resetPose() {
	sTop.write(45);
	sTopPos = 45;
	sBot.write(150);
	sBotPos = 150;
}

void MyNeck::testPose() {
	sTop.write(90);
	sTopPos = 90;
	sBot.write(0);
	sBotPos = 0;
}

void MyNeck::animateNeck() {
	static unsigned long lastTop = 0L;
	static unsigned long lastBottom = 0L;
	if (millis() - lastTop >= TOP_PERIOD_MS) {
		// Add a random number so moves aren't regular
		lastTop = millis() - random(ANIMATION_FUZZ_NECK);
		sTopPos += (int)random(-TOP_RANGE, TOP_RANGE);
		sTopPos = constrain(sTopPos, 1, 199);
		sTop.write((int)sTopPos);
		Serial.print("T");
		Serial.println((int)sTopPos);
	}
	if (millis() - lastBottom >= BOTTOM_PERIOD_MS) {
		lastBottom = millis() - random(ANIMATION_FUZZ_NECK);
		sBotPos += (int)random(-BOTTOM_RANGE, BOTTOM_RANGE);
		sBotPos = constrain(sBotPos, 1, 199);
		sBot.write((int)sBotPos);
		Serial.print("B");
		Serial.println((int)sBotPos);
	}
}
