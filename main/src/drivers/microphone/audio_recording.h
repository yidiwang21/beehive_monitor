/*
 *
 * audio_recording.h
 * Description: 
 * Created on Oct 22, 2018
 *
 */

#ifndef _AUDIO_RECORDING_H_
#define _AUDIO_RECORDING_H_

#include <SD.h>

class AudioClass
{
public:
    AudioClass();
    void audioRecording(void);
    void sendAudiofile(void);
    void _setup(void);
    File audio_rec;

private:
    void startRecording(void);
    void stopRecording(void);
    void continueRecording(void);    
    int recordingMode;
};

extern AudioClass AudioRecorder;

#endif