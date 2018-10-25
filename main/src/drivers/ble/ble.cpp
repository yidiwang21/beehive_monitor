#include "ble.h"
#include "../microphone/audiofile.h"
#include "../../../include.h"

// tried other rx tx pins, didn't work
// #define BT_RX   9
// #define BT_TX   10
// SoftwareSerial Bluetooth(BT_RX, BT_TX);	// RX, TX

#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 9

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

BleClass::BleClass() {
    laststatus = ACI_EVT_DISCONNECTED;
}

void BleClass::_setup() {
    // Bluetooth.begin(9600);
    // Bluetooth.println("Hello Viewer!");
    BTLEserial.begin();
}

void BleClass::sendAudiofile(void) {
    // tell nRF8001 to process data
    BTLEserial.pollACI();
    
    char* str = AudioRecorder.readRaw();
    // need to convert the file to bytes, no more than 20 as defined 
    int8_t sendbuffer[20];

    status = BTLEserial.getState();
    if (status != laststatus) {
#ifdef DEBUG
        if (status == ACI_EVT_DEVICE_STARTED) {
            Serial.println(F("* Advertising started"));
        }
        if (status == ACI_EVT_CONNECTED) {
            Serial.println(F("* Connected!"));
        }
        if (status == ACI_EVT_DISCONNECTED) {
            Serial.println(F("* Disconnected or advertising timed out"));
        }
#endif
        laststatus = status;
    }
    if (status == ACI_EVT_CONNECTED) {

    }

  
}

// FIXME: 
/* void BleClass::sendAudiofile(void) {
    audio_rec = AudioRecorder.audio_rec;
    if (SD.exists("save.raw")) {
        audio_rec = SD.open("save.raw");
#ifdef DEBUG
        Serial.println("File opened.");
#endif
    }
    if (audio_rec) {
#ifdef DEBUG
        Serial.println("Start sending audio file");
#endif
        while (audio_rec.available()) {
            Bluetooth.println(audio_rec.read());
        }
#ifdef DEBUG
        Serial.println("Finished");
#endif
        audio_rec.close();
    }
} */

BleClass BLE = BleClass();