#!/bin/bash

echo powersave | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
sleep 10
while true; do
	cd /home/pi/beehive/
	sudo python gateway_polling.py 
	sleep 5
	killall python
	sleep 5
done
