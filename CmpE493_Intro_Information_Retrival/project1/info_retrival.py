#!/usr/bin/python

import os, sys, glob, time, re
import ast

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

    isTitle = 0
    isBody = 0
    pointer = 0
    tokens = []
    normalizedTokens = []

    stopwords = []
    newid = 0
    tempString = ""

    fileStopwords = open("stopwords.txt", "r")
    fileStopwords = fileStopwords.read()


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
                        #print(tempString)
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

    return normalizedTokens

#build dictionary and inverted index file with having tokens
def buildDictionary_InvertedIndex(tokens):
    os.chdir("..")
    isExistDic = 0
    isExistInv = 0

    if os.path.exists("dictionary"):
        print("Dictionary Exists! Delete the file if it needs to be replaced")
        isExistDic = 1
    else:
        fdictionary = open("dictionary", "w+")
        print("Dictionary Created!")

    if os.path.exists("inverted index"):
        print("Inverted Index Exists! Delete the file if it needs to be replaced")
        isExistInv = 1
    else:
        finverted = open("inverted index", "w+")
        print("Inverted Index Created!")


    hashmap = dict()
    newList = []
    tempList = []

    counter = 0
    if isExistDic == 0:
        for i in range(0, len(tokens)):
            for element in tokens[i]:
                hashmap[counter] = element
                counter += 1
            counter = 0
            fdictionary.write(hashmap.__str__())
            fdictionary.write("\n")
            hashmap = dict()

        fdictionary.close()

    fdictionary = open("dictionary", "r")

    allDictionary = fdictionary.read()
    listSize = 0

    for i in range(0, len(allDictionary)):
        if allDictionary[i] == "\n":
            listSize += 1

    fdictionary.close()
    fdictionary = open("dictionary", "r")

    fileId = 0
    for i in range(0, listSize):
        newList.append([])
        line = fdictionary.readline()
        line = ast.literal_eval(line)

        for j in range(0, len(line.values())):
            newList[fileId].append(line.get(j))

        fileId += 1

    fdictionary.close()

    if isExistInv == 0:
        for i in range(0, len(newList)):
            for element in newList[i]:
                if element in hashmap:
                    tempInv = hashmap.get(element)
                    if newList[i][0] not in tempInv:
                        hashmap[element].append(newList[i][0])
                else:
                    hashmap[element] = [newList[i][0]]
                    counter += 1

        finverted.write(hashmap.__str__())
        finverted.close()

#merge algorithm
def merge(queryList, mapinverted):

    list = []
    tempList = []
    returnList = []
    isEmpty = 0

    for i in range(0, len(queryList)):
        list.append([])
        list[i] = mapinverted.get(queryList[i])
        if not list[i]:
            isEmpty = 1

    if isEmpty:
        print("No Document has that query!")

    else:
        returnList = list[0]

        for i in range(1, len(list)):
            for j in range(0, len(list[i])):
                if list[i][j] in returnList:
                    tempList.append(list[i][j])
            returnList = tempList
            tempList = []

    return returnList

#function for phrase and proximity queries
def proximity(mergedList, proxyNum, splitedList):
    position = 0
    returnList = []

    fdictionary = open("dictionary", "r")

    allDictionary = fdictionary.read()
    listSize = 0

    for i in range(0, len(allDictionary)):
        if allDictionary[i] == "\n":
            listSize += 1

    fdictionary.close()
    fdictionary = open("dictionary", "r")

    if len(mergedList) != 0:
        if proxyNum == "0":
            for i in range(0, listSize):
                line = fdictionary.readline()

                hashmap = ast.literal_eval(line)
                if len(mergedList) > position:
                    if hashmap.get(0) == mergedList[position]:
                        isFound = 0
                        position += 1
                        counter = 0
                        while isFound == 0:
                            counter += 1
                            if hashmap.get(counter) == splitedList[0]:
                                isExist = 1
                                for j in range(1, len(splitedList)):
                                    if hashmap.get(counter + j) != splitedList[j]:
                                        isExist = 0

                                if isExist:
                                    if mergedList[position-1] not in returnList:
                                        returnList.append(mergedList[position-1])

                            if counter > len(hashmap):
                                isFound = 2

        else:
            for i in range(0, listSize):
                line = fdictionary.readline()

                hashmap = ast.literal_eval(line)
                if len(mergedList) > position:
                    if hashmap.get(0) == mergedList[position]:
                        isFound = 0
                        position += 1
                        counter = 0
                        while isFound == 0:
                            counter += 1
                            if hashmap.get(counter) == splitedList[0]:
                                isExist = 1
                                if hashmap.get(counter + int(proxyNum) + 1) != splitedList[1]:
                                    isExist = 0
                                if isExist:
                                    if mergedList[position-1] not in returnList:
                                        returnList.append(mergedList[position-1])

                            if counter > len(hashmap):
                                isFound = 2



    fdictionary.close()

    return returnList

