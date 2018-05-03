[y,fs] = audioread('SAVE.WAV');
len1 = length(y) - 1;
% t = (0:1:len1)/fs;
y = y(:,1);
figure(1);
subplot(2,1,1);
plot(t,y);

yy = fft(y);
yy = fftshift(yy);
len2 = length(yy) - 1;
subplot(2,1,2);
plot(-pi:2*pi/len2:pi,abs(yy));

h0 = [1:-2*cos(0),1];
h1 = [1:-2*cos(0.04912),1];
h2 = [1:-2*cos(0.1473),1];

y0 = conv(h0,y);
y1 = conv(h1,y0);
y2 = conv(h2,y1);

% Tfigure(2);
% plot(t,y1);

sound(y2,fs);