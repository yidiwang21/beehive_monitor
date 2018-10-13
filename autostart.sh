#!/bin/bash

echo powersave | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
sleep 10
while true; do
#	cd /repo/thingy_gateway
	sudo python thingy_gateway_updated.py 
	sleep 5
	killall python
	sleep 5
done
