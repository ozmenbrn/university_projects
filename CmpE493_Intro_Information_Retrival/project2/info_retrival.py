#!/usr/bin/python

import os, sys, glob, time, re
import ast, math, collections


testList = []
trainList = []

vocabCounter = 0
earnCount = 0
acqCount = 0
moneyCount = 0
grainCount = 0
crudeCount = 0

earnOccuranceMap = dict()
acqOccuranceMap = dict()
moneyOccuranceMap = dict()
grainOccuranceMap = dict()
crudeOccuranceMap = dict()

earnCompOccuranceMap = dict()
acqCompOccuranceMap = dict()
moneyCompOccuranceMap = dict()
grainCompOccuranceMap = dict()
crudeCompOccuranceMap = dict()

earnCmap = dict()
acqCmap = dict()
moneyCmap = dict()
grainCmap = dict()
crudeCmap = dict()

earnCompCmap = dict()
acqCompCmap = dict()
moneyCompCmap = dict()
grainCompCmap = dict()
crudeCompCmap = dict()

miEarnScoresMap = dict()
miAcqScoresMap = dict()
miMoneyScoresMap = dict()
miGrainScoresMap = dict()
miCrudeScoresMap = dict()

miEarnCompScoresMap = dict()
miAcqCompScoresMap = dict()
miMoneyCompScoresMap = dict()
miGrainCompScoresMap = dict()
miCrudeCompScoresMap = dict()

#Stemming algorithm copied from the porterstermmer
class PorterStemmer:

    def __init__(self):
        """The main part of the stemming algorithm starts here.
        b is a buffer holding a word to be stemmed. The letters are in b[k0],
        b[k0+1] ... ending at b[k]. In fact k0 = 0 in this demo program. k is
        readjusted downwards as the stemming progresses. Zero termination is
        not in fact used in the algorithm.

        Note that only lower case sequences are stemmed. Forcing to lower case
        should be done before stem(...) is called.
        """

        self.b = ""  # buffer for word to be stemmed
        self.k = 0
        self.k0 = 0
        self.j = 0   # j is a general offset into the string

    def cons(self, i):
        """cons(i) is TRUE <=> b[i] is a consonant."""
        if self.b[i] == 'a' or self.b[i] == 'e' or self.b[i] == 'i' or self.b[i] == 'o' or self.b[i] == 'u':
            return 0
        if self.b[i] == 'y':
            if i == self.k0:
                return 1
            else:
                return (not self.cons(i - 1))
        return 1

    def m(self):
        """m() measures the number of consonant sequences between k0 and j.
        if c is a consonant sequence and v a vowel sequence, and <..>
        indicates arbitrary presence,

           <c><v>       gives 0
           <c>vc<v>     gives 1
           <c>vcvc<v>   gives 2
           <c>vcvcvc<v> gives 3
           ....
        """
        n = 0
        i = self.k0
        while 1:
            if i > self.j:
                return n
            if not self.cons(i):
                break
            i = i + 1
        i = i + 1
        while 1:
            while 1:
                if i > self.j:
                    return n
                if self.cons(i):
                    break
                i = i + 1
            i = i + 1
            n = n + 1
            while 1:
                if i > self.j:
                    return n
                if not self.cons(i):
                    break
                i = i + 1
            i = i + 1

    def vowelinstem(self):
        """vowelinstem() is TRUE <=> k0,...j contains a vowel"""
        for i in range(self.k0, self.j + 1):
            if not self.cons(i):
                return 1
        return 0

    def doublec(self, j):
        """doublec(j) is TRUE <=> j,(j-1) contain a double consonant."""
        if j < (self.k0 + 1):
            return 0
        if (self.b[j] != self.b[j-1]):
            return 0
        return self.cons(j)

    def cvc(self, i):
        """cvc(i) is TRUE <=> i-2,i-1,i has the form consonant - vowel - consonant
        and also if the second c is not w,x or y. this is used when trying to
        restore an e at the end of a short  e.g.

           cav(e), lov(e), hop(e), crim(e), but
           snow, box, tray.
        """
        if i < (self.k0 + 2) or not self.cons(i) or self.cons(i-1) or not self.cons(i-2):
            return 0
        ch = self.b[i]
        if ch == 'w' or ch == 'x' or ch == 'y':
            return 0
        return 1

    def ends(self, s):
        """ends(s) is TRUE <=> k0,...k ends with the string s."""
        length = len(s)
        if s[length - 1] != self.b[self.k]: # tiny speed-up
            return 0
        if length > (self.k - self.k0 + 1):
            return 0
        if self.b[self.k-length+1:self.k+1] != s:
            return 0
        self.j = self.k - length
        return 1

    def setto(self, s):
        """setto(s) sets (j+1),...k to the characters in the string s, readjusting k."""
        length = len(s)
        self.b = self.b[:self.j+1] + s + self.b[self.j+length+1:]
        self.k = self.j + length

    def r(self, s):
        """r(s) is used further down."""
        if self.m() > 0:
            self.setto(s)

    def step1ab(self):
        """step1ab() gets rid of plurals and -ed or -ing. e.g.

           caresses  ->  caress
           ponies    ->  poni
           ties      ->  ti
           caress    ->  caress
           cats      ->  cat

           feed      ->  feed
           agreed    ->  agree
           disabled  ->  disable

           matting   ->  mat
           mating    ->  mate
           meeting   ->  meet
           milling   ->  mill
           messing   ->  mess

           meetings  ->  meet
        """
        if self.b[self.k] == 's':
            if self.ends("sses"):
                self.k = self.k - 2
            elif self.ends("ies"):
                self.setto("i")
            elif self.b[self.k - 1] != 's':
                self.k = self.k - 1
        if self.ends("eed"):
            if self.m() > 0:
                self.k = self.k - 1
        elif (self.ends("ed") or self.ends("ing")) and self.vowelinstem():
            self.k = self.j
            if self.ends("at"):   self.setto("ate")
            elif self.ends("bl"): self.setto("ble")
            elif self.ends("iz"): self.setto("ize")
            elif self.doublec(self.k):
                self.k = self.k - 1
                ch = self.b[self.k]
                if ch == 'l' or ch == 's' or ch == 'z':
                    self.k = self.k + 1
            elif (self.m() == 1 and self.cvc(self.k)):
                self.setto("e")

    def step1c(self):
        """step1c() turns terminal y to i when there is another vowel in the stem."""
        if (self.ends("y") and self.vowelinstem()):
            self.b = self.b[:self.k] + 'i' + self.b[self.k+1:]

    def step2(self):
        """step2() maps double suffices to single ones.
        so -ization ( = -ize plus -ation) maps to -ize etc. note that the
        string before the suffix must give m() > 0.
        """
        if self.b[self.k - 1] == 'a':
            if self.ends("ational"):   self.r("ate")
            elif self.ends("tional"):  self.r("tion")
        elif self.b[self.k - 1] == 'c':
            if self.ends("enci"):      self.r("ence")
            elif self.ends("anci"):    self.r("ance")
        elif self.b[self.k - 1] == 'e':
            if self.ends("izer"):      self.r("ize")
        elif self.b[self.k - 1] == 'l':
            if self.ends("bli"):       self.r("ble") # --DEPARTURE--
            # To match the published algorithm, replace this phrase with
            #   if self.ends("abli"):      self.r("able")
            elif self.ends("alli"):    self.r("al")
            elif self.ends("entli"):   self.r("ent")
            elif self.ends("eli"):     self.r("e")
            elif self.ends("ousli"):   self.r("ous")
        elif self.b[self.k - 1] == 'o':
            if self.ends("ization"):   self.r("ize")
            elif self.ends("ation"):   self.r("ate")
            elif self.ends("ator"):    self.r("ate")
        elif self.b[self.k - 1] == 's':
            if self.ends("alism"):     self.r("al")
            elif self.ends("iveness"): self.r("ive")
            elif self.ends("fulness"): self.r("ful")
            elif self.ends("ousness"): self.r("ous")
        elif self.b[self.k - 1] == 't':
            if self.ends("aliti"):     self.r("al")
            elif self.ends("iviti"):   self.r("ive")
            elif self.ends("biliti"):  self.r("ble")
        elif self.b[self.k - 1] == 'g': # --DEPARTURE--
            if self.ends("logi"):      self.r("log")
        # To match the published algorithm, delete this phrase

    def step3(self):
        """step3() dels with -ic-, -full, -ness etc. similar strategy to step2."""
        if self.b[self.k] == 'e':
            if self.ends("icate"):     self.r("ic")
            elif self.ends("ative"):   self.r("")
            elif self.ends("alize"):   self.r("al")
        elif self.b[self.k] == 'i':
            if self.ends("iciti"):     self.r("ic")
        elif self.b[self.k] == 'l':
            if self.ends("ical"):      self.r("ic")
            elif self.ends("ful"):     self.r("")
        elif self.b[self.k] == 's':
            if self.ends("ness"):      self.r("")

    def step4(self):
        """step4() takes off -ant, -ence etc., in context <c>vcvc<v>."""
        if self.b[self.k - 1] == 'a':
            if self.ends("al"): pass
            else: return
        elif self.b[self.k - 1] == 'c':
            if self.ends("ance"): pass
            elif self.ends("ence"): pass
            else: return
        elif self.b[self.k - 1] == 'e':
            if self.ends("er"): pass
            else: return
        elif self.b[self.k - 1] == 'i':
            if self.ends("ic"): pass
            else: return
        elif self.b[self.k - 1] == 'l':
            if self.ends("able"): pass
            elif self.ends("ible"): pass
            else: return
        elif self.b[self.k - 1] == 'n':
            if self.ends("ant"): pass
            elif self.ends("ement"): pass
            elif self.ends("ment"): pass
            elif self.ends("ent"): pass
            else: return
        elif self.b[self.k - 1] == 'o':
            if self.ends("ion") and (self.b[self.j] == 's' or self.b[self.j] == 't'): pass
            elif self.ends("ou"): pass
            # takes care of -ous
            else: return
        elif self.b[self.k - 1] == 's':
            if self.ends("ism"): pass
            else: return
        elif self.b[self.k - 1] == 't':
            if self.ends("ate"): pass
            elif self.ends("iti"): pass
            else: return
        elif self.b[self.k - 1] == 'u':
            if self.ends("ous"): pass
            else: return
        elif self.b[self.k - 1] == 'v':
            if self.ends("ive"): pass
            else: return
        elif self.b[self.k - 1] == 'z':
            if self.ends("ize"): pass
            else: return
        else:
            return
        if self.m() > 1:
            self.k = self.j

    def step5(self):
        """step5() removes a final -e if m() > 1, and changes -ll to -l if
        m() > 1.
        """
        self.j = self.k
        if self.b[self.k] == 'e':
            a = self.m()
            if a > 1 or (a == 1 and not self.cvc(self.k-1)):
                self.k = self.k - 1
        if self.b[self.k] == 'l' and self.doublec(self.k) and self.m() > 1:
            self.k = self.k -1

    def stem(self, p, i, j):
        """In stem(p,i,j), p is a char pointer, and the string to be stemmed
        is from p[i] to p[j] inclusive. Typically i is zero and j is the
        offset to the last character of a string, (p[j+1] == '\0'). The
        stemmer adjusts the characters p[i] ... p[j] and returns the new
        end-point of the string, k. Stemming never increases word length, so
        i <= k <= j. To turn the stemmer into a module, declare 'stem' as
        extern, and delete the remainder of this file.
        """
        # copy the parameters into statics
        self.b = p
        self.k = j
        self.k0 = i
        if self.k <= self.k0 + 1:
            return self.b # --DEPARTURE--

        # With this line, strings of length 1 or 2 don't go through the
        # stemming process, although no mention is made of this in the
        # published algorithm. Remove the line to match the published
        # algorithm.

        self.step1ab()
        self.step1c()
        self.step2()
        self.step3()
        self.step4()
        self.step5()
        return self.b[self.k0:self.k+1]


