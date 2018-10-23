/*
 *
 * audio_recording.cpp
 * Description: 
 * Created on Oct 22, 2018
 *
 */

#include "../../../include.h"
#include "audio_recording.h"

//************ Audio System Design **************
AudioInputAnalog         adc1;   // using adc0 of Teensy 3.6
AudioRecordQueue         queue1;
AudioConnection          patchCord2(adc1, queue1);

const int audioPin = 16;  // adc0 of Teensy 3.6
const int audioTrigger = 0;

AudioClass::AudioClass() {
    recordingMode = 0;
};

void AudioClass::_setup(void) {};

//***************** Functions *******************
void AudioClass::startRecording(void) {
    Serial.println("Start recording");
    if (SD.exists("save.raw")) {
        Serial.println("Remove existing file");
        SD.remove("save.raw");
    }
    audio_rec = SD.open("save.raw", FILE_WRITE);
    if(audio_rec) {
        Serial.println("File successfully opened");
        queue1.begin();
        recordingMode = 1;
    }
}


void AudioClass::stopRecording(void) {
    Serial.println("Stop recording");
    queue1.end();
    if (recordingMode == 1) {
        while (queue1.available() > 0) {
            audio_rec.write((byte*)queue1.readBuffer(), 256);
            queue1.freeBuffer();
        }
        audio_rec.close();
    }
    recordingMode = 0;
}

void AudioClass::continueRecording(void) {
    if (queue1.available() >= 2) {
        byte buffer[512];
        memcpy(buffer, queue1.readBuffer(), 256);
        queue1.freeBuffer();
        memcpy(buffer + 256, queue1.readBuffer(), 256);
        queue1.freeBuffer();
        elapsedMicros usec = 0;
        audio_rec.write(buffer, 512);  //256 or 512 (dudes code)

#if CONSOLE_MODE
        Serial.print("SD write, us=");
        Serial.println(usec);
#endif
    }
}

void AudioClass::audioRecording(void) {
    startRecording();
    unsigned long time = millis();
    while (true) {
        if(millis() - time > 15000 && recordingMode == 1) {
            stopRecording();
            break;
        }else continueRecording();
    }

#if CONSOLE_MODE
    if (SD.exists("save.raw"))
        Serial.println("file exists");
    else Serial.println("file does not exist");
#endif
}

AudioClass AudioRecorder = AudioClass();