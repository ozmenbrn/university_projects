clear all 
clc

[clap, Fs] = audioread('clap.wav');  % Read audio Mike.wav
[snap, Fs1] = audioread('snap.wav');   % Read auido Street.Wav

clap = clap(:,1); % They must be vectors to be able to use spectrogtam
snap = snap(:,1); % They must be vectors to be able to use spectrogtam

meanFreqClap = meanfreq(clap,Fs); % Mean Frequency to determine a treshold
meanFreqSnap = meanfreq(snap,Fs); % Mean Frequency to determine a treshold
avarageFreq = (meanFreqClap + meanFreqSnap)/3;

inputFile = input('Enter the file name = ', 's'); % Take input from the user as string
[inputSound, Fs2] = audioread(inputFile); % read the audio file

inputSound = inputSound(:,1); % They must be vectors to be able to use spectrogtam

meanFreq = meanfreq(inputSound, Fs2); % Mean frequency of input sound

% Threshold is the avarage of these 2 trained wav files. snap lies between 3k- 6k where
% clap lies between 1k - 2k. My treshold is 2.6k is enough to be able to detect
if(meanFreq > avarageFreq)
    disp('Snap Detected!');
else
    disp('Clap Detected!');
end
   
% Spectrogram of it
figure('Name', 'Frequency Domain Representation') 
spectrogram(inputSound,kaiser(256,5),220,512,Fs);
