import matplotlib.pyplot as plt
import numpy as np

""" # send all the data
delay_between_sends = np.array([10, 15, 18, 20, 35, 45])
transmision_time = np.array([89.4, 132.9, 159.1, 176.6, 307.7, 398.1])
miss_rate = np.array([56.1, 34, 20.8, 11.9, 0.3, 0])
 """

# send half of the data
delay_between_sends = np.array([15, 20, 25, 30])
transmision_time = np.array([67.04, 88.86, 110.83, 132.48])
miss_rate = np.array([35.22, 12.9, 1.49, 1.01])


fig, ax1 = plt.subplots()
plt.title('Transmision Time vs. Miss Rate')

ax1.set_xlabel('delay between sends (ms)')
ax1.set_ylabel('transmision time (s)', color='tab:red')
ax1.plot(delay_between_sends, transmision_time, color='tab:red')
ax1.tick_params(axis='y', labelcolor='tab:red')

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
ax2.set_ylabel('miss rate (%)', color='tab:blue')  # we already handled the x-label with ax1
ax2.scatter(delay_between_sends, miss_rate, color='tab:blue')
ax2.tick_params(axis='y', labelcolor='tab:blue')

fig.tight_layout()  # otherwise the right y-label is slightly clipped
plt.show()