#main part that returns the queries
def booleanQueryProcessor():
    finverted = open("inverted index", "r")
    isContinue = 1

#Sorting
    """
    mapinverted = finverted.read()
    mapinverted = ast.literal_eval(mapinverted)

    listKeys = list(mapinverted.keys())

    for l in range(len(listKeys)-1,0,-1):
        for o in range(l):
            if len(mapinverted.get(listKeys[o]))< len(mapinverted.get(listKeys[o+1])):
                temp = listKeys[o]
                listKeys[o] = listKeys[o+1]
                listKeys[o+1] = temp

    print(listKeys[0:20])

    print(len(listKeys))


    finverted.close()
    finverted = open("inverted index", "r")
    """
#Ends

    p = PorterStemmer()

    print("\nEnter the number for Query Type \n'1' for conjunctive query \n'2' for phrase query \n'3' for proximity query")
    answer = input()

    while isContinue == 1:
        if answer == "1" or answer == "'1'":
            print("Conjunctive Query")
            conjuctive = input("Enter The Query: ").lower()
            mapinverted = finverted.read()
            mapinverted = ast.literal_eval(mapinverted)

            splitedList = conjuctive.split(" and ")
            for i in range(0, len(splitedList)):
                splitedList[i] = p.stem(splitedList[i], 0,len(splitedList[i])-1).lower()

            finalList = merge(splitedList, mapinverted)
            print("\nDocument ID's that have that query:")
            print(finalList)

        elif answer == "2" or answer == "'2'":
            print("Phase Query")
            phase = input("Enter The Query: ").lower()
            mapinverted = finverted.read()
            mapinverted = ast.literal_eval(mapinverted)

            splitedList = phase.split(" ")
            for i in range(0, len(splitedList)):
                splitedList[i] = p.stem(splitedList[i], 0,len(splitedList[i])-1).lower()

            mergedList = merge(splitedList, mapinverted)
            finalList = proximity(mergedList, "0", splitedList)
            print("\nDocument ID's that have that query:")
            print(finalList)

        elif answer == "3" or answer == "'3'":
            print("Proximity Query")
            phase = input("Enter The Query: ").lower()
            mapinverted = finverted.read()
            mapinverted = ast.literal_eval(mapinverted)
            splitedList = []

            tempList = phase.split(" ")
            splitedList.append(tempList[0])
            splitedList.append(tempList[2])
            proxyNum = tempList[1][1:]

            for i in range(0, len(splitedList)):
                splitedList[i] = p.stem(splitedList[i], 0,len(splitedList[i])-1).lower()

            mergedList = merge(splitedList, mapinverted)
            finalList = proximity(mergedList, proxyNum, splitedList)
            print("\nDocument ID's that have that query:")
            print(finalList)

        else:
            print("Wrong Input!")

        answer = input("\nWant to type query again? (type 'y' for yes, any other string to quit): ")

        finverted.close()

        if answer.lower() == "y":
            isContinue = 1
            print("\nEnter the number for Query Type \n'1' for conjunctive query \n'2' for phrase query \n'3' for proximity query")
            answer = input()
            finverted = open("inverted index", "r")

        else:
            isContinue = 0


tokens = tokenizer()
buildDictionary_InvertedIndex(tokens)
booleanQueryProcessor()
