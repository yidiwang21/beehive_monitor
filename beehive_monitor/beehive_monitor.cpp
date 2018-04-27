/*
 *
 * beehive_monitor.cpp
 * Description: this page includes pins, setup, and the main loop
 * Created on Apr 26, 2018
 *
 */
#include <Arduino.h>
#include "TeensyThreads.h"

//***************** Teensy Pins *****************
// default led pin
const int ledPin = 13;
// photoresistors pin
const int pResistor1 = 31;
const int pResistor2 = 32;

void setup() {
	pinMode(pResistor1, INPUT);
	pinMode(pResistor2, INPUT);
	attachInterrupt(pResistor1, monitorResistence, LOW);
	attachInterrupt(pResistor2, monitorResistence, LOW);
	Serial.begin(9600);
}

void loop() {

}
