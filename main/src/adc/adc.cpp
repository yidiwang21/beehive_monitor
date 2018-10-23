#include "adc.h"
#include "../../include.h"

const int avg_num = 16;
const int resolution = 16;

AdcClass::AdcClass() {}

void AdcClass::_setup(void) {
    ADC *adc = new ADC();	// adc object 
    // ADC1 //
	// use adc1 to resolve adc0 audio input conflict
    adc->setAveraging(avg_num, ADC_1); // set number of averages
    adc->setResolution(resolution, ADC_1); // set bits of resolution
    adc->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED, ADC_1); // change the conversion speed
    adc->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED, ADC_1); // change the sampling speed
}

AdcClass _ADC = AdcClass();