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

BleClass::BleClass() {}

void BleClass::_setup() {
    // Bluetooth.begin(9600);
    // Bluetooth.println("Hello Viewer!");
    Serial.println("Setup BLE...");
    BTLEserial.begin();
}

void BleClass::sendAudiofile(void) {
    // tell nRF8001 to process data
    BTLEserial.pollACI();
    Serial.println("Starting BLE...");

    File f;
    if (SD.exists("save.raw"))
        f = SD.open("save.raw", FILE_READ);
    if (f == NULL)
        errorHalt("ERROR: File is empty!")
    char* str = AudioRecorder.readRaw(f);
    byte* byteBuf;

    unsigned long i = 0;
    unsigned long cursorpos = 0;

    aci_evt_opcode_t status = BTLEserial.getState();
    aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;
    
    if (status != laststatus) {
        if (status == ACI_EVT_DEVICE_STARTED) {
            Serial.println(F("* Advertising started"));
        }
        if (status == ACI_EVT_CONNECTED) {
            Serial.println(F("* Connected!"));
        }
        if (status == ACI_EVT_DISCONNECTED) {
            Serial.println(F("* Disconnected or advertising timed out"));
        }
        laststatus = status;
    }
    if (status == ACI_EVT_CONNECTED) {
        while(!f.seek(cursorpos)) {
            // wait for "ready to send" signal from rpi
            while (BTLEserial.available()) {
                char c = BTLEserial.read();
                if (c == 'y') break;
                // TODO: add timeout
            }// ready to send, every 20 bytes
            strcat((char*)byteBuf, str + (i * SEND_BUFFER_SIZE));
            i++;
            cursorpos += SEND_BUFFER_SIZE;
            BTLEserial.write(byteBuf, SEND_BUFFER_SIZE);
        }
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