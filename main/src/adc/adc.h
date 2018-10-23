#ifndef _ADC_H_
#define _ADC_H_

class AdcClass
{
public:
    AdcClass();
    void _setup(void);
private:
    const int avg_num;
    const int resolution;
};

extern AdcClass _ADC;
#endif