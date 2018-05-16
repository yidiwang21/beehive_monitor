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

#define pResistor_UNIT_NUM 1	// FIXME: for testing

//***************** Variables *******************
int recordingMode = 0;  // status: 0 = stop, 1 = recording

//****************** structs ********************
struct PhotoResistors {
	const int pin;	// photoresistors pin
	double vol;
	double res;
	double res_log;
};

struct PhotoResistors pResistor[pResistor_UNIT_NUM] = {
	{31, 0.0, 0.0, 0.0},
	// {32, 0.0, 0.0, 0.0},
	// {33, 0.0, 0.0, 0.0},
};

struct PhotoResistors prev_pResistor[pResistor_UNIT_NUM] = {
	{31, 0.0, 0.0, 0.0},
	// {32, 0.0, 0.0, 0.0},
	// {33, 0.0, 0.0, 0.0},
};

//***************** Teensy Pins *****************
const int ledPin = 13;	// default led pin

const int audioRecorder = 16;
const int audioTrigger = 0;

const int SDCARD_CS_PIN = BUILTIN_SDCARD;	// 254?
const int SDCARD_MOSI_PIN = 11;
const int SDCARD_SCK_PIN = 13;

const int BT_RX = 0;
const int BT_TX = 1;

//***************** objects *****************
ADC *adc = new ADC();	// adc object
SoftwareSerial Bluetooth(BT_RX, BT_TX);	// RX, TX

void setup() {
	Serial.begin(9600);	// 57600
	Bluetooth.begin(115200);
//****************** Pin Mode *******************
	pinMode(pResistor[0].pin, INPUT);
 	// pinMode(pResistor[1].pin, INPUT);
 	// pinMode(pResistor[2].pin, INPUT);
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
			delay(100);
    	}
  	}
	else
  		Serial.println("Successfully access the SD card!");
//*************** Audio Recording ****************
	delay(1000);
	Serial.println("Teensy Audio AudioMemory()");
    AudioMemory(60);	// FIXME
}

void loop() {
	audioRecording();
	// pResistorMonitor();
}
