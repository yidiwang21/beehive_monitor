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

#define pResistor_UNIT_NUM 3	// FIXME: for testing

ADC *adc = new ADC();	// adc object

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
	{32, 0.0, 0.0, 0.0},
	{33, 0.0, 0.0, 0.0},
};

struct PhotoResistors prev_pResistor[pResistor_UNIT_NUM] = {
	{31, 0.0, 0.0, 0.0},
	{32, 0.0, 0.0, 0.0},
	{33, 0.0, 0.0, 0.0},
};

//***************** Teensy Pins *****************
const int ledPin = 13;	// default led pin
/* photoresistors pins already defined in pResistor.ino
const int pResistor1 = 31;
const int pResistor2 = 32;
const int pResistor3 = 33;
*/
const int audioRecorder = 16;
const int audioTrigger = 0;

const int SDCARD_CS_PIN = BUILTIN_SDCARD;	// 254?
const int SDCARD_MOSI_PIN = 11;
const int SDCARD_SCK_PIN = 13;

void setup() {
	Serial.begin(9600);	// 57600
//****************** Pin Mode *******************
	pinMode(pResistor[0].pin, INPUT);
 	pinMode(pResistor[1].pin, INPUT);
 	pinMode(pResistor[2].pin, INPUT);
//***************** ADC Setups ******************
	///// ADC0 ////
    adc->setAveraging(16); // set number of averages
    adc->setResolution(16); // set bits of resolution
    adc->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED); // change the conversion speed
    adc->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED); // change the sampling speed
    ////// ADC1 /////
    #if ADC_NUM_ADCS > 1
    adc->setAveraging(16, ADC_1); // set number of averages
    adc->setResolution(16, ADC_1); // set bits of resolution
    adc->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED, ADC_1); // change the conversion speed
    adc->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED, ADC_1); // change the sampling speed
    #endif
//**************** SD Card Inits *****************
	SPI.setMOSI(SDCARD_MOSI_PIN);
	SPI.setSCK(SDCARD_SCK_PIN);
	if (!(SD.begin(SDCARD_CS_PIN))) {	// stop here if no SD card, but print a message
    	while (1) {
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
	// pResistorMonitor();
	startRecording();
	if(millis() > 20000 && recordingMode == 1) {
		stopRecording();
		delay(2000);
	}
}
