#include "ble.h"
#include "../microphone/audio_recording.h"
#include "../../../include.h"

// tried other rx tx pins, didn't work
const int BT_RX = 9;
const int BT_TX = 10;
SoftwareSerial Bluetooth(BT_RX, BT_TX);	// RX, TX

BleClass::BleClass() {};

void BleClass::_setup() {
    Bluetooth.begin(9600);
    Bluetooth.println("Hello Viewer!");
}

// FIXME: 
void BleClass::sendAudiofile(void) {
    audio_rec = AudioRecorder.audio_rec;
    if (SD.exists("save.raw")) {
        audio_rec = SD.open("save.raw");
        Serial.println("File opened.");
    }
    if (audio_rec) {
        Serial.println("Start sending audio file");
        while (audio_rec.available()) {
            Bluetooth.println(audio_rec.read());
        }
        Serial.println("Finished");
        audio_rec.close();
    }else
        Serial.println("Error opening file!");
}

BleClass BLE = BleClass();