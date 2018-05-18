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
    answer = input('Directory is not Exist! Type again = ', 's');
    for k=1:length(files)
       if strcmp(answer, files(k).name)
          isExist = true; 
       end
    end
end

cd (reply); % go to subfolder

files=dir('*.csv'); % look for csv files
for k=1:length(files)
   M = csvread(files(k).name,1,0); %read files
   figure
   findpeaks(M ,'MinPeakProminence', 2); %plot the numbers with finding peaks
   xlabel('Number"s Position on CSV file'); ylabel('Value');
end


cd .. %go back to first directory again


