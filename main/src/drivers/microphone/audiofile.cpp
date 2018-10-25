/*
 *
 * audio_recording.cpp
 * Description: 
 * Created on Oct 22, 2018
 *
 */

#include "../../../include.h"
#include "audiofile.h"

//************ Audio System Design **************
AudioInputAnalog         adc1;   // using adc0 of Teensy 3.6
AudioRecordQueue         queue1;
AudioConnection          patchCord2(adc1, queue1);

AudioClass::AudioClass() {
    recordingMode = 0;
}

void AudioClass::_setup(void) {
#ifdef DEBUG
    delay(100);
	Serial.println("Teensy Audio AudioMemory()");
#endif
    AudioMemory(60);	// FIXME: ???
}

//***************** Functions *******************
void AudioClass::startRecording(void) {
#ifdef DEBUG
    Serial.println("Start recording");
#endif
    if (SD.exists("save.raw")) {
#ifdef DEBUG
    Serial.println("Remove existing file");
#endif
        SD.remove("save.raw");
    }
    audio_rec = SD.open("save.raw", FILE_WRITE);
    if(audio_rec) {
#ifdef DEBUG
        Serial.println("File successfully opened");
#endif 
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

#ifdef DEBUG
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

#ifdef DEBUG
    if (SD.exists("save.raw"))
        Serial.println("file exists");
    else Serial.println("file does not exist");
#endif
}

char* AudioClass::readRaw(void) {
    char str[FILE_SIZE] = {'0'};   // 20 bytes to store the file
    if (SD.exists("save.raw"))
        audio_rec = SD.open("save.raw", FILE_READ);
    if (audio_rec == NULL)
        errorHalt("ERROR: File is empty!")
    
    int w = 0;
    int i = -1;
    int j = 0;
    int r1 = 0, r2 = 0, l1 = 0, l2 = 0, l = 0, r = 0;
    while (audio_rec.available()) {
        w = audio_rec.read();
        i++;
        if (i == 0) r1 = (unsigned)w;
        if (i == 1) { 
            r2 = (unsigned)w;
            r = (r2 << 8) + r1; // r1 | r2 << 8
        }
        if (i == 2) l1 = (unsigned)w;
        if (i == 3) {
            l2 = (unsigned)w;
            l = (l2 << 8) + l1;
            str[j++] = (char)(r && 0xff);
            str[j++] = (char)((r >> 8) && 0xff);
            str[j++] = (char)(l && 0xff);
            str[j++] = (char)((l >> 8) && 0xff);
            i = -1;
        }
    }
    return str;
}

AudioClass AudioRecorder = AudioClass();