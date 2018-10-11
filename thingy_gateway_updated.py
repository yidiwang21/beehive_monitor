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
    devices = scanner.scan(timeout = 5)
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
                    if device_list[idx][2] == dev.addr:  
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
a# TODO: write the new dev info to the csv file
# study the code of connecting thingy
                try:
                    device_data = open(DEVICE_LIST_FILE_NAME, "a")
                    device_data.write(','.join(new_device))
                    device_data.write("\n")
                    device_data.flush()    
                    device_data.close()
                    print("# Writing to the file finished successfully")
                except:
                    print("# Error!!! File {} is not accessible!"\
                    .format(DEVICE_LIST_FILE_NAME))
                print()
#==============================================================================        
        # Gathering information from each available device
        for dev in devices:
            idx = 0
            while idx < len(device_Peripheral
                if device_list[idx]Peripheralc addr in the list
                    temp_val = 0
                    hum_val = 0
                    co2_val = 0
                    tvoc_val = 0
                    pressure_bal = Peripheral
                    battery_val = 0Peripheral
                    MAC_ADDRESS = device_list[index][2]
                    device_name = device_list[index][1]
                    # RPI connects to thingy and then disconnect
                    print("# Connecting to Thingy with address {}...".format(MAC_ADDRESS))
                    thingy = thingy52.Thingy52(MAC_ADDRESS)
                    print("# Setting notification handler to default handler...")
                    #thingy.setDelegate(thingy52.MyDelegate())
                    thingy.setDelegate(MyDelegate())
                    #print("# Setting notification handler to new handler...")
                    #thingy.setDelegate(NewDelegate())

                    print("# Configuring and enabling sensor notification...")
                    thingy.battery.enable()
                    thingy.environment.enable()
                    thingy.environment.configure(temp_int=60000, humid_int=60000, gas_mode_int=3, press_int=60000)
                    #thingy.environment.configure(temp_int=5000, humid_int=5000, gas_mode_int=2)
                    #thingy.environment.configure(temp_int=1000, humid_int=1000, gas_mode_int=1)
                    thingy.environment.set_temperature_notification(True)
                    thingy.environment.set_gas_notification(True)
                    thingy.environment.set_humidity_notification(True)
                    thingy.environment.set_pressure_notification(True)
                    #thingy.sound.enable()
                    #thingy.sound.configure(microphone_mode=0x01)
                    #thingy.sound.set_microphone_notification(True)

                    #print("# Configuring and enabling button press notification...")
                    #thingy.ui.enable()
                    #thingy.ui.set_btn_notification(True)

                    #print("# Waiting for three notifications...")
                    #thingy.waitForNotifications(timeout=5)
                    #thingy.waitForNotifications(timeout=5)
                    # thingy.waitForNotifications(timeout=5)
                    
                    thingy.waitForNotifications(timeout = 5)
                    d = datetime.now()
                    runNo = now.strftime("%Y%m%d")
                    log_filename = "log_{}_{}.csv".format(device_name, runNo)

                    if (os.path.isfile(log_filename) == True):    
                        log_data = open(log_filename, "a")
                    else:
                        log_data = open(log_filename, "w")
                        log_data.write("Date,\
                                        Time,\
                                        Day,\
                                        Thingy,\
                                        Temperature(celcius),\
                                        Humidity(percent),\
                                        CO2(ppm),\
                                        TVOC(ppb),\
                                        Pressure(hPa),\
                                        Battery(percent)\n")
                        log_data.flush()

                    print('{},{},{},{},{},{},{},{},{}'.format(
                        d.strftime('%Y-%m-%d'),
                        d.strftime('%H:%M:%S'),
                        d.strftime('%d'),
                        device_name,
                        temp_val,
                        hum_val,
                        co2_val,
                        tvoc_val,
                        pressure_val),
                        file = log_data)
                    log_data.flush()
                    log_data.close()

                    iso = time.asctime(time.gmtime()) # UTC
                    battery_val = thingy.battery.read()
                    print("{} Battery: {}".format(session, battery_val))
                    json_body = [
                        {
                            "measurement": session,
                            "tags": {
                                "run": runNo,
                            },
                            "time": iso,
                            #"time": str(datetime.now())[:-3],
                            "fields": {
                                "temperature" : temp_val, 
                                "humidity" : hum_val, 
                                "co2" : co2_val, 
                                "tvoc" : tvoc_val,
                                "pressure" : pressure_val,
                                "battery" : battery_val
                                # ,"gpio" : gpio
                            }
                        }
                    ]
                    try:
                        client.write_points(json_body)
                    except:
                        print("# cannot connect to InfluxDB - {}".format(session))
                        pass
                        # TODO: disconnect thingy, in which scope?





                    



    