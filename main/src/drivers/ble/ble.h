#ifndef _BLE_H_
#define _BLE_H_

#include <SD.h>
#include "Adafruit_BLE_UART.h"

// need to convert the file to bytes, no more than 20 as defined 
#define SEND_BUFFER_SIZE    20

class BleClass
{
public:
    BleClass();
    void _setup(void);
    void sendAudiofile(void);

private:
    File audio_rec;    
};

extern BleClass BLE;

#endif  