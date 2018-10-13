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

host = "192.168.1.26"
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
    flag = 0
    if (devices!=[]):
#==============================================================================
        for dev in devices:
            #print("Device {} ({}), RSSI={} dB".format(dev.addr, dev.addrType, dev.rssi))
            for (adtype, desc, value) in dev.getScanData():
                #print("  {}, {} = {}".format(adtype, desc, value))
                #if (value == "Thingy"):
                index = 0
                while index < len(device_list):
                    if device_list[index][2] == dev.addr:  
                        flag = 1
                        break
                    index += 1
                else: # when none of the previous devices are matched
                    if "Thingy" in value:
                        flag = 1
                        print("# Thingy ({}) found with address: {}".format(value, dev.addr))
                        device_counter += 1
                        new_device = [str(device_counter),
                            DEVICE_NAME+str(device_counter).zfill(2),
                            dev.addr
                            ]
                        print("# The new device is named {}".format(new_device[1]))
                        device_list.append(new_device)
                        # Writing the new device info to table file
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
        
        if flag == 0:
            print("Thingy not found, please try again.")
            sys.exit() 
#==============================================================================        
        # Gathering information from each available device
        for dev in devices:
            index = 0
            while index < len(device_list):
                if device_list[index][2] == dev.addr:
                    temp_val = 0
                    hum_val = 0
                    co2_val = 0
                    tvoc_val = 0
                    pressure_val = 0
                    battery_val = 0
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
                    runNo = d.strftime("%Y%m%d")
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
                    print("{} Battery: {}".format(device_name, battery_val))
                    json_body = [
                        {
                            "measurement": device_name,
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
                        print("Connecting to {}....\n".format(host))
                        print()
                        client.write_points(json_body)
                    except:
                        print("# cannot connect to InfluxDB - {}".format(device_name))
                    
                    print("# Disconnecting...")
                    thingy.disconnect()
                    del thingy
                index += 1
    
        print()

# Create the InfluxDB object
client = InfluxDBClient(host, port, user, password, dbname, timeout = 3)

# ==========================================================================
print("# Creating new delegate class to handle notifications...")
class SampleDelegate(btle.DefaultDelegate):
    def handleNotification(self, hnd, data):
        if (hnd == thingy52.e_temperature_handle):
            print("Notification: Temperature received: {}".format(repr(data)))
        if (hnd == thingy52.ui_button_handle):
            print("Notification: Button press received: {}".format(repr(data)))

class MyDelegate(btle.DefaultDelegate):
    
    def handleNotification(self, hnd, data):
        global temp_val, hum_val, co2_val, tvoc_val, pressure_val
        #Debug print repr(data)
        if (hnd == thingy52.e_temperature_handle):
            teptep = binascii.b2a_hex(data)
	    temp_val = float("{}.{}".format(self._str_to_int(teptep[:-2]), int(teptep[-2:], 16)))
            #print('Temp:  {} degCelcius'.format(temp_val))
            #print('{},{}'.format(str(datetime.now()), temp_val), file = log_temp)
            
        elif (hnd == thingy52.e_pressure_handle):
            pressure_int, pressure_dec = self._extract_pressure_data(data)
            #print('Notification: Press received: {}.{} hPa'.format(
            #            pressure_int, pressure_dec))
	    pressure_val = float("{}.{}".format(pressure_int, pressure_dec))
            #print('{}.{}'.format(str(datetime.now()), pressure_val), file = log_pressure)

        elif (hnd == thingy52.e_humidity_handle):
            teptep = binascii.b2a_hex(data)
	    hum_val = int("{}".format(self._str_to_int(teptep)))
            #print('Humidity: {} %'.format(hum_val))
            #print('{},{}'.format(str(datetime.now()), hum_val), file = log_hum)

        elif (hnd == thingy52.e_gas_handle):
            eco2, tvoc = self._extract_gas_data(data)
	    co2_val = eco2
            tvoc_val = tvoc
            #print('Gas: eCO2 ppm: {}, TVOC ppb: {} %'.format(eco2, tvoc))
            #print('{},{}'.format(str(datetime.now()), co2_val), file = log_co2)
            #print('{},{}'.format(str(datetime.now()), tvoc_val), file = log_tvoc)

        elif (hnd == thingy52.e_color_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Color: {}'.format(teptep))            

        elif (hnd == thingy52.ui_button_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Button state [1 -> released]: {}'.format(self._str_to_int(teptep)))

        elif (hnd == thingy52.m_tap_handle):
            direction, count = self._extract_tap_data(data)
            print('Notification: Tap: direction: {}, count: {}'.format(direction, self._str_to_int(count)))

        elif (hnd == thingy52.m_orient_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Orient: {}'.format(teptep))

        elif (hnd == thingy52.m_quaternion_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Quaternion: {}'.format(teptep))

        elif (hnd == thingy52.m_stepcnt_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Step Count: {}'.format(teptep))

        elif (hnd == thingy52.m_rawdata_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Raw data: {}'.format(teptep))

        elif (hnd == thingy52.m_euler_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Euler: {}'.format(teptep))

        elif (hnd == thingy52.m_rotation_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Rotation matrix: {}'.format(teptep))

        elif (hnd == thingy52.m_heading_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Heading: {}'.format(teptep))

        elif (hnd == thingy52.m_gravity_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Gravity: {}'.format(teptep))        

        elif (hnd == thingy52.s_speaker_status_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Speaker Status: {}'.format(teptep))

        elif (hnd == thingy52.s_microphone_handle):
            teptep = binascii.b2a_hex(data)
            print('Notification: Microphone: {}'.format(teptep))

        else:
            teptep = thingy52.binascii.b2a_hex(data)
            print('Notification: UNKOWN: hnd {}, data {}'.format(hnd, teptep))
            

    def _str_to_int(self, s):
        """ Transform hex str into int. """
        i = int(s, 16)
        if i >= 2**7:
            i -= 2**8
        return i    

    def _extract_pressure_data(self, data):
        """ Extract pressure data from data string. """
        teptep = binascii.b2a_hex(data)
        pressure_int = 0
        for i in range(0, 4):
                pressure_int += (int(teptep[i*2:(i*2)+2], 16) << 8*i)
        pressure_dec = int(teptep[-2:], 16)
        return (pressure_int, pressure_dec)

    def _extract_gas_data(self, data):
        """ Extract gas data from data string. """
        teptep = binascii.b2a_hex(data)
        eco2 = int(teptep[:2], 16) + (int(teptep[2:4], 16) << 8)
        tvoc = int(teptep[4:6], 16) + (int(teptep[6:8], 16) << 8)
        return eco2, tvoc

    def _extract_tap_data(self, data):
        """ Extract tap data from data string. """
        teptep = binascii.b2a_hex(data)
        direction = teptep[0:2]
        count = teptep[2:4]
        return (direction, count)

# ==========================================================================
# Creating the device list file if it does not exist
if (os.path.isfile(DEVICE_LIST_FILE_NAME) == False):
    device_data = open(DEVICE_LIST_FILE_NAME, "w")
    device_data.write("Number,Device Name,MAC Address")
    device_data.write("\n")
    device_data.flush()    
    device_data.close()

device_data = open(DEVICE_LIST_FILE_NAME, "r")

first_line = 1
device_list = []
device_counter = 0
for line in device_data:
    # Split strings which are seperated with ','
    line_data = line.split(',')
    # Remove '\n' at the end of each line
    line_data[-1] = line_data[-1][:len(line_data[-1])-1]

    # To prevent putting the header to the list
    if (first_line == 1):
        first_line = 0
    else:
        # Check if the line is valid (and not empty) before adding it 
        # to the device list
        if (line_data[0] == str(device_counter+1)):
            device_list.append(line_data)
            device_counter+=1

print("# Searching for new device...")
print()

thingyPoller()
