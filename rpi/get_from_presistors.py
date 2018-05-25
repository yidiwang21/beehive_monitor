#! /usr/bin/python
'''
Created on May 17, 2018
Description:

Start bluetoorh connection on rpi:
sudo rfcomm connect hci0 98:D3:41:FD:34:9E 1

'''
import serial
import time
import csv
import datetime

bluetoothSerial = serial.Serial("/dev/rfcomm0", baudrate = 9600)    # FIXME
if bluetoothSerial:
    print("Bluetooth connected")

def writeToFile():
    with open('pResistors.csv', 'wb') as csvfile:
        fw = csv.writer(csvfile, quotechar = '|', quoting = csv.QUOTE_MINIMAL)
        fw.writerow(time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime()))
        for cnt in range(1,6):
            data = bluetoothSerial.readline()
            print(data)
            fw.writerow(data)
        time.sleep(1)

if __name__ == '__main__':
    try:
        while True:
            print("===============================")
            writeToFile()
    except KeyboardInterrupt:
        print("Terminated")
