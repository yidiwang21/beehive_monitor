/*
 *
 * beehive_monitor.ino
 * Description: this page includes pins, setup, and the main loop
 * Created on Apr 27, 2018
 *
 */

#include "include.h"

#include "src/drivers/microphone/audio_recording.h"
#include "src/drivers/ble/ble.h"

#define CONSOLE_MODE	1

//***************** Variables *******************

//***************** Teensy Pins *****************
const int ledPin = 13;	// default led pin

const int SDCARD_CS_PIN = BUILTIN_SDCARD;	// 254?
const int SDCARD_MOSI_PIN = 11;
const int SDCARD_SCK_PIN = 13;

//***************** objects *****************
ADC *adc = new ADC();	// adc object


void setup() {
	Serial.begin(9600);	// 57600
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
	AudioRecorder.audioRecording();
	BLE.sendAudiofile();
	delay(45000);	// TODO: go to sleep mode
}
