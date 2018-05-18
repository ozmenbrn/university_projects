clear 
%% First Part including reading data
%Take input from the user
answer = input('Enter the file name = ', 's');

isExist = false;  %boolean function for determine if the file is exist or not

files=dir('*.*');  %Take all the files in the directory

% Look all the files, if file exist, turn the boolen funtion true
for k=1:length(files)
    if strcmp(answer, files(k).name)
       isExist = true; 
    end
end

% If not exist, ask again until find it
while ~isExist
    answer = input('Directory is not Exist!(type "folder" to access data) Type again = ', 's');
    for k=1:length(files)
       if strcmp(answer, files(k).name)
          isExist = true; 
       end
    end
end

cd (answer); % go to subfolder


files=dir('*.csv'); % look for csv files
for k=1:length(files)
    M = csvread(files(k).name,1,0); %read files
    figure('Name', 'Low-Pass Filter', 'units', 'normalized', 'outerposition', [0 0 1 1]); % to maximaze the window to see clear    
    Num = numel(findpeaks(M ,'MinPeakProminence', 1.9)); %plot the numbers with finding peaks
    subplot(2, 3,1),findpeaks(M ,'MinPeakProminence', 1.9);
    xlabel('Numbers Positions on CSV file'); ylabel('Value');
    string = strcat('No Filter ,# of Peaks=', num2str(Num)); % combine strings
    title(string);
    fprintf('Applying Low-Pass Filter\n');
    fprintf('No-filter, # of Peaks= %i\n', Num);
    %% First Part
    temp = 1;
    string = strcat('Limit Frequency = (', num2str(i*1000), ')'); % combine strings
    for i=1:4 
        size = length(M) - mod(length(M), i*1000); % Sample Size
        cut_off = size/(i*1000); % Determine the cutoff frequency
        B = 1/cut_off*ones(cut_off,1); % Filter Formula
        lowPassFilter = filter(B,1,M); % filter function
        Num = numel(findpeaks(lowPassFilter ,'MinPeakProminence', 1.9)); %number of peaks
        subplot(2,3,i+1), findpeaks(lowPassFilter ,'MinPeakProminence', 1.9); % plot
        xlabel('Numbers Positions on CSV file'); ylabel('Value');
        string = strcat('LimitFreq=', num2str(i), 'kHz, # of Peaks=', num2str(Num)); % combine strings
        title(string);
        disp(string);
        temp = temp + 1; % keep the value between 1-10
    end

       
    %% Second Part
    temp = 1;
    fprintf('\n\nApplying Moving Averager Filter\n');
    for i=1:3 % To be able to fit 30 figures, it divides into 3 to subplot 10 plots.
        string = strcat('Moving Avarage Filter with N = (', num2str(i*10-9), '_to_', num2str(i*10), ')'); % combine strings
        figure('Name', string, 'units', 'normalized', 'outerposition', [0 0 1 1]); % to maximaze the window to see clear
        for k=i*10-9:i*10 % 10 loops
            B = 1/k*ones(k,1); %average filter formula
            averageFilter = filter(B,1,M); % filter function into average filter
            Num = numel(findpeaks(averageFilter ,'MinPeakProminence', 1.9)); %number of peaks
            subplot(2,5,temp), findpeaks(averageFilter ,'MinPeakProminence', 1.9); % plot
            string = strcat('N=', num2str(k), ' averager, # of Peaks=', num2str(Num)); % combine strings
            title(string);
            disp(string);
            temp = temp + 1; % keep the value between 1-10
        end
        temp = 1;
    end
   
end

cd .. %go back to first directory again


    



