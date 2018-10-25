#include "include.h"
#include "src/drivers/microphone/audiofile.h"
#include "src/drivers/ble/ble.h"
#include "src/adc/adc.h"
#include "src/drivers/sd/sd.h"

SnoozeAlarm alarm;
SnoozeBlock config_teensy36(alarm);

#define SLEEP_TIME_HR	0
#define SLEEP_TIME_MIN	0
#define SLEEP_TIME_SEC	10

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	alarm.setRtcTimer(SLEEP_TIME_HR, SLEEP_TIME_MIN, SLEEP_TIME_SEC);	// hours, minutes, seconds
#ifdef DEBUG
	Serial.begin(9600);
#endif
	BLE._setup();
	_ADC._setup();
	_SD._setup();
	AudioRecorder._setup();

	// startup indicator
	for (int i = 0; i < 1; i++) {
		digitalWrite(LED_BUILTIN, HIGH); delay(200);
		digitalWrite(LED_BUILTIN, LOW); delay(200);
    }
}

void loop() {
	int who;
	who = Snooze.hibernate(config_teensy36);	// get into sleep for 10 min until wakeup
	if (who == 35){	// rtc wakeup value
	// polling every 10 min
		AudioRecorder.audioRecording();
		BLE.sendAudiofile();	
	}
	// TODO: sleep + running = 15 min
}
