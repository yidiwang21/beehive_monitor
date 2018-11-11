#include "include.h"
#include "src/drivers/audiofile.h"
#include "src/drivers/ble.h"
#include "src/adc/adc.h"  
#include "src/drivers/sd.h"

SnoozeAlarm alarm;
// SnoozeTimer timer;
SnoozeBlock config_teensy36(alarm);
// SnoozeBlock config_teensy36(timer);

#define SLEEP_MODE_ENABLED
// #define AUDIO_DEBUG
// #define BLE_DEBUG

#define SLEEP_TIME_HR	0
#define SLEEP_TIME_MIN	0
#define SLEEP_TIME_SEC	30

#define EX_LED_PIN		31

void setup() {
	pinMode(EX_LED_PIN, OUTPUT);
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
	// timer.setTimer(10000);
	alarm.setRtcTimer(SLEEP_TIME_HR, SLEEP_TIME_MIN, SLEEP_TIME_SEC);	// hours, minutes, seconds
}

void loop() {

	delay(2000);
	for (int i = 0; i < 1; i++) {
		digitalWrite(EX_LED_PIN, HIGH); delay(500);
		digitalWrite(EX_LED_PIN, LOW); delay(500);
	}
	Serial.println("**********************************************");
	// FIXME: recording failed after sleep
	AudioRecorder.audioRecording();
	// BLE.sendAudiofile();	
	delay(2000);

	#ifdef SLEEP_MODE_ENABLED
	Serial.println("Entering sleeping mode...");
	Snooze.hibernate(config_teensy36);	// get into sleep for 10 min until wakeup
#else
	int who = 35;
#endif

#ifndef SLEEP_MODE_ENABLED
	while(1);
#endif
}