#Tokenizer struct to get the tokens from the news
def tokenizer():

    global testList, trainList, earnCount, acqCount, moneyCount, grainCount, crudeCount

    earnTestCount = 0
    acqTestCount = 0
    moneyTestCount = 0
    grainTestCount = 0
    crudeTestCount = 0

    isTitle = 0
    isBody = 0
    isTrain = 0
    isTest = 0
    isClassified = 0
    isTopic = 0
    pointer = 0
    counterClass = 0
    tokens = []
    normalizedTokens = []

    trainCount = 0
    testCount = 0
    classList = ["earn", "acq", "money-fx", "grain", "crude"]
    testid = 0

    stopwords = []
    newid = 0
    tempString = ""
    topicString = ""
    classString = ""

    fileStopwords = open("stopwords.txt", "r")
    fileStopwords = fileStopwords.read()


    for i in range(0 , 5):
    	trainList.append([])
    	trainList[i].append(classList[i])

    for i in range(0, len(fileStopwords)):
        if fileStopwords[i] == "\n":
            stopwords.append(tempString)
            tempString = ""
        else:
            tempString += fileStopwords[i]

    os.chdir("Dataset")

    p = PorterStemmer()

    #take all the files ends with .sgm
    for file in glob.glob("*.sgm"):
        for k in range(0, 1000):
            tokens.append([])
            normalizedTokens.append([])

        files = open(file, "r")
        mainText = files.read()

        tempString = ""

        #tokenizer part starts and first look for newid then, title and body
        for i in range(0, len(mainText)):
            if mainText[pointer: pointer+5] == "NEWID":
                newid += 1
                pointer += 7
                i += 7
                isNewId = 1
                tempString = ""
                while isNewId:
                    if mainText[pointer+1] != ">":
                        tempString += mainText[pointer]
                        pointer += 1
                        i += 1
                    else:
                        isNewId = 0
                        tokens[newid-1].append(tempString)
                        normalizedTokens[newid-1].append(tempString)
                        tempString = ""

            if mainText[pointer: pointer+7] == "<TITLE>":
                isTitle = 1
                pointer += 7
                i += 7

            if mainText[pointer: pointer+8] == "</TITLE>":
                isTitle = 0
                i += 8
                pointer += 8
                if len(tempString) > 1:

                    tempString = p.stem(tempString, 0,len(tempString)-1)
                    tokens[newid-1].append(tempString.lower())

                    if tempString.lower() not in stopwords:
                        tempString = p.stem(tempString, 0,len(tempString)-1)
                        normalizedTokens[newid-1].append(tempString.lower())

                tempString = ""

            if mainText[pointer: pointer+6] == "<BODY>":
                isBody = 1
                i += 6
                pointer += 6

            if mainText[pointer: pointer + 10] == "LEWISSPLIT":
            	i += 12
            	pointer += 12
            	if mainText[pointer: pointer+4] == "TRAI":
            		trainCount += 1
            		isTrain = 1

            	elif mainText[pointer: pointer+4] == "TEST":
            		testCount += 1
            		isTest = 1

            if mainText[pointer: pointer+7] == "</BODY>":
                isBody = 0
                i += 7
                pointer += 7
                if len(tempString) > 1:
                    tokens[newid-1].append(tempString.lower())

                    if tempString.lower() not in stopwords:
                        tempString = p.stem(tempString, 0,len(tempString)-1)
                        normalizedTokens[newid-1].append(tempString.lower())
                tempString = ""

            if mainText[pointer: pointer+7] == "</TEXT>":
            	i += 7
                pointer += 7

            	#Dictionary for test
                if isTest and isClassified:			 
                    testList.append([])
                    testList[testid].append(classString)
                    for l in range(0, len(normalizedTokens[newid-1])):
                        testList[testid].append(normalizedTokens[newid-1][l])
                    testid+=1

                    if classString == "earn":
                        earnTestCount += 1

                    elif classString == "acq":
                        acqTestCount += 1

                    elif classString == "money-fx":
                        moneyTestCount += 1
                    elif classString == "grain":
                        grainTestCount += 1
                    else:
                        crudeTestCount += 1


                if isTrain and isClassified:
                    if classString == "earn":
                        for l in range(1, len(normalizedTokens[newid-1])):
                            trainList[0].append(normalizedTokens[newid-1][l])
                        earnCount += 1

                    elif classString == "acq":
                        for l in range(1, len(normalizedTokens[newid-1])):
                            trainList[1].append(normalizedTokens[newid-1][l])
                        acqCount += 1

                    elif classString == "money-fx":
                        for l in range(1, len(normalizedTokens[newid-1])):
                            trainList[2].append(normalizedTokens[newid-1][l])
                        moneyCount += 1

                    elif classString == "grain":
                        for l in range(1, len(normalizedTokens[newid-1])):
                            trainList[3].append(normalizedTokens[newid-1][l])
                        grainCount += 1

                    else:
                        for l in range(1, len(normalizedTokens[newid-1])):
                            trainList[4].append(normalizedTokens[newid-1][l])
                        crudeCount += 1
	            
                isTest=0
                isTrain=0
                isClassified = 0

	        # Select 5 specific topics
            if mainText[pointer: pointer+8] == "<TOPICS>":
            	isTopic = 1
            	i += 8
            	pointer += 8
    
            if mainText[pointer: pointer+9] == "</TOPICS>":
                isTopic = 0
                i += 9
                pointer += 9
                
                topicString = topicString.replace("<D>", "")
                topicString = topicString.replace("</D>", ",")

                tempTopics = topicString.split(",")

                classString = ""

                for k in range(0, len(tempTopics) - 1):
                	if tempTopics[k] in classList:
                		counterClass += 1
                		classString = tempTopics[k]

               	if counterClass == 1:
               		isClassified = 1
               	else:
               		isClassified = 0
               		classString = ""

               	counterClass = 0
                topicString = ""

            if isTopic:
            	topicString += mainText[pointer]
            	

            if isTitle:
                if mainText[pointer] == "\n":
                    if len(tempString) > 1:
                        if tempString[0] != "&":
                            tokens[newid-1].append(tempString.lower())
                            if tempString.lower() not in stopwords:
                                tempString = p.stem(tempString, 0,len(tempString)-1)
                                normalizedTokens[newid-1].append(tempString.lower())

                    tempString = ""

                elif mainText[pointer] != " " and mainText[pointer] != "." and mainText[pointer] != "," and mainText[pointer] != ";" and mainText[pointer] != "+" and mainText[pointer] != "/" and mainText[pointer] != "(" and mainText[pointer] != ")" and mainText[pointer] != "\"":
                    tempString += mainText[pointer]
                else:
                    if len(tempString) > 1:
                        if tempString[0] != "&" and ">" not in tempString:
                            tokens[newid-1].append(tempString.lower())

                            if tempString.lower() not in stopwords:
                                tempString = p.stem(tempString, 0,len(tempString)-1)
                                normalizedTokens[newid-1].append(tempString.lower())

                    tempString = ""

            if isBody:
                if mainText[pointer] == "\n":
                    if len(tempString) > 1:
                        if tempString[0] != "&":
                            tokens[newid-1].append(tempString.lower())
                            if tempString.lower() not in stopwords:
                                tempString = p.stem(tempString, 0,len(tempString)-1)
                                normalizedTokens[newid-1].append(tempString.lower())

                    tempString = ""

                elif mainText[pointer] != " " and mainText[pointer] != "." and mainText[pointer] != "," and mainText[pointer] != ";" and mainText[pointer] != "+" and mainText[pointer] != "/" and mainText[pointer] != "(" and mainText[pointer] != ")" and mainText[pointer] != "\"":
                    tempString += mainText[pointer]

                else:
                    if len(tempString) > 0:
                        if tempString[0] != "&" and ">" not in tempString:
                            tokens[newid-1].append(tempString.lower())

                            if tempString.lower() not in stopwords:
                                tempString = p.stem(tempString, 0,len(tempString)-1)
                                normalizedTokens[newid-1].append(tempString.lower())
                    tempString = ""

            pointer += 1

        files.close()
        #newid += 1
        pointer = 0
        print("Document #" + file + " is read")
        


    print("\nThe Number Of Documents For 'Earn' Class In The Training-Set : %i" %(earnCount))
    print("The Number Of Documents For 'Acq' Class In The Training-Set : %i" %(acqCount))
    print("The Number Of Documents For 'Money-fx' Class In The Training-Set : %i" %(moneyCount))
    print("The Number Of Documents For 'Grain' Class In The Training-Set : %i" %(grainCount))
    print("The Number Of Documents For 'Crude' Class In The Training-Set : %i" %(crudeCount))

    print("\nThe Number Of Documents For 'Earn' Class In The Test-Set : %i" %(earnTestCount))
    print("The Number Of Documents For 'Acq' Class In The Test-Set : %i" %(acqTestCount))
    print("The Number Of Documents For 'Money-fx' Class In The Test-Set : %i" %(moneyTestCount))
    print("The Number Of Documents For 'Grain' Class In The Test-Set : %i" %(grainTestCount))
    print("The Number Of Documents For 'Crude' Class In The Test-Set : %i \n" %(crudeTestCount))

    os.chdir("..")
    return normalizedTokens

