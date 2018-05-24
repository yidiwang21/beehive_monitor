%% spectrum analysis
[y,fs] = audioread('../bee_sounds/bee_swarm_small.wav');
len1 = length(y) - 1;
t = (0:1:len1)/fs;
y = y(:,1);
figure(1);
subplot(2,1,1);
plot(t,y);
title('Original Signal in Time Domain');
xlabel('Time');
ylabel('Amplitude');

yy = fft(y);
yy = fftshift(yy);
len2 = length(yy) - 1;
subplot(2,1,2);
plot(-pi:2*pi/len2:pi,abs(yy));
title('Original Signal in Freq Domain');
xlabel('\omega/\pi');
ylabel('Amplitude');