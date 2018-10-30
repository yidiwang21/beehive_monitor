#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"

using namespace std;

#define SLEEPING_CURRENT    0.056   // mA
#define RUNNING_CURRENT     25      // mA
#define RUNNING_UNIT_TIME   1.5     // sending 1s audio file needs 1.5min, neglecting recording time        

int main(int argc, char *argv[]) {
    /* if (argc != 5) {
        cout<<"# Usage: ./energy_calc -s [sampling rate (times/h)] -r [recording time (s)]"<<endl;
        abort();
    } */

    int opt;
    int idx = 1;
    int sampling_rate = 0;
    int recording_time = 0;
    
    while ((opt = getopt(argc, argv, "sr:")) != EOF) {
        switch (opt)
        {
            case 's': 
                sampling_rate = atoi(argv[idx+1]); idx = idx + 2; break;
            case 'r': recording_time = atoi(argv[idx+1]); idx = idx + 2; break;
            case '?': fprintf(stderr, "# Usage: ./energy_calc -s [sampling rate (times/h)] -r [recording time (s)]");
            default: cout<<endl; break;
        }
    }

    float running_time = sampling_rate * RUNNING_UNIT_TIME * recording_time;  // minutes/hour
    float running_energy_hr = running_time / 60 * RUNNING_CURRENT;
    float sleeping_energy_hr = (60 - running_time) / 60 * SLEEPING_CURRENT;
    float total_energy_month = (running_energy_hr + sleeping_energy_hr) * 24 * 30;
    cout<<"Total energy in a month: "<<total_energy_month<<endl;
    return 0;
}
