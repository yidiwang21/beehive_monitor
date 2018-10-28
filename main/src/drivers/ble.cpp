#include "ble.h"
#include "audiofile.h"
#include "../../include.h"

#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 9

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

BleClass::BleClass() {}

void BleClass::_setup() {
    // Bluetooth.begin(9600);
    // Bluetooth.println("Hello Viewer!");
    BTLEserial.begin();
}

void BleClass::sendAudiofile(void) {
    Serial.println("Starting BLE...");

    File f;
    if (SD.exists("save.raw"))
        f = SD.open("save.raw", FILE_READ);
    if (f == NULL)
        errorHalt("* ERROR: File is empty!");

    // char *audio_str = (char*)malloc(AudioRecorder.STR_LENGTH * sizeof(char));
    // Serial.println(AudioRecorder.readRaw(f));
    // strcpy(audio_str, AudioRecorder.readRaw(f));
    
    // Serial.print("str length = "); Serial.println(strlen(audio_str));
    Serial.print("File size: "); Serial.print(f.size()); Serial.println(" bytes");

    unsigned long cursorpos = 0;
    aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;
    // BTLEserial.pollACI();
    
    byte audio_byte[SEND_BUFFER_SIZE];
    byte byteBuf[SEND_BUFFER_SIZE];

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
    
        // TODO: resuming transmision if disconnected
        if (status == ACI_EVT_CONNECTED) {
            int i = 0;
            Serial.println("# Start sending data...");
            unsigned long ble_start_time = millis();
            while(f.available()) {
#ifdef BLE_DEBUG
                if(i > 10) break;
#endif
                BTLEserial.pollACI();
                f.read(byteBuf, SEND_BUFFER_SIZE);
                // strcat((char*)byteBuf, &audio_byte[cursorpos]);
                cursorpos += SEND_BUFFER_SIZE;
                BTLEserial.write(byteBuf, SEND_BUFFER_SIZE);
                i++;
            }
            Serial.print("# Total transmision time: "); Serial.println(millis() - ble_start_time);
            Serial.print("# Totle transmision bytes: "); Serial.println(i * SEND_BUFFER_SIZE);
            break;
        }
    }
    f.close();
    delay(5000);
}

BleClass BLE = BleClass();