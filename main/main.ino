#include "include.h"
#include "src/drivers/audiofile.h"
#include "src/drivers/ble.h"
#include "src/adc/adc.h"  
#include "src/drivers/sd.h"

SnoozeAlarm alarm;
//SnoozeTimer timer;
SnoozeBlock config_teensy36(alarm);
//SnoozeBlock config_teensy36(timer);

#define SLEEP_MODE_ENABLED
// #define AUDIO_DEBUG
// #define BLE_DEBUG

#define SLEEP_TIME_HR	0
#define SLEEP_TIME_MIN	0
#define SLEEP_TIME_SEC	20

#define EX_LED_PIN		31

void ledBlink(int cnt) {
	for (int i = 0; i < cnt; i++) {
		digitalWrite(EX_LED_PIN, HIGH); delay(500);
		digitalWrite(EX_LED_PIN, LOW); delay(500);
    }
}

void setup() {
	pinMode(EX_LED_PIN, OUTPUT);
	Serial.begin(9600);
	BLE._setup();
	// _ADC._setup();	// FIXME: conflict with audio pin
	_SD._setup();
	AudioRecorder._setup();
	Serial.println("**********************************************");
	ledBlink(2);
	//timer.setTimer(30000);
	alarm.setRtcTimer(SLEEP_TIME_HR, SLEEP_TIME_MIN, SLEEP_TIME_SEC);	// hours, minutes, seconds
}

// TODO: 
// assign audio objects in the loop, however, queue will generate interrupts
// try to use AudioNoInterrupts() to avoid it; and AudioInterrupts() to re-enable
void loop() {
	SIM_SCGC6 &= ~SIM_SCGC6_I2S;
  int who;
  who = Snooze.deepSleep(config_teensy36);  // get into sleep for 10 min until wakeup
  SIM_SCGC6 |= SIM_SCGC6_I2S;
	
	//
	if (who == 35) {
	  Serial.println("**********************************************");
    delay(8000);
//     I2S0_TCSR |= I2S_TCSR_TE;
//     I2S0_RCSR |= I2S_RCSR_RE;
		ledBlink(1);
		AudioRecorder.audioRecording();
    	delay(1000);
  
	// BLE.sendAudiofile();	
	}

	#ifdef SLEEP_MODE_ENABLED
	Serial.println("Entering sleeping mode...");
	
#else
	int who = 35;
#endif

#ifndef SLEEP_MODE_ENABLED
	while(1);
#endif
}
