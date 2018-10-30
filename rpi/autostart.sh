#!/bin/bash

sleep 5
while true; do
    sudo python gateway.py
    sleep 5
    killall python

    # convert a raw file to a wav file:
    sudo chmod -R 777 input.raw
    echo "Converting raw file to wav..."
    sox -t raw -b 16 -e signed-integer -r 22050 -c 2 input.raw output.wav

    python spectrogram.py
    sleep 5
    killall python
    echo "Sleeping 10 minutes..."
    sleep 2m
done