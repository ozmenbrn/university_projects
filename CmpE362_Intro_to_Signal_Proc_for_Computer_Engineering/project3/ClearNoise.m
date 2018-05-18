clear all 
clc

[y1, Fs1] = audioread('mike.wav');  % Read audio Mike.wav
[y2, Fs2] = audioread('street.wav');   % Read auido Street.Wav

audiowrite('mike+street.wav', y1+y2, Fs1);  % Combine 2 wav files into 1. Fs1 = Fs2 so pick one of them is enough

[y, Fs] = audioread('mike+street.wav'); % Read combined auido file Mike+Street.wav

% Because of all 3 got same length of time (9seconds) 1 calculation is enough for both of them

%% Part 1) Frequency Domain Representation of Mike.wav, Street.wav, Mike+Street.wav

N = length(y); % Length of the audio
f=Fs*(0:N/2)/N; % Frequency vector

figure('Name', 'Frequency Domain Representations (Unfiltered)', 'units', 'normalized', 'outerposition', [0 0 1 1]); % to maximaze the window to see clear

datafft=fft(y1); datafft_abs=abs(datafft/N); datafft_abs=datafft_abs(1:N/2+1); % fft function for frequency domain
subplot(1,3,1); plot(f, datafft_abs); title('Mike Sound'); xlabel('Frequency(Hz)'); ylabel('Magnitude(x10^3 db)');
xlim([-500 12000]); ylim([0 0.021]);

datafft=fft(y2); datafft_abs=abs(datafft/N); datafft_abs=datafft_abs(1:N/2+1); % fft function for frequency domain
subplot(1,3,2); plot(f, datafft_abs); title('Street Sound'); xlabel('Frequency(Hz)'); ylabel('Magnitude(x10^3 db)');
xlim([-500 12000]); ylim([0 0.021]);

datafft=fft(y); datafft_abs=abs(datafft/N); datafft_abs=datafft_abs(1:N/2+1); % fft function for frequency domain
subplot(1,3,3); plot(f, datafft_abs); title('Combined Sound'); xlabel('Frequency(Hz)'); ylabel('Magnitude(x10^3 db)');
xlim([-500 12000]); ylim([0 0.021]);

%% Part 2) Time Domain Representation of Mike.wav, Street.wav, Mike+Street.wav

timeY = y(:,1); % Vector as colomn
dt = 1/Fs; % Period
t = 0:dt:(length(timeY)*dt)-dt;	%time is determined

figure('Name', 'Time Domain Representations (Unfiltered)', 'units', 'normalized', 'outerposition', [0 0 1 1]); % to maximaze the window to see clear
subplot(1,3,1), plot(t,y1); title('Mike Sound'); xlabel('Seconds'); ylabel('Amplitude'); ylim([-0.21 0.3]) % Mike's sound
subplot(1,3,2), plot(t,y2); title('Street Sound'); xlabel('Seconds'); ylabel('Amplitude'); ylim([-0.21 0.3]) % Street's sound
subplot(1,3,3), plot(t,y); title('Combined Sound'); xlabel('Seconds'); ylabel('Amplitude'); ylim([-0.21 0.3]) % Combined sound


%% Part 3) Frequency Domain Representation of Mike.wav, Filtered Mike+Street.wav

%Averging for reducing intensity of high frequencies
for j=2:length(y)-1
    y(j,1) = (y(j-1,1) + y(j,1) + y(j+1,1))/3 ;
end
g = gausswin(20); %Creataing Gaussian window of 20
g = g/sum(g); 
mm= conv(y(:,1), g, 'same'); %Applying Conv to remove effect of noise
filteredSound=sgolayfilt(mm,1,17); % Apply signal smoothing using Savitzky-Golay smoothing filter.

figure('Name', 'Frequency Domain Representations (Filtered)', 'units', 'normalized', 'outerposition', [0 0 1 1]); % to maximaze the window to see clear

datafft=fft(y1); datafft_abs=abs(datafft/N); datafft_abs=datafft_abs(1:N/2+1); % fft function for frequency domain
subplot(1,2,1); plot(f, datafft_abs); title('Mike Sound'); xlabel('Frequency(Hz)'); ylabel('Magnitude(x10^3 db)');
xlim([-500 12000]); ylim([0 0.021]);

datafft=fft(filteredSound); datafft_abs=abs(datafft/N); datafft_abs=datafft_abs(1:N/2+1);% fft function for frequency domain 
subplot(1,2,2); plot(f, datafft_abs); title('Combined Sound'); xlabel('Frequency(Hz)'); ylabel('Magnitude(x10^3 db)');
xlim([-500 12000]); ylim([0 0.021]);

%% Part 4) Time Domain Representation of Mike.wav, Filtered Mike+Street.wav

figure('Name', 'Time Domain Representations (Filtered)', 'units', 'normalized', 'outerposition', [0 0 1 1]); % to maximaze the window to see clear
subplot(1,2,1), plot(t,y1); title('Mike Sound'); xlabel('Seconds'); ylabel('Amplitude'); ylim([-0.21 0.3]) % Mike's sound

subplot(1,2,2), plot(t,filteredSound); title('Combined Sound'); xlabel('Seconds'); ylabel('Amplitude'); ylim([-0.21 0.3]) % Mike's sound

sound(filteredSound,Fs);

%% Last Part) Calculate SNR value

sum1 = 0; % First sum
sum2 = 0; % second sum
for i=1:length(y) % Sum symbol where starts from 1 to length of the vector sound
    sum1 = y1(i)^2 + sum1; % I value
    sum2 = (filteredSound(i)-y1(i))^2 + sum2; % E - I value
end
SNR=10*log(sum1/sum2); % last calculation

disp('SNR Value = '); 
disp(SNR); % SNR value

