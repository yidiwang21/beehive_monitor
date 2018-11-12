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
	Serial.println("# Teensy Audio AudioMemory()");
    AudioMemory(60);
}

// TODO: not overwrite file every time, keep max 10 files 
void AudioClass::startRecording(void) {
    // audio_queue.update();
    if (SD.exists("save.raw")) {
    Serial.println("# Existing file removed");
        SD.remove("save.raw");
    }
    audio_rec = SD.open("save.raw", FILE_WRITE);
    if(audio_rec) {
        Serial.println("# Start recording");
        Serial.print("# Queue available ?"); Serial.println(audio_queue.available());
        audio_queue.begin();
        recordingMode = 1;
    }
}

void AudioClass::stopRecording(void) {
    // NP
    Serial.println("# Stop recording");
    Serial.print("File size: "); Serial.print(audio_rec.size()); Serial.println(" bytes");
    audio_queue.end();
    if (recordingMode == 1) {
        while (audio_queue.available() > 0) {
            audio_rec.write((byte*)audio_queue.readBuffer(), 256);
            audio_queue.freeBuffer();
        }
        audio_rec.close();
    }
    // audio_queue.clear();
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
#ifdef AUDIO_DEBUG
        Serial.print("SD write, us=");
        Serial.println(usec);
#endif 
    }
}

void AudioClass::audioRecording(void) {
    startRecording();
    unsigned long time = millis();
    while (millis() - time < REC_TIME) {
        // NP
        continueRecording();
    }
    stopRecording();
}

/* char* AudioClass::readRaw(File f) {
    char audio_str[MAX_FILE_SIZE] = {'0'};   // 20 bytes to store the file
    
    int k = 0;
    int w = 0;
    int i = -1;
    int j = 0;
    int r1 = 0, r2 = 0, l1 = 0, l2 = 0, l = 0, r = 0;
    while (f.available()) {
        byte msb = f.read();
        byte lsb = f.read();
        int val = (msb << 8) + lsb;
        audio_str[k] = (char)(val & 0xff);
        Serial.print(audio_str[k]);
        audio_str[++k] = (char)((val >> 8) & 0xff);
        Serial.println(audio_str[k]);
    }
    Serial.print("k = "); Serial.print(k);
    STR_LENGTH = k;
}
 */

AudioClass AudioRecorder = AudioClass();