import matplotlib.pyplot as plt
import numpy as np
import wave
import sys
import time
import csv
from scipy.fftpack import fft, ifft, fftshift
from scipy.signal import argrelextrema, find_peaks_cwt
from math import pi, log, floor, ceil

def nextpow2(x):
    if x == 0:
        return 1
    else:
        return 2 ** (x - 1).bit_length()

file = "sample/output.wav"

wf = wave.open(file, 'rb')
data = wf.readframes(-1)
data = np.frombuffer(data, 'Int16') # FIXME: 32 
data = data / (2.**15)  # 
fs = wf.getframerate()

len1 = len(data)
len1 = nextpow2(len1)
y = abs(fft(data, len1))
y = y / float(len1)
y = y ** 2
f = np.arange(0, len(y)) * fs / len1

plt.figure(1)
plt.title('Waveform in Freq Domain')
# plt.hist2d(y[1:int(floor(len1/2))], f[1:int(floor(len1/2))])
plt.hist2d(y[1:int(floor(len1/2))], f[1:int(floor(len1/2))])
cbar = plt.colorbar()
cbar.ax.set_ylabel('Counts')
# plt.plot(f[1:int(floor(len1/2))], y[1:int(floor(len1/2))])
plt.xlabel('Frequency(Hz)')
plt.ylabel('Amplitude')
plt.show()


