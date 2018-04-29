/*
 *
 * audioRecording.ino
 * Description: this page includes pins, setup, and the main loop
 * Created on Apr 28, 2018
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

//***************** Functions *******************
void startRecording(void) {
    Serial.println("Start recording");
    if (SD.exists("save.wav")) {
        SD.remove("save.wav");
    }
    audio_saving = SD.open("save.wav", FILE_WRITE);
    if(audio_saving) {
        Serial.println("File successfully opened");
        queue1.begin();
        recordingMode = 1;
    }
}

void stopRecording(void) {
    Serial.println("Stop recording");
    queue1.end();
    while (queue1.available() > 0) {
        queue1.readBuffer();
        queue1.freeBuffer();
    }
    audio_saving.close();
    recordingMode = 0;
}
