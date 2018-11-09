#ifndef _BLE_H_
#define _BLE_H_

#include <SD.h>
#include "Adafruit_BLE_UART.h"

// 24MHz @ 22050 originally
// 24MHz @ 11025 now
#define SMALL_SAMPLE_RATE
#define SEND_BUFFER_SIZE    20

#if defined SMALL_SAMPLE_RATE
#define READ_BUFFER_SIZE    40
#else
#define READ_BUFFER_SIZE    20
#endif

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