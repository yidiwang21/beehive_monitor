/*
 *
 * audioRecording.ino
 * Description: this page includes pins, setup, and the main loop
 * Created on Apr 28, 2018
 *
 * convert a raw file to a wav file:
 * sox -t raw -b 16 -e signed-integer -r 22050 -c 2 save.raw output.wav
 *
 */
//************ Audio System Design **************
AudioInputAnalog         adc1;
AudioRecordQueue         queue1;
AudioConnection          patchCord2(adc1, queue1);

//***************** Variables *******************
File audio_rec;

//***************** Functions *******************
void startRecording(void) {
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

void stopRecording(void) {
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

void continueRecording(void) {
    if (queue1.available() >= 2) {
        byte buffer[512];
        memcpy(buffer, queue1.readBuffer(), 256);
        queue1.freeBuffer();
        memcpy(buffer + 256, queue1.readBuffer(), 256);
        queue1.freeBuffer();
        elapsedMicros usec = 0;
        audio_rec.write(buffer, 512);  //256 or 512 (dudes code)
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
    if (SD.exists("save.raw"))
        Serial.println("file exists");
    else Serial.println("file does not exist");
    flag = 2;
    while(1);
}

void sendAudioFile(void) {
    if (SD.exists("save.raw")) {
        audio_rec = SD.open("save.raw");
        Serial.println("File opened.");
    }
    if (audio_rec) {
        Serial.println("Start sending audio file");
        while (audio_rec.available()) {
            Bluetooth.write(audio_rec.read());
            Serial.println("fuck");
        }
        Serial.println("Finished");
        audio_rec.close();
    }else
        Serial.println("Error opening file!");
    flag = 3;
}
