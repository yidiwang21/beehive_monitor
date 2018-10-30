import wave
import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

file1 = "../bee_sounds/bee_swarm_small.wav"
file2 = "../bee_sounds/bee_buzzing_single.wav"

file = "output.wav"

def get_spectrogram(file):
    wf = wave.open(file, 'rb')
    data = wf.readframes(-1)
    data = np.frombuffer(data, 'Int16')
    data = data[1:len(data)/15]
    fs = wf.getframerate()

    f, t, Sxx = signal.spectrogram(data, fs)
    return t, f, Sxx

t, f, Sxx = get_spectrogram(file)
plt.figure(1)
plt.pcolormesh(t, f, Sxx)
plt.ylabel('Frequency [Hz]')
plt.xlabel('Time [sec]')

# t1, f1, Sxx1 = get_spectrogram(file1)
# print("t: ", len(t1))
# print("f: ", len(f1))
# print("Sxx: ", len(Sxx1))
# plt.figure(1)
# plt.title("Swarming")
# plt.pcolormesh(t1, f1, Sxx1)
# # plt.plot(Sxx1, f1)
# plt.ylabel('Frequency [Hz]')
# plt.xlabel('Time [sec]')

# t2, f2, Sxx2 = get_spectrogram(file2)
# plt.figure(2)
# plt.title("Normal")
# plt.pcolormesh(t2, f2, Sxx2)
# plt.ylabel('Frequency [Hz]')
# plt.xlabel('Time [sec]')

plt.show()