#build dictionary and inverted index file with having tokens
def buildTrainTestSet(tokens, setType):

    fdictionary = open(setType, "w+")
    print(setType + " Created!")
	
    hashmap = dict()

    counter = 0
    for i in range(0, len(tokens)):
        for element in tokens[i]:
            hashmap[counter] = element
            counter += 1
        counter = 0
        fdictionary.write(hashmap.__str__())
        fdictionary.write("\n")
        hashmap = dict()

    fdictionary.close()

def buildVocabulary(tokens):
    global vocabCounter

    hashmap = dict()

    finverted = open("vocabulary", "w+")
    print("File Named 'Vocabulary' Created!")


    for i in range(0, len(tokens)):
        for j in range(1, len(tokens[i])):
            if tokens[i][j] in hashmap:
                hashmap[tokens[i][j]] += 1
            else:
                hashmap[tokens[i][j]] = 1
                vocabCounter += 1

    finverted.write(hashmap.__str__())
    finverted.close()



def buildOccuranceMap():
    global trainList, earnOccuranceMap, acqOccuranceMap, moneyOccuranceMap, grainOccuranceMap, crudeOccuranceMap
    global earnCompOccuranceMap, acqCompOccuranceMap, moneyCompOccuranceMap, grainCompOccuranceMap, crudeCompOccuranceMap

    for i in range(0, len(trainList)):
        hashmap = dict()
        for j in range(1, len(trainList[i])):
            if trainList[i][j] in hashmap:
                hashmap[trainList[i][j]] += 1
            else:
                hashmap[trainList[i][j]] = 1

        if(trainList[i][0] == "earn"):
            earnOccuranceMap = hashmap
            earnCompOccuranceMap = hashmap

        elif(trainList[i][0] == "acq"):
            acqOccuranceMap = hashmap
            acqCompOccuranceMap = hashmap

        elif(trainList[i][0] == "money-fx"):
            moneyOccuranceMap = hashmap
            moneyCompOccuranceMap = hashmap

        elif(trainList[i][0] == "grain"):
            grainOccuranceMap = hashmap
            grainCompOccuranceMap = hashmap

        else:
            crudeOccuranceMap = hashmap
            crudeCompOccuranceMap = hashmap



