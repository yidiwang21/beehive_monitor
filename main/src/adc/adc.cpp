#include "adc.h"
#include "../../include.h"

void AdcClass::AdcClass() {
    ADC *adc = new ADC();	// adc object
    avg_num = 16;
    resolution = 16;
}

void AdcClass::_setup(void) {
     // ADC1 //
	// use adc1 to resolve adc0 audio input conflict
    adc->setAveraging(avg_num, ADC_1); // set number of averages
    adc->setResolution(resolution, ADC_1); // set bits of resolution
    adc->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED, ADC_1); // change the conversion speed
    adc->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED, ADC_1); // change the sampling speed
}

AdcClass _ADC = AdcClass();