#! /usr/bin/python
'''
Created on May 20, 2018
Description: basic processing of the audio file

ffmpeg -i input.mp3 output.wav
'''
import matplotlib.pyplot as plt
import numpy as np
import wave
import sys
import time
import csv
from scipy.fftpack import fft, ifft, fftshift
from scipy.signal import argrelextrema, find_peaks_cwt
from math import pi, log, floor, ceil

import warnings
warnings.simplefilter("once", DeprecationWarning)

csvfile =  open('freq.csv', 'wb')

class calc():
    def __init__(self):
        pass

    def nextpow2(self, x):
        if x == 0:
            return 1
        else:
            return 2 ** (x - 1).bit_length()

    def getMaxRange(self, arr, f, interval):
        f_sort = f[arr.argsort()[::-1]]
        ff = np.array([])
        for e in f_sort[0:50]:
            ff = np.append(ff, int(e / interval) * interval)
        _, idx = np.unique(ff, return_index = True)
        return ff[np.sort(idx)]

    def getLocalMax(self, arr, f, interval):
        pass

    def writeToFile(self, arr):
        fw = csv.writer(csvfile, delimiter = ',', quotechar = '|', quoting = csv.QUOTE_MINIMAL)
        fw.writerow([time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime()), str(arr)])
        time.sleep(1)

class AudioProcessing():
    def __init__(self, file):
        self.wf = wave.open(file, 'rb')
        self.data = self.wf.readframes(-1)
        self.data = np.frombuffer(self.data, 'Int16')
        self.data = self.data / (2.**15)    # normalize amplitude
        self.calc_ins = calc()
        self.fs = self.wf.getframerate()

    def time_spec(self):
        self.len1 = len(self.data)
        self.len1 = self.calc_ins.nextpow2(self.len1)
        # self.t = np.arange(0, self.len1)
        # self.t = self.t / self.wf.getframerate()

    def freq_spec(self):
        self.y = fft(self.data, self.len1)
        self.y = abs(self.y)
        self.y = self.y / float(self.len1)
        self.y = self.y ** 2
        self.f = np.arange(0, len(self.y)) * self.fs / self.len1
        # get envolope of the frequency waveform
        self.yy = self.y[1:int(floor(self.len1/2))]
        self.yy = self.yy[argrelextrema(self.yy, np.greater)[0]]
        indexes = argrelextrema(
            np.array(self.yy),
            comparator = np.greater,order = 100 # FIXME: depend on the degree of dispersion
            )
        self.yy =  self.yy[indexes]
        self.len_env = len(self.yy)
        self.f_env = np.arange(0, self.len_env) * self.fs / self.len_env
        yy = self.calc_ins.getMaxRange(self.yy, self.f_env, 50)
        print('Feature Frequencies:')
        print yy[0:10]
        self.calc_ins.writeToFile(yy[0:10])

    def plot_spec(self):
        plt.figure(1)
        plt.title('Waveform in Time Domain')
        plt.plot(self.data)
        plt.xlabel('Time(s) / 22050')
        plt.ylabel('Amplitude')

        plt.figure(2)
        plt.title('Waveform in Freq Domain')
        plt.plot(self.f[1:int(floor(self.len1/2))], self.y[1:int(floor(self.len1/2))])
        plt.xlabel('Frequency(Hz)')
        plt.ylabel('Amplitude')

        plt.figure(3)
        plt.title('Envolope of Frequency')
        plt.plot(self.f_env[1:int(floor(self.len_env/5))], self.yy[1:int(floor(self.len_env/5))])
        plt.xlabel('Frequency(Hz)')
        plt.ylabel('Amplitude')

        plt.figure(4)
        plt.title('Power of Frequency')
        plt.plot(self.f[1:int(floor(self.len1/2))], 10 * np.log10(self.y[1:int(floor(self.len1/2))]))
        plt.xlabel('Frequency(Hz)')
        plt.ylabel('Power(dB)')

        plt.show()

    def main(self):
        self.time_spec()
        self.freq_spec()
        self.plot_spec()

if __name__ == '__main__':
    try:
        start_time = time.time()
        f1 = AudioProcessing("../bee_sounds/bee_swarm_small.wav")
        f1.main()
        end_time = time.time()
        print("Execution Time: ", end_time - start_time)

        start_time = time.time()
        f2 = AudioProcessing("../bee_sounds/bee_inside_box.wav")
        f2.main()
        end_time = time.time()
        print("Execution Time: ", end_time - start_time)

        #
        # f3 = AudioProcessing("../bee_sounds/bee_buzzing_single.wav")
        # f3.main()
    except KeyboardInterrupt:
        print('Terminated')
