%%High Frequency Time and Frequency domains
[highFreq,fs] = audioread('highFreq.wav'); %To ready file

highFreq = highFreq(:,1);
dt = 1/fs;
t = 0:dt:(length(highFreq)*dt)-dt;	%time is determined
figure('Name', 'TimeDomainHighFreq');
plot(t,highFreq); xlabel('Seconds'); ylabel('Amplitude'); 

figure('Name', 'FreqDomainHighFreq');
spectrogram(highFreq,kaiser(256,5),220,512,fs); %draw spectogram of it.


%%Low Frequency Time and Frequency domains
[lowFreq,fs] = audioread('lowFreq.wav');  %To ready file

lowFreq = lowFreq(:,1);
dt = 1/fs;
t = 0:dt:(length(lowFreq)*dt)-dt; %time is determined

figure('Name', 'TimeDomainLowFreq'); %create multiple figures
plot(t,lowFreq); xlabel('Seconds'); ylabel('Amplitude'); 

figure('Name', 'FreqDomainLowFreq') 
%findpeaks(lowFreq, fs, 'MinPeakHeight',0.5)
spectrogram(lowFreq,kaiser(256,5),220,512,fs);