def buildCMAP():
    global trainList, earnOccuranceMap, acqOccuranceMap, moneyOccuranceMap, grainOccuranceMap, crudeOccuranceMap, vocabCounter
    global earnCmap, acqCmap, moneyCmap, grainCmap, crudeCmap
    global earnCompOccuranceMap, acqCompOccuranceMap, moneyCompOccuranceMap, grainCompOccuranceMap, crudeCompOccuranceMap
    global earnCompCmap, acqCompCmap, moneyCompCmap, grainCompCmap, crudeCompCmap


    earnCmap = dict()
    acqCmap = dict()
    moneyCmap = dict()
    grainCmap = dict()
    crudeCmap = dict()    

    earnCompCmap = dict()
    acqCompCmap = dict()
    moneyCompCmap = dict()
    grainCompCmap = dict()
    crudeCompCmap = dict()

    cmapValue = 0.0
    cmapCompValue = 0.0
    alpha = 1

    for i in range(0, len(trainList)):
        hashmap = dict()

        ## Real Part of the algortihm to find the cmap values
        if(trainList[i][0] == "earn"):
            for element in earnOccuranceMap:
                cmapValue = (float)(earnOccuranceMap[element] + alpha)/ ((len(trainList[0])-1) + (alpha*vocabCounter))
                earnCmap[element] = cmapValue
                cmapValue = 0.0

        elif(trainList[i][0] == "acq"):
            for element in acqOccuranceMap:
                cmapValue = (float)(acqOccuranceMap[element] + alpha)/ ((len(trainList[1])-1) + (alpha*vocabCounter))
                acqCmap[element] = cmapValue
                cmapValue = 0.0

        elif(trainList[i][0] == "money-fx"):
            for element in moneyOccuranceMap:
                cmapValue = (float)(moneyOccuranceMap[element] + alpha)/ ((len(trainList[2])-1) + (alpha*vocabCounter))
                moneyCmap[element] = cmapValue
                cmapValue = 0.0

        elif(trainList[i][0] == "grain"):
            for element in grainOccuranceMap:
                cmapValue = (float)(grainOccuranceMap[element] + alpha)/ ((len(trainList[3])-1) + (alpha*vocabCounter))
                grainCmap[element] = cmapValue
                cmapValue = 0.0

        else:
            for element in crudeOccuranceMap:
                cmapValue = (float)(crudeOccuranceMap[element] + alpha)/ ((len(trainList[4])-1) + (alpha*vocabCounter))
                crudeCmap[element] = cmapValue
                cmapValue = 0.0

    ### Complementary part 
    for element in earnCompOccuranceMap:
        Nk = 0
        if element in acqCompOccuranceMap:
            Nk += acqCompOccuranceMap[element]
        if element in moneyCompOccuranceMap:
            Nk += moneyCompOccuranceMap[element]
        if element in grainCompOccuranceMap:
            Nk += grainCompOccuranceMap[element]
        if element in crudeCompOccuranceMap:
            Nk += crudeCompOccuranceMap[element] 
        cmapCompValue = (float)(Nk + alpha)/ ((len(trainList[1])+len(trainList[2])+len(trainList[3])+len(trainList[4])-4) + (alpha*vocabCounter))
        earnCompCmap[element] = cmapCompValue
        cmapCompValue = 0.0

    for element in acqCompOccuranceMap:
        Nk = 0
        if element in earnCompOccuranceMap:
            Nk += earnCompOccuranceMap[element]
        if element in moneyCompOccuranceMap:
            Nk += moneyCompOccuranceMap[element]
        if element in grainCompOccuranceMap:
            Nk += grainCompOccuranceMap[element]
        if element in crudeCompOccuranceMap:
            Nk += crudeCompOccuranceMap[element] 
        cmapCompValue = (float)(Nk + alpha)/ ((len(trainList[0])+len(trainList[2])+len(trainList[3])+len(trainList[4])-4) + (alpha*vocabCounter))
        acqCompCmap[element] = cmapCompValue
        cmapCompValue = 0.0

    for element in moneyCompOccuranceMap:
        Nk = 0
        if element in acqCompOccuranceMap:
            Nk += acqCompOccuranceMap[element]
        if element in earnCompOccuranceMap:
            Nk += earnCompOccuranceMap[element]
        if element in grainCompOccuranceMap:
            Nk += grainCompOccuranceMap[element]
        if element in crudeCompOccuranceMap:
            Nk += crudeCompOccuranceMap[element] 
        cmapCompValue = (float)(Nk + alpha)/ ((len(trainList[1])+len(trainList[0])+len(trainList[3])+len(trainList[4])-4) + (alpha*vocabCounter))
        moneyCompCmap[element] = cmapCompValue
        cmapCompValue = 0.0

    for element in grainCompOccuranceMap:
        Nk = 0
        if element in acqCompOccuranceMap:
            Nk += acqCompOccuranceMap[element]
        if element in moneyCompOccuranceMap:
            Nk += moneyCompOccuranceMap[element]
        if element in earnCompOccuranceMap:
            Nk += earnCompOccuranceMap[element]
        if element in crudeCompOccuranceMap:
            Nk += crudeCompOccuranceMap[element] 
        cmapCompValue = (float)(Nk + alpha)/ ((len(trainList[1])+len(trainList[2])+len(trainList[0])+len(trainList[4])-4) + (alpha*vocabCounter))
        grainCompCmap[element] = cmapCompValue
        cmapCompValue = 0.0

    for element in crudeCompOccuranceMap:
        Nk = 0
        if element in acqCompOccuranceMap:
            Nk += acqCompOccuranceMap[element]
        if element in moneyCompOccuranceMap:
            Nk += moneyCompOccuranceMap[element]
        if element in grainCompOccuranceMap:
            Nk += grainCompOccuranceMap[element]
        if element in earnCompOccuranceMap:
            Nk += earnCompOccuranceMap[element] 
        cmapCompValue = (float)(Nk + alpha)/ ((len(trainList[1])+len(trainList[2])+len(trainList[3])+len(trainList[0])-4) + (alpha*vocabCounter))
        crudeCompCmap[element] = cmapCompValue
        cmapCompValue = 0.0


