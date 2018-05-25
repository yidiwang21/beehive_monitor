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

bluetoothSerial = serial.Serial("/dev/rfcomm0", baudrate = 9600)    # FIXME
if bluetoothSerial:
    print("Bluetooth connected")

def writeToFile():
    fw = csv.writer(csvfile, delimiter = ',', quotechar = '|', quoting = csv.QUOTE_MINIMAL)
    list = []
    for cnt in range(0,6):
        data = bluetoothSerial.readline()
        print(str(data))
        list.append(str(data))
    fw.writerow([time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime()), list])
    time.sleep(1)

if __name__ == '__main__':
    try:
        with open('pResistors.csv', 'wb') as csvfile:
            while True:
                print("===============================")
                writeToFile()
    except KeyboardInterrupt:
        print("Terminated")
