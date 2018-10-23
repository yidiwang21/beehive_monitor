#ifndef _BLE_H_
#define _BLE_H_

#include <SD.h>

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