def testData(file):
    global earnCmap, acqCmap, moneyCmap, grainCmap, crudeCmap, testList, trainList, vocabCounter
    global earnCompCmap, acqCompCmap, moneyCompCmap, grainCompCmap, crudeCompCmap    
    global earnCount, acqCount, moneyCount, grainCount, crudeCount

    alpha = 1

    hashClassTable = dict()
    hashClassTable["earn"] = [0, 0, 0]
    hashClassTable["acq"] = [0, 0, 0]
    hashClassTable["money-fx"] = [0, 0, 0]
    hashClassTable["grain"] = [0, 0, 0]
    hashClassTable["crude"] = [0, 0, 0]

    totalClassNum = earnCount + acqCount + moneyCount + grainCount + crudeCount
    cmapValue = 0
    cmapCompValue = 0 

    fresultAll = open(file, "w+")

    for i in range(0, len(testList)):
        
        probEarn = math.log10((float)(earnCount)/totalClassNum)
        probAcq = math.log10((float)(acqCount)/totalClassNum)
        probMoney = math.log10((float)(moneyCount)/totalClassNum)
        probGrain = math.log10((float)(grainCount)/totalClassNum)
        probCrude = math.log10((float)(crudeCount)/totalClassNum)    

        probCompEarn = math.log10((float)(totalClassNum - earnCount)/totalClassNum)
        probCompAcq = math.log10((float)(totalClassNum - acqCount)/totalClassNum)
        probCompMoney = math.log10((float)(totalClassNum - moneyCount)/totalClassNum)
        probCompGrain = math.log10((float)(totalClassNum - grainCount)/totalClassNum)
        probCompCrude = math.log10((float)(totalClassNum - crudeCount)/totalClassNum)               
        
        for j in range(2, len(testList[i])):
            if testList[i][j] in earnCmap:               
                probEarn += math.log10(earnCmap[testList[i][j]])
                probCompEarn += math.log10(earnCompCmap[testList[i][j]])
            else:
                cmapValue = math.log10((float)(alpha)/ ((len(trainList[0])-1) + (alpha*vocabCounter)))
                cmapCompValue = math.log10((float)(alpha)/ ((len(trainList[1]) + len(trainList[2]) + len(trainList[3]) + len(trainList[4]) -4) + (alpha*vocabCounter)))
                probCompEarn += cmapCompValue
                probEarn += cmapValue

            if testList[i][j] in acqCmap:  
                probAcq += math.log10(acqCmap[testList[i][j]])                         
                probCompAcq += math.log10(acqCompCmap[testList[i][j]])                         
            
            else:
                cmapValue = math.log10((float)(alpha)/ ((len(trainList[1])-1) + (alpha*vocabCounter)))
                probAcq += cmapValue                
                cmapCompValue = math.log10((float)(alpha)/ ((len(trainList[0]) + len(trainList[2]) + len(trainList[3]) + len(trainList[4]) -4) + (alpha*vocabCounter)))
                probCompAcq += cmapCompValue

            if testList[i][j] in moneyCmap:               
                probMoney += math.log10(moneyCmap[testList[i][j]])
                probCompMoney += math.log10(moneyCompCmap[testList[i][j]])

            else:
                cmapValue = math.log10((float)(alpha)/ ((len(trainList[2])-1) + (alpha*vocabCounter)))
                probMoney += cmapValue
                cmapCompValue = math.log10((float)(alpha)/ ((len(trainList[1]) + len(trainList[0]) + len(trainList[3]) + len(trainList[4]) -4) + (alpha*vocabCounter)))
                probCompMoney += cmapCompValue

            if testList[i][j] in grainCmap:               
                probGrain += math.log10(grainCmap[testList[i][j]])
                probCompGrain += math.log10(grainCompCmap[testList[i][j]])

            else:
                cmapValue = math.log10((float)(alpha)/ ((len(trainList[3])-1) + (alpha*vocabCounter)))
                probGrain += cmapValue
                cmapCompValue = math.log10((float)(alpha)/ ((len(trainList[1]) + len(trainList[2]) + len(trainList[0]) + len(trainList[4]) -4) + (alpha*vocabCounter)))
                probCompGrain += cmapCompValue

            if testList[i][j] in crudeCmap:               
                probCrude += math.log10(crudeCmap[testList[i][j]])               
                probCompCrude += math.log10(crudeCompCmap[testList[i][j]])               

            else:
                cmapValue = math.log10((float)(alpha)/ ((len(trainList[4])-1) + (alpha*vocabCounter)))
                probCrude += cmapValue
                cmapCompValue = math.log10((float)(alpha)/ ((len(trainList[1]) + len(trainList[2]) + len(trainList[3]) + len(trainList[0]) -4) + (alpha*vocabCounter)))
                probCompCrude += cmapCompValue


        
        minIndex = 0
        minValue = 1000000.0
        selectedClass = ""
        listT = [probEarn/probCompEarn, probAcq/probCompAcq, probMoney/probCompMoney, probGrain/probCompGrain, probCrude/probCompCrude]
        for m in range(0, 5):
            if listT[m] < minValue:
                minIndex = m
                minValue = listT[m]

        selectedClass = trainList[minIndex][0]

        if selectedClass == testList[i][0]:
            hashClassTable[selectedClass][0] += 1            
        else:
            hashClassTable[selectedClass][1] += 1
            hashClassTable[testList[i][0]][2] += 1


        fresultAll.write("True Topic for newId(" + testList[i][1] + "): " + testList[i][0] + " --------- MyClassifer => earn_Class: %f, acq_Class: %f, money-fx_Class: %f, grain_Class: %f, crude_Class: %f\n" % (probEarn, probAcq, probMoney, probGrain, probCrude))
        fresultAll.write("Complementary Part(Same Topic)  ------------ MyClassifer => ~earn_Class: %f, ~acq_Class: %f, ~money-fx_Class: %f, ~grain_Class: %f, ~crude_Class: %f\n" % (probCompEarn, probCompAcq, probCompMoney, probCompGrain, probCompCrude))
        fresultAll.write("MyClassifier =======> " + selectedClass + "\n\n")

    print("File named '" + file + "' Created!\n")
    fresultAll.close()

    macroPrecision = 0.0
    macroPrecision += (float)(hashClassTable["earn"][0]) / (hashClassTable["earn"][0] + hashClassTable["earn"][1])
    macroPrecision += (float)(hashClassTable["acq"][0]) / (hashClassTable["acq"][0] + hashClassTable["acq"][1])
    macroPrecision += (float)(hashClassTable["money-fx"][0]) / (hashClassTable["money-fx"][0] + hashClassTable["money-fx"][1])
    macroPrecision += (float)(hashClassTable["grain"][0]) / (hashClassTable["grain"][0] + hashClassTable["grain"][1])
    macroPrecision += (float)(hashClassTable["crude"][0]) / (hashClassTable["crude"][0] + hashClassTable["crude"][1])
    macroPrecision = (float)(macroPrecision)/5

    macroRecall = 0.0
    macroRecall += (float)(hashClassTable["earn"][0]) / (hashClassTable["earn"][0] + hashClassTable["earn"][2])
    macroRecall += (float)(hashClassTable["acq"][0]) / (hashClassTable["acq"][0] + hashClassTable["acq"][2])
    macroRecall += (float)(hashClassTable["money-fx"][0]) / (hashClassTable["money-fx"][0] + hashClassTable["money-fx"][2])
    macroRecall += (float)(hashClassTable["grain"][0]) / (hashClassTable["grain"][0] + hashClassTable["grain"][2])
    macroRecall += (float)(hashClassTable["crude"][0]) / (hashClassTable["crude"][0] + hashClassTable["crude"][2])
    macroRecall = (float)(macroRecall)/5

    macroFMesuare = (float)(2*macroPrecision*macroRecall)/(macroPrecision + macroRecall)

    print("MacroPrecision (for " + file + ") = %f" %(macroPrecision))
    print("MacroRecall (for " + file + ") = %f" %(macroRecall))
    print("Macro F-Mesuare (for " + file + ") = %f\n" %(macroFMesuare))

    totalPre = (float)(hashClassTable["earn"][0] + hashClassTable["acq"][0] + hashClassTable["money-fx"][0] + hashClassTable["grain"][0] + hashClassTable["crude"][0])
    microPrecision = (float)(totalPre) / (totalPre + hashClassTable["earn"][1] + hashClassTable["acq"][1] + hashClassTable["money-fx"][1] + hashClassTable["grain"][1] + hashClassTable["crude"][1])

    microRecall = (float)(totalPre) / (totalPre + hashClassTable["earn"][2] + hashClassTable["acq"][2] + hashClassTable["money-fx"][2] + hashClassTable["grain"][2] + hashClassTable["crude"][2])

    microFMesuare = (float)(2*microPrecision*microRecall)/(microPrecision + microRecall)

    print("MicroPrecision (for " + file + ") = %f" %(microPrecision))
    print("MicroRecall (for " + file + ") = %f" %(microRecall))
    print("Micro F-Mesuare (for " + file + ") = %f\n" %(microFMesuare))

        


