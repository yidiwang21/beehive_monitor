#! /usr/bin/python
'''
Created on May 20, 2018
Description: plot basic spectrums of the audio file

ffmpeg -i input.mp3 output.wav
'''
import matplotlib.pyplot as plt
import numpy as np
import wave
import sys
from scipy.fftpack import fft, ifft, fftshift
from scipy.signal import argrelextrema, find_peaks_cwt, general_gaussian, fftconvolve
import scipy.signal
from math import pi, log, floor, ceil

import warnings
warnings.simplefilter("once", DeprecationWarning)

class calc():
    def __init__(self):
        pass

    def nextpow2(self, x):
        if x == 0:
            return 1
        else:
            return 2 ** (x - 1).bit_length()

    def getMaxRange(self, arr, interval, fs):
        y = np.array([])
        num = int(ceil(len(arr) / interval))
        idxs = np.arange(1, num)
        for i in range(1, num):
            y = np.append(y, np.average(arr[(i-1) * interval : i * interval]))
        y = np.vstack((y, idxs))
        y = np.transpose(y)
        y = y[y[:,0].argsort()]
        y[:,1] = y[:,1] * interval * fs / len(y)
        # # TODO: transfer second col of y to corresponding freq

        return y[0:3,:]

class AudioSpec():
    def __init__(self, file):
        self.wf = wave.open(file, 'rb')
        self.data = self.wf.readframes(-1)
        self.data = np.frombuffer(self.data, 'Int16')
        self.data = self.data / (2.**15)
        self.calc_ins = calc()
        self.fs = self.wf.getframerate()

    def time_spec(self):
        self.len1 = len(self.data)
        self.len1 = self.calc_ins.nextpow2(self.len1)
#         self.t = np.arange(0, self.len1)
#         self.t = self.t / self.wf.getframerate()

    def freq_spec(self):
        self.y = fft(self.data, self.len1)
        self.y = abs(self.y)
        # self.y = self.y / float(self.len1)
        self.y = self.y ** 2
#         self.y = fftshift(self.y)
        len2 = len(self.y)
        self.f = np.arange(0, len2) * self.fs / self.len1
        # get envolope of the frequency waveform
        self.yy = self.y[argrelextrema(self.y, np.greater)[0]]
        indexes = scipy.signal.argrelextrema(
            np.array(self.yy),
            comparator = np.greater,order = 500
            )
        print('Peaks are: %s' % (indexes[0]))
        self.yy =  self.yy[indexes]
        print len(self.yy)
        self.len_env = len(self.yy)
        self.f_env = np.arange(0, self.len_env) * self.fs / self.len_env
        yy = self.calc_ins.getMaxRange(self.yy, len(self.yy)/20, self.fs)
        print yy


    def plot_spec(self):
        plt.figure(1)
        plt.title('Waveform in Time Domain')
        plt.plot(self.data)
        plt.xlabel('Time(s) / 22050')
        plt.ylabel('Amplitude')

        plt.figure(2)
        plt.title('Waveform in Freq Domain')
        plt.plot(self.f[1:int(floor(self.len1/2))], self.y[1:int(floor(self.len1/2))])
#         plt.plot(abs(self.y))
        plt.xlabel('Frequency(Hz)')
        plt.ylabel('Amplitude')

        plt.figure(3)
        plt.title('Envolope of Frequency')
        plt.plot(self.f_env[1:int(floor(self.len_env/5))], self.yy[1:int(floor(self.len_env/5))])
        plt.xlabel('Frequency(Hz)')
        plt.ylabel('Amplitude')

        plt.figure(4)
        plt.title('Waveform in Freq Domain')
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
        f = AudioSpec("../bee_sounds/bee_swarm_small.wav")
        f.main()
    except KeyboardInterrupt:
        print('Terminated')
