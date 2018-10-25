/*
 *
 * audiofile.cpp
 * Description: 
 * Created on Oct 22, 2018
 *
 */

#include "../../include.h"
#include "audiofile.h"

//************ Audio System Design **************
AudioInputAnalog         audio_adc;   // using adc0 of Teensy 3.6
AudioRecordQueue         audio_queue;
AudioConnection          patchCord2(audio_adc, audio_queue);

File audio_rec;

AudioClass::AudioClass() {
    recordingMode = 0;
}

void AudioClass::_setup(void) {
    delay(100);
	Serial.println("Teensy Audio AudioMemory()");
    AudioMemory(60);	// FIXME: ???
}

// TODO: not overwrite file every time, keep max 10 files 
void AudioClass::startRecording(void) {
    Serial.println("Start recording");
    if (SD.exists("save.raw")) {
    Serial.println("Remove existing file");
        SD.remove("save.raw");
    }
    audio_rec = SD.open("save.raw", FILE_WRITE);
    if(audio_rec) {
        Serial.println("File successfully opened");
        audio_queue.begin();
        recordingMode = 1;
    }
}


void AudioClass::stopRecording(void) {
    Serial.println("Stop recording");
    audio_queue.end();
    if (recordingMode == 1) {
        while (audio_queue.available() > 0) {
            audio_rec.write((byte*)audio_queue.readBuffer(), 256);
            audio_queue.freeBuffer();
        }
        audio_rec.close();
    }
    recordingMode = 0;
}

void AudioClass::continueRecording(void) {
    if (audio_queue.available() >= 2) {
        byte buffer[512];
        memcpy(buffer, audio_queue.readBuffer(), 256);
        audio_queue.freeBuffer();
        memcpy(buffer + 256, audio_queue.readBuffer(), 256);
        audio_queue.freeBuffer();
        elapsedMicros usec = 0;
        audio_rec.write(buffer, 512);  //256 or 512 (dudes code)
        Serial.print("SD write, us=");
        Serial.println(usec);
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

    if (SD.exists("save.raw"))
        Serial.println("file exists");
    else Serial.println("file does not exist");
}

char* AudioClass::readRaw(File f) {
    // char str[FILE_SIZE] = {'0'};   // 20 bytes to store the file
    char* str;
    
    int w = 0;
    int i = -1;
    int j = 0;
    int r1 = 0, r2 = 0, l1 = 0, l2 = 0, l = 0, r = 0;
    while (f.available()) {
        w = f.read();
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