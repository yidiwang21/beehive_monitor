#! /usr/bin/python
'''
Created on May 17, 2018
Description:

Start bluetoorh connection on rpi:
sudo rfcomm connect hci0 98:D3:41:FD:34:9E 1

'''
import serial
import time

bluetoothSerial = serial.Serial("/dev/rfcomm0", baudrate = 9600)    # FIXME
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
