#! /usr/bin/python
'''
Created on May 24, 2018
Description: for receiving audio file

Start bluetoorh connection on rpi:
sudo rfcomm connect hci0 98:D3:41:FD:34:9E 1

'''
import serial
import time

bluetoothSerial = serial.Serial("/dev/rfcomm0", baudrate = 9600)    # FIXME
if bluetoothSerial:
    print("Bluetooth connected")

class saveFile():
    def __init__(self):
        self.f = open("input.raw", "w")

    def getData(self):
        data = bluetoothSerial.readline()
        time.sleep(1)
        return data

    def writeToFile(self, data):
            data = data.format(r"\r\n\\")
            self.f.write(data)

    def closeFile(self):
        self.f.close()

    def main(self):
        while True:
            data = self.getData()
            if data:
                self.writeToFile(data)
            else:
                break
        self.closeFile()

if __name__ == '__main__':
    try:
        ins = saveFile()
        ins.main()
    except KeyboardInterrupt:
        print("Terminated")
