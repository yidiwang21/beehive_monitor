#include "include.h"
#include "src/drivers/audiofile.h"
#include "src/drivers/ble.h"
#include "src/adc/adc.h"  
#include "src/drivers/sd.h"

SnoozeAlarm alarm;
SnoozeBlock config_teensy36(alarm);

// #define SLEEP_MODE_ENABLED
// #define AUDIO_DEBUG
// #define BLE_DEBUG

#define SLEEP_TIME_HR	0
#define SLEEP_TIME_MIN	2
#define SLEEP_TIME_SEC	0

#define EX_LED_PIN		31

void setup() {
	pinMode(EX_LED_PIN, OUTPUT);
	alarm.setRtcTimer(SLEEP_TIME_HR, SLEEP_TIME_MIN, SLEEP_TIME_SEC);	// hours, minutes, seconds
	Serial.begin(9600);
	BLE._setup();
	// _ADC._setup();	// FIXME: conflict with audio pin
	_SD._setup();
	AudioRecorder._setup();
	Serial.println("**********************************************");
	for (int i = 0; i < 1; i++) {
		digitalWrite(EX_LED_PIN, HIGH); delay(200);
		digitalWrite(EX_LED_PIN, LOW); delay(200);
    }
}

void loop() {
#ifdef SLEEP_MODE_ENABLED
	Serial.println("Entering sleeping mode...");
	int who;
	who = Snooze.hibernate(config_teensy36);	// get into sleep for 10 min until wakeup
#else
	int who = 35;
#endif

	if (who == 35){	// rtc wakeup value
	// polling every 10 min
		for (int i = 0; i < 1; i++) {
			digitalWrite(EX_LED_PIN, HIGH); delay(200);
			digitalWrite(EX_LED_PIN, LOW); delay(200);
		}
		Serial.println("**********************************************");
		AudioRecorder.audioRecording();
		BLE.sendAudiofile();	
	}
	// TODO: sleep + running = 15 min
#ifndef SLEEP_MODE_ENABLED
	while(1);
#endif
}
