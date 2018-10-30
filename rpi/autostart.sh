# !/bin/bash

sleep 10
while true; do
    sudo python gateway.py
    sleep 10m
    killall python
    sleep 5

    # convert a raw file to a wav file:
    sudo chmod -R 777 input.raw
    sox -t raw -b 16 -e signed-integer -r 22050 -c 2 input.raw output.wav

    python spectrogram.py
    sleep 5
    killall python
    sleep 5
done