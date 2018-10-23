convert a raw file to a wav file:
sox -t raw -b 16 -e signed-integer -r 22050 -c 2 save.raw output.wav