Program is written in python2.7 and you can simply run it by typing:
>> python info_retrival.py <path> <file-name>

It returns the lexRank scores of sentences on the document.
Output(The numbers are not real, they are just provided as an example):
0.67 0.45 0.21 0.66 0.89 0.13 0.71

to be able to run function calculateRogueScores() rouge must be setup. Run the following scripts from terminal:
>> git clone https://github.com/pltrdy/rouge
>> cd rouge
>> sudo python3 setup.py install

I couldn't run rouge library from .py files. pyhton2.7 not accepts rouge library.
So i use "subprocess.Popen(...)" to access shell and run it on shell to get results of scores as string and turn it in into jsonObjects.
The program then create a directory seperated and write results on resultFile. 
