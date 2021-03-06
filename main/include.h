#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include <Arduino.h>
#include "TeensyThreads.h"
#include <math.h>
#include <ADC.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <SoftwareSerial.h>

#include <Snooze.h>
#include "Adafruit_BLE_UART.h"

#define errorHalt(msg) {Serial.println(F(msg)); while(1);}

#endif