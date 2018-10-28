from __future__ import print_function

import Adafruit_BluefruitLE
from Adafruit_BluefruitLE.services import UART
import os, sys

import threading

AUDIO_FILE_NAME = "input.raw"
REPEAT_TIME = 120

ble = Adafruit_BluefruitLE.get_provider()
#============================================================================

def main():
#============================================================================
    # do it every "REPEAT_TIME" seconds
    threading.Timer(REPEAT_TIME, main).start()
    # Clear any cached data because both bluez and CoreBluetooth have issues with
    # caching data and it going stale.
    ble.clear_cached_data()
    adapter = ble.get_default_adapter()
    adapter.power_on()
    print('# Using adapter: {0}'.format(adapter.name))
    # Disconnect any current connected devices
    print("# Disconnecting any connected UART devices...")
    UART.disconnect_devices()
    # Scan for UART devices, return a list
    print('# Searching for UART device...')
    try:
        adapter.start_scan()
        device = UART.find_device()
        if device is None:
            raise RuntimeError('# Failed to find UART device!')
    finally:
        adapter.stop_scan()

    print("# Connecting to {} with MAC Address: {}...".format(device.name, device.id))
    device.connect()    # timeout = 60s

    try:
        print("# Discovering services...")
        UART.discover(device)
        # Once service discovery is complete create an instance of the service
        uart = UART(device)

        if os.path.isfile(AUDIO_FILE_NAME) == True:
            os.remove(AUDIO_FILE_NAME)
        f = open(AUDIO_FILE_NAME, "w")

        print('# Starting receiving data...')
        while True:
            data = uart.read(timeout_sec=1)
            if data is not None:
                f.write(data)
                # print('# Received: {0}'.format(data))
            else:
                print('# No data received, exiting...')
                break
    finally:
        f.close()
        device.disconnect()
#============================================================================

ble.initialize()
ble.run_mainloop_with(main)    
