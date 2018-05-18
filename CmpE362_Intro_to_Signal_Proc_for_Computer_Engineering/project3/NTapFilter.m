clear all 
clc

%% Pre-processing Part, Adding delay to original mike

[mike, Fs] = audioread('mike.wav');  % Read audio Mike.wav

dt = 1/Fs; % Period
t = 0:dt:(length(mike)*dt)-dt;	%time is determined

delay = 0.1; % 100 miliseconds delay constant
echo = 1; % echo constant

D = delay*Fs;
mikeDelayed = zeros(size(mike));

mikeDelayed(1:D) = mike(1:D); % Equals its first variables (start varaible)

% Add 0.1 delay to original mike.wav
for i = D + 1: length(mike)
    mikeDelayed(i) = mike(i) + echo*mike(i-D);
end

%sound(mikeDelayed, Fs);

%% Part 1)Use constant N and K, change ? from 0 to 1 and plot SNR of mike.wav and recovered signal.

% K = 0.1 seconds(100 miliseconds) & N = 5
% y[n] = x[n] - a*x[n-1] + a^2*x[n-2] - a^3*x[n-3] + a^4*x[n-4] - a^5*x[n-5]

alpha = 0:0.1:1; % Differantiate between 0 to 1 with ascending 0.1
SNRAlpha = zeros(1,11); % Create matrix for 11 values

for i = alpha % changing alpha
    
    bk = [1 (-i) i^2 (-i)^3 i^4 (-i)^5]; % coresponding bk values
    alphaMike = conv(bk, mikeDelayed); % Conv function to filter the data
    SNRAlpha(int32(i*10)+1) = SNRCalc(mike, alphaMike); % Call SNRCalc to calculate SNR values and store
    
end

% Plot the figure
figure('Name', 'alpha change');
plot(0:0.1:1, SNRAlpha, '-*');
xlabel('alpha-value');
ylabel('SNR-value');
%sound(alphaMike, Fs);

%% Part 2) Use constant ? and K, change N from 1 to 50 and plot SNR of mike.wav and recovered signal.

% K = 0.1 seconds(100 miliseconds) & ? = 0.7
% y[n] = x[n] - a*x[n-1] + a^2*x[n-2] - a^3*x[n-3] + ... + a^50x[n-50]

SNRNTap = zeros(1,51); % Create matrix for 50 values
alpha = 0.7;

bk = zeros(1,51); % 51 Values 
bk(1) = 1;

SNRNTap(1) = SNRCalc(mike, mikeDelayed);
for N = 1:50
    
    bk(N+1) = (-alpha)^N; % coresponding bk values
    ntapMike = conv(bk, mikeDelayed); % Conv function to filter the data
    SNRNTap(int32(N)+1) = SNRCalc(mike, ntapMike); % Call SNRCalc to calculate SNR values and store
    
end

% Plot the figure
figure('Name', 'N change');
plot(1:51, SNRNTap, '-*');
xlabel('N-value');
ylabel('SNR-value');
%sound(ntapMike, Fs);

%% Part 3) Use constant ? and N, change K between 100,200,300,400 miliseconds and plot SNR of mike.wav and recovered signal.

% ? = 0.7 & N = 5
% y[n] = x[n] - a*x[n-1] + a^2*x[n-2] - a^3*x[n-3] + a^4*x[n-4] - a^5*x[n-5]

bk = [1 (-alpha) alpha^2 (-alpha)^3 alpha^4 (-alpha)^5]; % coresponding bk values
SNRDelay = zeros(1,4); % Create matrix for 11 values

for i = 1:4
    delay = i; % 100 miliseconds delay constant
    D = delay*Fs*0.1;
    mikeDelayed = zeros(size(mike));

    mikeDelayed(1:D) = mike(1:D); % Equals its first variables (start varaible)

    % Add K delay to original mike.wav
    for j = D + 1: length(mike)
        mikeDelayed(j) = mike(j) + echo*mike(j-D);
    end
    
    kMike = conv(bk, mikeDelayed); % Conv function to filter the data
    SNRDelay(int32(i)) = SNRCalc(mike, kMike); % Call SNRCalc to calculate SNR values and store
    
end

% Plot the figure
figure('Name', 'K change');
plot(100:100:400, SNRDelay, '-*');
xlabel('K-value(miliseconds)');
ylabel('SNR-value');
%sound(kMike, Fs);

%% SNR CALCULATION FUNCTION

function SNR = SNRCalc(mike, recovered)
    
    sum1 = 0; % First sum
    sum2 = 0; % second sum
    for i=1:length(mike) % Sum symbol where starts from 1 to length of the vector sound
        sum1 = mike(i)^2 + sum1; % I value
        sum2 = (recovered(i)-mike(i))^2 + sum2; % E - I value
    end
    
    SNR=10*log(sum1/sum2); % last calculation
end