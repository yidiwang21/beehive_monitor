#ifndef _BLE_H_
#define _BLE_H_

#include <SD.h>
#include "Adafruit_BLE_UART.h"

class BleClass
{
public:
    BleClass();
    void _setup(void);
    void sendAudiofile(void);

private:
    File audio_rec;    
    aci_evt_opcode_t laststatus;
    aci_evt_opcode_t status;
};

extern BleClass BLE;

#endif  