/*
 *
 * beehive_monitor.cpp
 * Description: this page includes pins, setup, and the main loop
 * Created on Apr 26, 2018
 *
 */
#include <Arduino.h>
#include "TeensyThreads.h"

#include <vector>

//***************** Teensy Pins *****************
// default led pin
const int ledPin = 13;
// photoresistors pin
const int pResistor1 = 31;
const int pResistor2 = 32;

void setup() {
	pinMode(pResistor1, INPUT);
	pinMode(pResistor2, INPUT);
	attachInterrupt(pResistor1, isBlocked, FALLING);
	attachInterrupt(pResistor1, isNotBlocked, RISING);
	Serial.begin(9600);	// 57600
}



void loop() {

}
