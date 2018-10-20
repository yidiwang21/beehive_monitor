/*
 *
 * beehive_monitor.ino
 * Description: this page includes pins, setup, and the main loop
 * Created on Apr 27, 2018
 *
 */
#include <Arduino.h>
#include "TeensyThreads.h"
#include <math.h>
#include <ADC.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <SoftwareSerial.h>

//***************** Variables *******************
int recordingMode = 0;  // status: 0 = stop, 1 = recording
int flag = 0;			// 1: finished pResistors; 2: finished recording audio; 3: finished sending audio file

//***************** Teensy Pins *****************
const int ledPin = 13;	// default led pin

const int audioRecorder = 16;
const int audioTrigger = 0;

const int SDCARD_CS_PIN = BUILTIN_SDCARD;	// 254?
const int SDCARD_MOSI_PIN = 11;
const int SDCARD_SCK_PIN = 13;

// tried other rx tx pins, didn't work
const int BT_RX = 9;
const int BT_TX = 10;

//***************** objects *****************
ADC *adc = new ADC();	// adc object
SoftwareSerial Bluetooth(BT_RX, BT_TX);	// RX, TX

void setup() {
	Serial.begin(9600);	// 57600
	Bluetooth.begin(9600);
	// Bluetooth.println("Hello Viewer!");
//****************** Pin Mode *******************
	
//***************** ADC Setups ******************
    // ADC1 //
	// use adc1 to resolve adc0 audio input conflict
    // #if ADC_NUM_ADCS > 1
    adc->setAveraging(16, ADC_1); // set number of averages
    adc->setResolution(16, ADC_1); // set bits of resolution
    adc->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED, ADC_1); // change the conversion speed
    adc->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED, ADC_1); // change the sampling speed
    // #endif
//**************** SD Card Inits *****************
	SPI.setMOSI(SDCARD_MOSI_PIN);
	SPI.setSCK(SDCARD_SCK_PIN);
	if (!(SD.begin(SDCARD_CS_PIN))) {	// stop here if no SD card, but print a message
    	while (true) {
      		Serial.println("Unable to access the SD card!");
			delay(200);
    	}
  	}
	else
  		Serial.println("Successfully access the SD card!");
//*************** Audio Recording ****************
	delay(100);
	Serial.println("Teensy Audio AudioMemory()");
    AudioMemory(60);	// FIXME
	// getBaseline();
}

void loop() {
	audioRecording();
	delay(45000);	// TODO: go to sleep mode
}
