#include "ble.h"
#include "audiofile.h"
#include "../../include.h"

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
    Serial.println("Starting BLE...");

    File f;
    if (SD.exists("save.raw"))
        f = SD.open("save.raw", FILE_READ);
    if (f == NULL)
        errorHalt("ERROR: File is empty!");

    Serial.print("STR_LENGTH: "); Serial.println(AudioRecorder.STR_LENGTH);
    char *audio_str = (char*)malloc(AudioRecorder.STR_LENGTH * sizeof(char));
    Serial.println(AudioRecorder.readRaw(f));
    // FIXME: returned str len = 0
    strcpy(audio_str, AudioRecorder.readRaw(f));
    
    Serial.print("str length = "); Serial.println(strlen(audio_str));

    Serial.print("File size: "); Serial.print(f.size()); Serial.println(" bytes");
    Serial.println("raw file reading finished.");

    byte byteBuf[SEND_BUFFER_SIZE];

    unsigned long i = 0;
    unsigned long cursorpos = 0;
    unsigned long ble_start_time = millis();
    aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;
    // BTLEserial.pollACI();
    
    // while(millis() - ble_start_time < 60000) {    // FIXME: set 60 secs for file transmision
    while (true) {
        // tell nRF8001 to process data
    
    
        BTLEserial.pollACI();
        aci_evt_opcode_t status = BTLEserial.getState();
        
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
            while(cursorpos < f.size()) {
                unsigned long tm_start = millis();
                // wait for "ready to send" signal from rpi
                while (BTLEserial.available()) {   // FIXME: 
                    Serial.println("ggggggggg");
                    char c = BTLEserial.read();
                    if (c == 'y') break;
                }// ready to send, every 20 bytes
                strcat((char*)byteBuf, audio_str + (i * SEND_BUFFER_SIZE));
                i++;
                cursorpos += SEND_BUFFER_SIZE;
                Serial.print("cursorpos: "); Serial.println(cursorpos);
                BTLEserial.write(byteBuf, SEND_BUFFER_SIZE);
                delay(10);
            }
            Serial.println("111111111111111111111111");
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