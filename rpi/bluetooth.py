#! /usr/bin/ python2
import serial
import time

bluetoothSerial = serial.Serial("/dev/ FIXME", baudrate = 115200)
if bluetoothSerial:
    print("Bluetooth connected")

if __name__ == '__main__':
    try:
        while True:
            data = bluetoothSerial.readline()
            print(data)
            time.sleep(1)
            bluetoothSerial.write(data)
    except KeyboardInterrupt:
        print("Terminated")
