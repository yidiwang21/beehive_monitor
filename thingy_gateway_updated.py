from __future__ import print_function

from bluepy import btle
import sys, os, signal
sys.path.append(os.getcwd())
import base_thingy as thingy52
import binascii
import time
from datetime import datetime
from influxdb import InfluxDBClient
import threading

host = "localhost"
#host = "rten.ucr.edu"
port = 8086
user = "rtenlab"
password = "sensor4world!!"
# database name
dbname = "beehive"

MAC_ADDRESS = None

DEVICE_NAME = 'Thingy'
DEVICE_LIST_FILE_NAME = "thingy_list.csv"
REPEAT_TIME = 300

print("# Looking for Thingy advertisement...")
scanner = btle.Scanner()

def thingyPoller():
    global device_list
    global device_counter
    global DEVICE_NAME
    global DEVICE_LIST_FILE_NAME
    global client
    global REPEAT_TIME
    
    # do it every "REPEAT_TIME" seconds
    threading.Timer(REPEAT_TIME, thingyPoller).start()
    
#    print("searching for new device ...")
    devices = scanner.scan(timeout = 5) # FIXME: list format?
    #    print("{} devices found!".format(len(devices)))
    if (devices!=[]):
#==============================================================================
        for dev in devices:
            #print("Device {} ({}), RSSI={} dB".format(dev.addr, dev.addrType, dev.rssi))
            for (adtype, desc, value) in dev.getScanData():
                #print("  {}, {} = {}".format(adtype, desc, value))
                #if (value == "Thingy"):
                idx = 0
                while idx < len(device_list):
                    if device_list[idx][2] == dev:  # FIXME: device name?
                        break
                    idx += 1
                else: # when none of the previous devices are matched
                print("# Thingy ({}) found with address: {}".format(value, dev.addr))
                device_counter += 1
                new_device = [str(device_counter),
                    DEVICE_NAME+str(device_counter).zfill(2),
                    dev
                    ]
                print("# The new device is named {}".format(new_device[1]))
                device_list.append(new_device)
                # Writing the new device info to table file
# added new dev to the list 
# TODO: write the new dev info to the csv file
# study the code of connecting thingy

    