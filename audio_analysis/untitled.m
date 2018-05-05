%% input
[y,fs] = audioread('SAVE.WAV');
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

%% filter
h0 = [1,-2*cos(0),1];
h1 = [1,-2*cos(pi/2),1];
h2 = [1,-2*cos(pi),1];

y0 = conv(h0,y);
y1 = conv(h1,y0);
y2 = conv(h2,y1);

%% output
len_t = length(y2) - 1;
tt = (0:1:len_t)/fs;

figure(2);
subplot(2,1,1);
plot(tt,y2);
title('New Signal in Time Domain');
xlabel('Time');
ylabel('Amplitude');
yy2 = fft(y2);
yy2 = fftshift(yy2);
len_yy2 = length(yy2) - 1;
subplot(2,1,2);
plot(-pi:2*pi/len_t:pi,abs(yy2));
title('New Signal in Freq Domain');
xlabel('\omega/\pi');
ylabel('Amplitude');

% sound(y2,fs);