def buildMutualInformation():
    global earnOccuranceMap, acqOccuranceMap, moneyOccuranceMap, grainOccuranceMap, crudeOccuranceMap, trainList
    global miEarnScoresMap, miAcqScoresMap, miMoneyScoresMap, miGrainScoresMap, miCrudeScoresMap
    global earnCompOccuranceMap, acqCompOccuranceMap, moneyCompOccuranceMap, grainCompOccuranceMap, crudeCompOccuranceMap
    global miEarnCompScoresMap, miAcqCompScoresMap, miMoneyCompScoresMap, miGrainCompScoresMap, miCrudeCompScoresMap


    totalNumWords = 0
    n01 = 0
    n11 = 0
    n01Comp = 0
    n11Comp = 0
    miScore = 0
    miScoreComp = 0
    temp = 1
    tempComp = 1

    miTable = []
    miTable.append([])
    miTable.append([])

    miTableComp = []
    miTableComp.append([])
    miTableComp.append([])    


    for i in range(0, 5):
        totalNumWords += len(trainList[i]) - 1


    for element in earnOccuranceMap:
        miTable[0].append(earnOccuranceMap[element])
        miTableComp[0].append(earnCompOccuranceMap[element])        
        if element in acqOccuranceMap:
            n01 += acqOccuranceMap[element] 
            n01Comp += acqCompOccuranceMap[element]             
        if element in moneyOccuranceMap:
            n01 += moneyOccuranceMap[element] 
            n01Comp += moneyCompOccuranceMap[element] 
        if element in grainOccuranceMap:
            n01 += grainOccuranceMap[element]
            n01Comp += grainCompOccuranceMap[element]             
        if element in crudeOccuranceMap:
            n01 += crudeOccuranceMap[element]
            n01Comp += crudeCompOccuranceMap[element] 
        miTable[0].append(n01)
        miTable[1].append(len(trainList[0]) - 1 - earnOccuranceMap[element])
        miTable[1].append(totalNumWords - miTable[0][0]- miTable[0][1]- miTable[1][0])

        miTableComp[0].append(n01Comp)
        miTableComp[1].append(len(trainList[0]) - 1 - earnCompOccuranceMap[element])
        miTableComp[1].append(totalNumWords - miTableComp[0][0]- miTableComp[0][1]- miTableComp[1][0])        
        
        for i in range(0, 1):
            for j in range(0, 1):
                temp *= (float)(miTable[i][j])/totalNumWords
                adjI = (i+1)%2
                adjJ = (j+1)%2
                temp *= math.log((float)(totalNumWords*miTable[i][j])/((miTable[i][j] + miTable[i][adjJ])*(miTable[i][j]) + miTable[adjI][j]), 2)
                miScore += temp
                temp = 1

                tempComp *= (float)(miTableComp[i][j])/totalNumWords
                tempComp *= math.log((float)(totalNumWords*miTableComp[i][j])/((miTableComp[i][j] + miTableComp[i][adjJ])*(miTableComp[i][j]) + miTableComp[adjI][j]), 2)
                miScoreComp += tempComp
                tempComp = 1

        miEarnScoresMap[element] = miScore
        miScore = 0
        n01 = 0
        n11 = 0
        miTable = []
        miTable.append([])
        miTable.append([])

        miEarnCompScoresMap[element] = miScoreComp
        miScoreComp = 0
        n01Comp = 0
        n11Comp = 0
        miTableComp = []
        miTableComp.append([])
        miTableComp.append([])        


    for element in acqOccuranceMap:
        miTable[0].append(acqOccuranceMap[element])
        miTableComp[0].append(acqCompOccuranceMap[element])        
        if element in earnOccuranceMap:
            n01 += earnOccuranceMap[element] 
            n01Comp += earnCompOccuranceMap[element]             
        if element in moneyOccuranceMap:
            n01 += moneyOccuranceMap[element] 
            n01Comp += moneyCompOccuranceMap[element] 
        if element in grainOccuranceMap:
            n01 += grainOccuranceMap[element]
            n01Comp += grainCompOccuranceMap[element]             
        if element in crudeOccuranceMap:
            n01 += crudeOccuranceMap[element]
            n01Comp += crudeCompOccuranceMap[element] 
        miTable[0].append(n01)
        miTable[1].append(len(trainList[1]) - 1 - acqOccuranceMap[element])
        miTable[1].append(totalNumWords - miTable[0][0]- miTable[0][1]- miTable[1][0])

        miTableComp[0].append(n01Comp)
        miTableComp[1].append(len(trainList[1]) - 1 - acqCompOccuranceMap[element])
        miTableComp[1].append(totalNumWords - miTableComp[0][0]- miTableComp[0][1]- miTableComp[1][0])        
        
        for i in range(0, 1):
            for j in range(0, 1):
                temp *= (float)(miTable[i][j])/totalNumWords
                adjI = (i+1)%2
                adjJ = (j+1)%2
                temp *= math.log((float)(totalNumWords*miTable[i][j])/((miTable[i][j] + miTable[i][adjJ])*(miTable[i][j]) + miTable[adjI][j]), 2)
                miScore += temp
                temp = 1

                tempComp *= (float)(miTableComp[i][j])/totalNumWords
                tempComp *= math.log((float)(totalNumWords*miTableComp[i][j])/((miTableComp[i][j] + miTableComp[i][adjJ])*(miTableComp[i][j]) + miTableComp[adjI][j]), 2)
                miScoreComp += tempComp
                tempComp = 1

        miAcqScoresMap[element] = miScore
        miScore = 0
        n01 = 0
        n11 = 0
        miTable = []
        miTable.append([])
        miTable.append([])

        miAcqCompScoresMap[element] = miScoreComp
        miScoreComp = 0
        n01Comp = 0
        n11Comp = 0
        miTableComp = []
        miTableComp.append([])
        miTableComp.append([])   


    for element in moneyOccuranceMap:
        miTable[0].append(moneyOccuranceMap[element])
        miTableComp[0].append(moneyCompOccuranceMap[element])        
        if element in acqOccuranceMap:
            n01 += acqOccuranceMap[element] 
            n01Comp += acqCompOccuranceMap[element]             
        if element in earnOccuranceMap:
            n01 += earnOccuranceMap[element] 
            n01Comp += earnCompOccuranceMap[element] 
        if element in grainOccuranceMap:
            n01 += grainOccuranceMap[element]
            n01Comp += grainCompOccuranceMap[element]             
        if element in crudeOccuranceMap:
            n01 += crudeOccuranceMap[element]
            n01Comp += crudeCompOccuranceMap[element] 
        miTable[0].append(n01)
        miTable[1].append(len(trainList[2]) - 1 - moneyOccuranceMap[element])
        miTable[1].append(totalNumWords - miTable[0][0]- miTable[0][1]- miTable[1][0])

        miTableComp[0].append(n01Comp)
        miTableComp[1].append(len(trainList[2]) - 1 - moneyCompOccuranceMap[element])
        miTableComp[1].append(totalNumWords - miTableComp[0][0]- miTableComp[0][1]- miTableComp[1][0])        
        
        for i in range(0, 1):
            for j in range(0, 1):
                temp *= (float)(miTable[i][j])/totalNumWords
                adjI = (i+1)%2
                adjJ = (j+1)%2
                temp *= math.log((float)(totalNumWords*miTable[i][j])/((miTable[i][j] + miTable[i][adjJ])*(miTable[i][j]) + miTable[adjI][j]), 2)
                miScore += temp
                temp = 1

                tempComp *= (float)(miTableComp[i][j])/totalNumWords
                tempComp *= math.log((float)(totalNumWords*miTableComp[i][j])/((miTableComp[i][j] + miTableComp[i][adjJ])*(miTableComp[i][j]) + miTableComp[adjI][j]), 2)
                miScoreComp += tempComp
                tempComp = 1

        miMoneyScoresMap[element] = miScore
        miScore = 0
        n01 = 0
        n11 = 0
        miTable = []
        miTable.append([])
        miTable.append([])

        miMoneyCompScoresMap[element] = miScoreComp
        miScoreComp = 0
        n01Comp = 0
        n11Comp = 0
        miTableComp = []
        miTableComp.append([])
        miTableComp.append([])   


    for element in grainOccuranceMap:
        miTable[0].append(grainOccuranceMap[element])
        miTableComp[0].append(grainCompOccuranceMap[element])        
        if element in acqOccuranceMap:
            n01 += acqOccuranceMap[element] 
            n01Comp += acqCompOccuranceMap[element]             
        if element in moneyOccuranceMap:
            n01 += moneyOccuranceMap[element] 
            n01Comp += moneyCompOccuranceMap[element] 
        if element in earnOccuranceMap:
            n01 += earnOccuranceMap[element]
            n01Comp += earnCompOccuranceMap[element]             
        if element in crudeOccuranceMap:
            n01 += crudeOccuranceMap[element]
            n01Comp += crudeCompOccuranceMap[element] 
        miTable[0].append(n01)
        miTable[1].append(len(trainList[3]) - 1 - grainOccuranceMap[element])
        miTable[1].append(totalNumWords - miTable[0][0]- miTable[0][1]- miTable[1][0])

        miTableComp[0].append(n01Comp)
        miTableComp[1].append(len(trainList[3]) - 1 - grainCompOccuranceMap[element])
        miTableComp[1].append(totalNumWords - miTableComp[0][0]- miTableComp[0][1]- miTableComp[1][0])        
        
        for i in range(0, 1):
            for j in range(0, 1):
                temp *= (float)(miTable[i][j])/totalNumWords
                adjI = (i+1)%2
                adjJ = (j+1)%2
                temp *= math.log((float)(totalNumWords*miTable[i][j])/((miTable[i][j] + miTable[i][adjJ])*(miTable[i][j]) + miTable[adjI][j]), 2)
                miScore += temp
                temp = 1

                tempComp *= (float)(miTableComp[i][j])/totalNumWords
                tempComp *= math.log((float)(totalNumWords*miTableComp[i][j])/((miTableComp[i][j] + miTableComp[i][adjJ])*(miTableComp[i][j]) + miTableComp[adjI][j]), 2)
                miScoreComp += tempComp
                tempComp = 1

        miGrainScoresMap[element] = miScore
        miScore = 0
        n01 = 0
        n11 = 0
        miTable = []
        miTable.append([])
        miTable.append([])

        miGrainCompScoresMap[element] = miScoreComp
        miScoreComp = 0
        n01Comp = 0
        n11Comp = 0
        miTableComp = []
        miTableComp.append([])
        miTableComp.append([])   


    for element in crudeOccuranceMap:
        miTable[0].append(crudeOccuranceMap[element])
        miTableComp[0].append(crudeCompOccuranceMap[element])        
        if element in acqOccuranceMap:
            n01 += acqOccuranceMap[element] 
            n01Comp += acqCompOccuranceMap[element]             
        if element in moneyOccuranceMap:
            n01 += moneyOccuranceMap[element] 
            n01Comp += moneyCompOccuranceMap[element] 
        if element in grainOccuranceMap:
            n01 += grainOccuranceMap[element]
            n01Comp += grainCompOccuranceMap[element]             
        if element in earnOccuranceMap:
            n01 += earnOccuranceMap[element]
            n01Comp += earnCompOccuranceMap[element] 
        miTable[0].append(n01)
        miTable[1].append(len(trainList[4]) - 1 - crudeOccuranceMap[element])
        miTable[1].append(totalNumWords - miTable[0][0]- miTable[0][1]- miTable[1][0])

        miTableComp[0].append(n01Comp)
        miTableComp[1].append(len(trainList[4]) - 1 - crudeCompOccuranceMap[element])
        miTableComp[1].append(totalNumWords - miTableComp[0][0]- miTableComp[0][1]- miTableComp[1][0])        
        
        for i in range(0, 1):
            for j in range(0, 1):
                temp *= (float)(miTable[i][j])/totalNumWords
                adjI = (i+1)%2
                adjJ = (j+1)%2
                temp *= math.log((float)(totalNumWords*miTable[i][j])/((miTable[i][j] + miTable[i][adjJ])*(miTable[i][j]) + miTable[adjI][j]), 2)
                miScore += temp
                temp = 1

                tempComp *= (float)(miTableComp[i][j])/totalNumWords
                tempComp *= math.log((float)(totalNumWords*miTableComp[i][j])/((miTableComp[i][j] + miTableComp[i][adjJ])*(miTableComp[i][j]) + miTableComp[adjI][j]), 2)
                miScoreComp += tempComp
                tempComp = 1

        miCrudeScoresMap[element] = miScore
        miScore = 0
        n01 = 0
        n11 = 0
        miTable = []
        miTable.append([])
        miTable.append([])

        miCrudeCompScoresMap[element] = miScoreComp
        miScoreComp = 0
        n01Comp = 0
        n11Comp = 0
        miTableComp = []
        miTableComp.append([])
        miTableComp.append([])   




    inc = 0
    tempDict = dict()
    tempDictComp = dict()    
    topCounter = collections.Counter(miEarnScoresMap).most_common(50)
    topCounterComp = collections.Counter(miEarnCompScoresMap).most_common(50)
    for element in topCounter:
        tempDict[topCounter[inc][0]] = earnOccuranceMap[topCounter[inc][0]]
        inc += 1    
    earnOccuranceMap = tempDict
    inc = 0
    for element in topCounterComp:
        tempDictComp[topCounterComp[inc][0]] = earnCompOccuranceMap[topCounterComp[inc][0]]
        inc += 1    
    earnCompOccuranceMap = tempDictComp

    inc = 0
    tempDict = dict()
    tempDictComp = dict()
    topCounter = collections.Counter(miAcqScoresMap).most_common(50)
    topCounterComp = collections.Counter(miAcqCompScoresMap).most_common(50)
    for element in topCounter:
        tempDict[topCounter[inc][0]] = acqOccuranceMap[topCounter[inc][0]]
        inc += 1    
    acqOccuranceMap = tempDict
    inc = 0
    for element in topCounterComp:
        tempDictComp[topCounterComp[inc][0]] = acqCompOccuranceMap[topCounterComp[inc][0]]
        inc += 1    
    acqCompOccuranceMap = tempDictComp


    inc = 0
    tempDict = dict()
    tempDictComp = dict()
    topCounter = collections.Counter(miMoneyScoresMap).most_common(50)
    topCounterComp = collections.Counter(miMoneyCompScoresMap).most_common(50)
    for element in topCounter:
        tempDict[topCounter[inc][0]] = moneyOccuranceMap[topCounter[inc][0]]
        inc += 1    
    moneyOccuranceMap = tempDict
    inc = 0
    for element in topCounterComp:
        tempDictComp[topCounterComp[inc][0]] = moneyCompOccuranceMap[topCounterComp[inc][0]]
        inc += 1    
    moneyCompOccuranceMap = tempDictComp


    inc = 0
    tempDict = dict()
    tempDictComp = dict()
    topCounter = collections.Counter(miGrainScoresMap).most_common(50)
    topCounterComp = collections.Counter(miGrainCompScoresMap).most_common(50)
    for element in topCounter:
        tempDict[topCounter[inc][0]] = grainOccuranceMap[topCounter[inc][0]]
        inc += 1    
    grainOccuranceMap = tempDict
    inc = 0
    for element in topCounterComp:
        tempDictComp[topCounterComp[inc][0]] = grainCompOccuranceMap[topCounterComp[inc][0]]
        inc += 1    
    grainCompOccuranceMap = tempDictComp


    inc = 0
    tempDict = dict()
    tempDictComp = dict()
    topCounter = collections.Counter(miCrudeScoresMap).most_common(50)
    topCounterComp = collections.Counter(miCrudeCompScoresMap).most_common(50)
    for element in topCounter:
        tempDict[topCounter[inc][0]] = crudeOccuranceMap[topCounter[inc][0]]
        inc += 1    
    crudeOccuranceMap = tempDict
    inc = 0
    for element in topCounterComp:
        tempDictComp[topCounterComp[inc][0]] = crudeCompOccuranceMap[topCounterComp[inc][0]]
        inc += 1    
    crudeCompOccuranceMap = tempDictComp    
   
 

tokenizer()
#buildTrainTestSet(testList, "testList")
#buildTrainTestSet(trainList, "trainList")
buildVocabulary(trainList)
buildOccuranceMap()
buildCMAP()
testData("Score_AllLexion")


buildMutualInformation()
buildCMAP()
testData("Score_MutualInformation")

top50List = []
for element in earnOccuranceMap:
    top50List.append(element)
print("Most 50 Discriminating Words List For 'Earn' Class:")
print(top50List)

top50List = []
for element in acqOccuranceMap:
    top50List.append(element)
print("\nMost 50 Discriminating Words List For 'Acq' Class:")
print(top50List)


top50List = []
for element in moneyOccuranceMap:
    top50List.append(element)
print("\nMost 50 Discriminating Words List For 'Money-fx' Class:")
print(top50List)


top50List = []
for element in grainOccuranceMap:
    top50List.append(element)
print("\nMost 50 Discriminating Words List For 'Grain' Class:")
print(top50List)


top50List = []
for element in crudeOccuranceMap:
    top50List.append(element)
print("\nMost 50 Discriminating Words List For 'Crude' Class:")
print(top50List)    