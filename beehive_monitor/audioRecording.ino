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
AudioInputAnalog         adc1;           //xy=192,198
AudioRecordQueue         queue1;         //xy=373,269
AudioAnalyzeFFT1024      fft1024_1;      //xy=374,192
AudioConnection          patchCord1(adc1, fft1024_1);
AudioConnection          patchCord2(adc1, queue1);

//***************** Variables *******************
File audio_saving;
int ff = 1;

//***************** Functions *******************
void startRecording(void) {
    Serial.println("Start recording");
    if (SD.exists("save.raw")) {
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
        memcpy(buffer + 256, queue1.readBuffer(), 256);
        queue1.freeBuffer();
        elapsedMicros usec = 0;
        audio_saving.write(buffer, 512);  //256 or 512 (dudes code)
        Serial.print("SD write, us=");
        Serial.println(usec);
    }
}
//
// void audioFFTtest(void) {
//     if (fft1024_1.available()) {
//         float fftAccumulator = 0;
//         for (int i = 0; i < 512; ++i)
//             fftAccumulator += fft1024_1.read(i);
//         ff = (int)(fftAccumulator * 100);
//         for (int i = 0; i < ff; i++) {
//             Serial.print("f");
//             Serial.printf("\tFFT: %d\n", ff);
//         }
//     }else
//         Serial.println("Unavailable fft");
// }

void audioRecording(void) {
    startRecording();
    unsigned long time = millis();
    while (true) {
        if(millis() - time > 5000 && recordingMode == 1) {
            stopRecording();
            break;
        }else continueRecording();
    }
    Serial.println("unplug");
    // for testing
    if (SD.exists("save.raw")) {
        Serial.println("file exists");
    }
    delay(1000);
}
