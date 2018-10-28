# convert a raw file to a wav file:
sudo chmod -R 777 input.raw
sox -t raw -b 16 -e signed-integer -r 22050 -c 2 input.raw output.wav
