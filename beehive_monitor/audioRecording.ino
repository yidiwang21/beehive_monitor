/*
 *
 * audioRecording.ino
 * Description: this page includes pins, setup, and the main loop
 * Created on Apr 28, 2018
 *
 * convert a raw file to a wav file:
 * sox -t raw -b 16 -e signed-integer -r 44100 -c 2 input.raw output.wav
 *
 */
//************ Audio System Design **************
AudioInputAnalog         adc1;
AudioRecordQueue         queue1;
AudioConnection          patchCord2(adc1, queue1);

//***************** Variables *******************
File audio_saving;

//***************** Functions *******************
void startRecording(void) {
    Serial.println("Start recording");
    if (SD.exists("save.raw")) {
        Serial.println("Remove existing file");
        SD.remove("save.raw");
    }
    audio_saving = SD.open("save.raw", FILE_WRITE);
    if(audio_saving) {
        Serial.println("File successfully opened");
        queue1.begin();
        recordingMode = 1;
    }
}

void stopRecording(void) {
    Serial.println("Stop recording");
    queue1.end();
    if (recordingMode == 1) {
        while (queue1.available() > 0) {
            audio_saving.write((byte*)queue1.readBuffer(), 256);
            queue1.freeBuffer();
        }
        audio_saving.close();
    }
    recordingMode = 0;
}

void continueRecording(void) {
    if (queue1.available() >= 2) {
        byte buffer[512];
        memcpy(buffer, queue1.readBuffer(), 256);
        queue1.freeBuffer();
        memcpy(buffer + 256, queue1.readBuffer(), 256);
        queue1.freeBuffer();
        elapsedMicros usec = 0;
        audio_saving.write(buffer, 512);  //256 or 512 (dudes code)
        Serial.print("SD write, us=");
        Serial.println(usec);
    }
}

void audioRecording(void) {
    startRecording();
    unsigned long time = millis();
    while (true) {
        if(millis() - time > 5000 && recordingMode == 1) {
            stopRecording();
            break;
        }else continueRecording();
    }
    // for testing
    if (SD.exists("save.raw")) {
        Serial.println("file exists");
    }else {
        Serial.println("file does not exist");
    }
    while(1);
}
