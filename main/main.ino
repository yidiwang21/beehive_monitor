#include "include.h"
#include "src/drivers/microphone/audio_recording.h"
#include "src/drivers/ble/ble.h"
#include "src/adc/adc.h"
#include "src/drivers/sd/sd.h"

const int ledPin = 13;	// default led pin

void setup() {
	Serial.begin(9600);	// 57600
	BLE._setup();
	_ADC._setup();
	_SD._setup();
	AudioRecorder._setup();
}

void loop() {
	AudioRecorder.audioRecording();
	BLE.sendAudiofile();
	// TODO: go to sleep mode
}
