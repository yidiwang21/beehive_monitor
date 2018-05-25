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

print("===============================")
with open('pResistors.csv', 'wb') as csvfile:
    fw = csv.writer(csvfile, quotechar = '|')
    fw.writerow(time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime()))
    for cnt in range(0,6):
        data = bluetoothSerial.readline()
        print(data)
        fw.writerow(str(data))
    time.sleep(1)
