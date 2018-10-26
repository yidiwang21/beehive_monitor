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

    // char *audio_str = (char*)malloc(AudioRecorder.STR_LENGTH * sizeof(char));
    // Serial.println(AudioRecorder.readRaw(f));
    // strcpy(audio_str, AudioRecorder.readRaw(f));
    
    // Serial.print("str length = "); Serial.println(strlen(audio_str));
    Serial.print("File size: "); Serial.print(f.size()); Serial.println(" bytes");

    byte byteBuf[SEND_BUFFER_SIZE];

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
            if (status == ACI_EVT_DEVICE_STARTED)
                Serial.println(F("* Advertising started"));
            if (status == ACI_EVT_CONNECTED)
                Serial.println(F("* Connected!"));
            if (status == ACI_EVT_DISCONNECTED)
                Serial.println(F("* Disconnected or advertising timed out"));
            laststatus = status;
        }

        byte audio_byte[f.size()];
        int i = 0;
        while(f.available()) {
            audio_byte[i] = f.read();
            ++i;
        }
        i = 0;
        // TODO: resuming transmision if disconnected
        if (status == ACI_EVT_CONNECTED) {
            while(f.available()) {
                BTLEserial.pollACI();
                unsigned long tm_start = millis();
                // wait for "ready to send" signal from rpi
                // while (BTLEserial.available()) {   // FIXME: 
                //     char c = BTLEserial.read();
                //     if (c == 'y') break;
                // }// ready to send, every 20 bytes
                /* for (int i = 0; i < SEND_BUFFER_SIZE; i = i + 2) {
                    byte msb = f.read();
                    byte lsb = f.read();
                    byteBuf[i] = msb; 
                    byteBuf[i+1]  = lsb;
                } */
                // strcat((char*)byteBuf, &audio_byte[cursorpos]);
                cursorpos += SEND_BUFFER_SIZE;
                // Serial.print("cursorpos: "); Serial.println(cursorpos);
                BTLEserial.write(&audio_byte[cursorpos], SEND_BUFFER_SIZE);
            }
            Serial.print("Total transmision time: "); Serial.println(millis() - ble_start_time);
            break;
        }
    }
}

BleClass BLE = BleClass();