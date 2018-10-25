#include "sd.h"
#include "../../include.h"

// not actually used with Teensy 3.6
// #define SDCARD_MOSI_PIN 11
// #define SDCARD_SCK_PIN  13

SdClass::SdClass() {}

void SdClass::_setup(void) {
	// SPI.setMOSI(SDCARD_MOSI_PIN);
	// SPI.setSCK(SDCARD_SCK_PIN);
	if (!(SD.begin(SDCARD_CS_PIN))) {	// stop here if no SD card, but print a message
    	while (true) {
      		Serial.println("Unable to access the SD card!");
			delay(200);
    	}
  	}else{
  		Serial.println("Successfully access the SD card!");
    }
}

SdClass _SD = SdClass();