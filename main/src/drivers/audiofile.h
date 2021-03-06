/*
 *
 * audiofile.h
 * Description: 
 * Created on Oct 22, 2018
 *
 */

#ifndef _AUDIOFILE_H_
#define _AUDIOFILE_H_

#include <SD.h>
#include <Audio.h>

#define AUDIO_PIN       16  // adc0 of Teensy 3.6
#define AUDIO_TRIGGER   0   // useless for now

// #define AUDIO_DEBUG

#define MAX_FILE_SIZE 10 * 1000 * 1000
#define REC_TIME    2000

class AudioClass
{
public:
    AudioClass();
    void audioRecording(void);
    void sendAudiofile(void);
    void _setup(void);
    // char* readRaw(File f);

private:
    void startRecording(void);
    void stopRecording(void);
    void continueRecording(void);    
    size_t readField(File* file, char* str, size_t size, char* delim);
    int recordingMode;
};

extern AudioClass AudioRecorder;

#endif