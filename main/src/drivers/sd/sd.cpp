#include "sd.h"
#include "../../../include.h"

const int SDCARD_CS_PIN = BUILTIN_SDCARD;	// 254?
const int SDCARD_MOSI_PIN = 11;
const int SDCARD_SCK_PIN = 13;

SdClass::SdClass() {}

void SdClass::_setup(void) {
	SPI.setMOSI(SDCARD_MOSI_PIN);
	SPI.setSCK(SDCARD_SCK_PIN);
	if (!(SD.begin(SDCARD_CS_PIN))) {	// stop here if no SD card, but print a message
    	while (true) {
#ifndef CONSOLE_MODE
      		Serial.println("Unable to access the SD card!");
#endif
			delay(200);
    	}
  	}else{
#ifndef CONSOLE_MODE
  		Serial.println("Successfully access the SD card!");
#endif // 
    }
}

SdClass _SD = SdClass();