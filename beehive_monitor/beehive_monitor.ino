/*
 *
 * beehive_monitor.ino
 * Description: this page includes pins, setup, and the main loop
 * Created on Apr 26, 2018
 *
 */
#include <Arduino.h>
#include "TeensyThreads.h"
#include <math.h>
#include <ADC.h>

#define pResistor_UNIT_NUM 3	// FIXME: for testing

ADC *adc = new ADC();	// adc object

//***************** Variables *******************

//***************** Teensy Pins *****************
const int ledPin = 13;	// default led pin
/* photoresistors pins already defined in pResistor.ino
const int pResistor1 = 31;
const int pResistor2 = 32;
const int pResistor3 = 33;
*/
void setup() {
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

	Serial.begin(9600);	// 57600
	delay(200);
}

void loop() {
	pResistorMonitor();
}
