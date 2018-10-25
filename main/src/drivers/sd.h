#ifndef _SD_H_
#define _SD_H_

#define SDCARD_CS_PIN   BUILTIN_SDCARD  // 254?

class SdClass
{
public:
    SdClass();
    void _setup(void);    
};

extern SdClass _SD;

#endif