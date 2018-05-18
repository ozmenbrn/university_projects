#!/usr/bin/python

import os, sys, glob, subprocess  
import ast, math, collections, json
import numpy as np  # Numpy library as variable np

#from rouge import FilesRouge    $$$ Rogue Library is not working in python2.7 so i did it via shell scripting trough subprocess.Popen and get the results

tokens = []    ## All tokens as 3-d array
tokens.append([])  ## First element 0 will not be used
newsTokens = []  ## News as 3-d array
newsTokens.append([])  ## First element 0 will not be used
summaryTokens = [] ## Summary tokens as 3-d array
summaryTokens.append([])  ## First element 0 will not be used
documents = []  ## sentences as 2-d array
documents.append([]) ## First element 0 will not be used
newsDocuments = [] ## news sentences as 2-d array
newsDocuments.append([]) ## First element 0 will not be used
summaryDocuments = [] ## summary sentences as 2-d array
summaryDocuments.append([]) ## First element 0 will not be used

documentTokens = []  ## same as tokens
documentTokens.append([]) ## First element 0 will not be used
newsDocumentTokens = [] ## same as newsTokens
newsDocumentTokens.append([]) ## First element 0 will not be used
summaryDocumentTokens = [] ## same as Summary tokens
summaryDocumentTokens.append([]) ## First element 0 will not be used


cosineMatrix = dict() ## Cosine scores as mapping

lexRankScores = dict() ## lexrank scores as mapping


## Dictionaries
allDict = dict()
newsDict = dict() 
summaryDict = dict()
idfScores = dict()
tfScores = dict()
tfxidfScores = dict()
termOccurance = dict()
mySummaryText = dict()

documentNumber = 0

#Tokenizer struct to get the tokens from the news
def tokenizer():

    global tokens, newsTokens, summaryTokens, documents, newsDocuments, summaryDocuments, allDict, newsDict, summaryDict, documentNumber, termOccurance, tfScores, tfxidfScores
    global documentTokens, newsDocumentTokens, summaryDocumentTokens

    tempString = ""
    documentId = 0
    isNews = 1
    tempArray = []

    counter = 0

    ## Append 1000 element to arrays to make able to create 2-d array with 1000 rows + 1 row(for element 0 = null)
    for i in range(0,1000):
        documents.append([])
        newsDocuments.append([])
        summaryDocuments.append([])
        tokens.append([])
        newsTokens.append([])
        summaryTokens.append([])
        documentTokens.append([])
        newsDocumentTokens.append([])
        summaryDocumentTokens.append([])


    ## For all files ends with .txt do
    for file in glob.glob("*.txt"):

        #Parser documentId as integer
        tempString = file
        tempInt =  file.index(".")
        tempString = tempString[0:tempInt]
        documentId = int(tempString);    

        # Dictionary created for a key value as document Id
        documentTokens[documentId].append([])
        newsDocumentTokens[documentId].append([])
        summaryDocumentTokens[documentId].append([])
        counter = 0
        counterSummary = 0

        documentNumber = documentNumber + 1

        files = open(file, "r")
        mainText = files.read()

        tempString = ""
        tokenString = ""

        #tokenizer part starts and first look for newid then, title and body
        for i in range(0, len(mainText)):
            if mainText[i] == '\n':  ## Read line by line
                if mainText[i-1] == '\n':  ## If the second \n comes in that means we are in summary now
                    tempString = ""
                    tokenString = ""
                    isNews = 0
                    counterSummary = 0
                else:
                    documents[documentId].append(tempString)
                    if isNews:
                        newsDocuments[documentId].append(tempString)
                    else:
                        summaryDocuments[documentId].append(tempString)
                    
                    ## If there exists string
                    if len(tokenString) > 0:  
                        if tokenString not in allDict:
                            allDict[tokenString] = [documentId]

                        else:
                            tempArray = allDict[tokenString]
                            if documentId not in tempArray:
                                allDict[tokenString].append(documentId)

                        tokens[documentId].append(tokenString)
                        documentTokens[documentId][counter].append(tokenString)

                        ## Main part that concern us for calculating idf scores and making lists contains news
                        if isNews:
                            newsTokens[documentId].append(tokenString)
                            newsDocumentTokens[documentId][counter].append(tokenString)
                            if tokenString not in newsDict:
                                newsDict[tokenString] = [documentId]
                                termOccurance[tokenString] = {}
                                tfScores[tokenString] = {}
                                tfScores[tokenString][documentId] = {}
                                tfxidfScores[tokenString] = {}
                                tfxidfScores[tokenString][documentId] = {}
                                termOccurance[tokenString][documentId] = {}   ##
                                termOccurance[tokenString][documentId][counter] = 1 ##

                            else:
                                #print(termOccurance[tokenString].keys())
                                if documentId not in termOccurance[tokenString].keys():
                                    termOccurance[tokenString][documentId] = {} ##
                                    tfScores[tokenString][documentId] = {}      
                                    tfxidfScores[tokenString][documentId] = {}
                                    termOccurance[tokenString][documentId][counter] = 1 ##
                                else:
                                    if counter not in termOccurance[tokenString][documentId].keys():
                                        termOccurance[tokenString][documentId][counter] = 1 ##
                                    else:
                                        termOccurance[tokenString][documentId][counter] += 1 ##

                                tempArray = newsDict[tokenString]
                                if documentId not in tempArray:
                                    newsDict[tokenString].append(documentId)
                        
                        ## IF summary
                        else:
                            summaryTokens[documentId].append(tokenString)
                            summaryDocumentTokens[documentId][counterSummary].append(tokenString)
                            if tokenString not in summaryDict:
                                summaryDict[tokenString] = [documentId]
                            else:
                                tempArray = summaryDict[tokenString]
                                if documentId not in tempArray:
                                    summaryDict[tokenString].append(documentId)                            

                tempString = ""
                tokenString = ""
                counter = counter + 1
                counterSummary = counterSummary + 1
                documentTokens[documentId].append([])
                newsDocumentTokens[documentId].append([])
                summaryDocumentTokens[documentId].append([])
            
            ## If we are on same line, look for punctuations, if exist reset tempstring, else if it is space and string with length > 0 add it as token
            else:
                tempString = tempString + mainText[i]
                if mainText[i] == ',' or mainText[i] == '.' or mainText[i] == '-' or mainText[i] == '"' or mainText[i] == ';' or mainText[i] == ':' or mainText[i] == '&' or mainText[i] == '/' or mainText[i] == '{' or mainText[i] == '}' or mainText[i] == '*' or mainText[i] == '+' or mainText[i] == '^':
                    tokenString = ""
                elif mainText[i] == ' ':
                    if len(tokenString) > 0:
                        if tokenString not in allDict:
                            allDict[tokenString] = [documentId]
                        else:
                            tempArray = allDict[tokenString]
                            if documentId not in tempArray:
                                allDict[tokenString].append(documentId)

                        tokens[documentId].append(tokenString)
                        documentTokens[documentId][counter].append(tokenString)

                        if isNews:
                            newsTokens[documentId].append(tokenString)
                            newsDocumentTokens[documentId][counter].append(tokenString)
                            if tokenString not in newsDict:
                                newsDict[tokenString] = [documentId]
                                termOccurance[tokenString] = {}
                                tfScores[tokenString] = {}
                                tfScores[tokenString][documentId] = {}
                                tfxidfScores[tokenString] = {}
                                tfxidfScores[tokenString][documentId] = {}
                                termOccurance[tokenString][documentId] = {}   ##
                                termOccurance[tokenString][documentId][counter] = 1 ##

                            else:
                                if documentId not in termOccurance[tokenString].keys():
                                    termOccurance[tokenString][documentId] = {} ##
                                    tfScores[tokenString][documentId] = {}      
                                    tfxidfScores[tokenString][documentId] = {}                              
                                    termOccurance[tokenString][documentId][counter] = 1 ##
                                else:
                                    if counter not in termOccurance[tokenString][documentId].keys():
                                        termOccurance[tokenString][documentId][counter] = 1 ##
                                    else:
                                        termOccurance[tokenString][documentId][counter] += 1 ##

                                tempArray = newsDict[tokenString]
                                if documentId not in tempArray:
                                    newsDict[tokenString].append(documentId)
                        else:
                            summaryTokens[documentId].append(tokenString)
                            summaryDocumentTokens[documentId][counterSummary].append(tokenString)
                            if tokenString not in summaryDict:
                                summaryDict[tokenString] = [documentId]
                            else:
                                tempArray = summaryDict[tokenString]
                                if documentId not in tempArray:
                                    summaryDict[tokenString].append(documentId)   
                        tokenString = ""                         
                    else:
                        tokenString = ""
                else:
                    tokenString = tokenString + mainText[i]


        summaryDocumentTokens[documentId].pop(0)
        isNews = 1


## Mehthod for calculating idf, tf and tfxidf scores
def calculateTfxIdf():

    global documentNumber, newsDict, idfScores, tfScores, tfxidfScores, termOccurance

    ## Look every term in news Dictionary
    for term in newsDict:
        idfScores[term] = math.log((float)(documentNumber)/len(newsDict[term])) ## Calculate idf scores of terms
        for documentId in termOccurance[term].keys(): ## For tf scores we need to go inside in document
            for sentences in termOccurance[term][documentId]: ## Look for sentences to calculate tf scores
                tfScores[term][documentId][sentences] = 1 + math.log(termOccurance[term][documentId][sentences]) ## tf score
                tfxidfScores[term][documentId][sentences] = tfScores[term][documentId][sentences]*idfScores[term]  ## tfxidf score              


## Build Cosine matrix for spesific file
def buildGraph(fileName):

    global documentNumber, idfScores, tfScores, tfxidfScores, documentTokens, newsDocumentTokens, summaryDocumentTokens, cosineMatrix

    teleportRate = 0.15 ## Constant teleport rate
    treshold = 0.10
    degree = 0

    tempString = fileName
    tempInt =  fileName.index(".")
    tempString = tempString[0:tempInt]
    documentId = int(tempString);

    #for documentId in range(1, documentNumber+1):    ## In case for looking all files and build cosine matrixes for all txt files

    cosineMatrix[documentId] = []

    for i in range(len(newsDocuments[documentId])):  ## Number of sentences i = to ... N
        cosineMatrix[documentId].append([])
        degree = 0  ## Make degree = 0 in each time of a new sentence
        for j in range(len(newsDocuments[documentId])): ## Number of sentences j = 0, to ... N
            cosineUpper = 0  ## upper part of the formula
            cosineBottomX = 0 ## Bottom part
            cosineBottomY = 0 ## Bottom part
            for k in range(len(newsDocumentTokens[documentId][i])):  ## Look for terms in a sentence
                if newsDocumentTokens[documentId][i][k] in newsDocumentTokens[documentId][j]: # If term exists to be able to calculate cosineupper
                    cosineUpper = cosineUpper + tfxidfScores[newsDocumentTokens[documentId][i][k]][documentId][i]*tfxidfScores[newsDocumentTokens[documentId][i][k]][documentId][j]
                cosineBottomX = cosineBottomX + tfxidfScores[newsDocumentTokens[documentId][i][k]][documentId][i]*tfxidfScores[newsDocumentTokens[documentId][i][k]][documentId][i]

            for l in range(len(newsDocumentTokens[documentId][j])): ## Look for terms in a sentencee
                if newsDocumentTokens[documentId][j][l] in newsDocumentTokens[documentId][i]: # If term exists to be able to calculate cosineupper
                    cosineUpper = cosineUpper + tfxidfScores[newsDocumentTokens[documentId][j][l]][documentId][j]*tfxidfScores[newsDocumentTokens[documentId][j][l]][documentId][i]
                cosineBottomY = cosineBottomY + tfxidfScores[newsDocumentTokens[documentId][j][l]][documentId][j]*tfxidfScores[newsDocumentTokens[documentId][j][l]][documentId][j]
            
            cosineScore = cosineUpper/(math.sqrt(cosineBottomX) * math.sqrt(cosineBottomY)) ## Do the calculation

            ## Look for the treshold value
            if(cosineScore > treshold):
                cosineMatrix[documentId][i].append(1.0)
                degree = degree + 1
            else:
                cosineMatrix[documentId][i].append(0.0)  

        ## Add teleportation to the cosine matrix        
        for j in range(len(newsDocuments[documentId])):
            temp = 1.0/len(newsDocuments[documentId])
            temp = temp*teleportRate
            if(cosineMatrix[documentId][i][j] == 0.0):
                cosineMatrix[documentId][i][j] = temp
            else:
                calc = (cosineMatrix[documentId][i][j]/degree) - (len(newsDocuments[documentId]) - degree)*(temp/degree)
                cosineMatrix[documentId][i][j] = calc   
                
                    
## Apply power method
def powerMethod(fileName):

    global documentNumber, lexRankScores, cosineMatrix, newsDocuments

    errorTolerance = 0.00001 ## Constant

    # Document id parser
    tempString = fileName
    tempInt =  fileName.index(".")
    tempString = tempString[0:tempInt]
    documentId = int(tempString);

    #for documentId in range(1, documentNumber+1):   ## In case for looking all files and build cosine matrixes for all txt files 

    lexRankScores[documentId] = []  ## Assign array to a dictionay

    ## Formula from the link
    for sentence in range(len(newsDocuments[documentId])):
    
        eigenVector = []
        eigenVector.append(1.0/documentNumber)
        sigma = errorTolerance + 1

        t = 0
        
        while (sigma>errorTolerance):
            t = t + 1
            transpose = np.array(cosineMatrix[documentId][sentence])
            transpose = transpose.transpose()
            eigenVector.append(np.array(transpose)*np.array(eigenVector[t-1]))
            sigma = abs(eigenVector[t]- eigenVector[t-1]).all()

        lexRankScores[documentId].append(eigenVector[t][sentence])


## Prints output of a program
def output(fileName):

    global newsDocuments, lexRankScores, mySummaryText

    ## Document id parser
    tempString = fileName
    tempInt =  fileName.index(".")
    tempString = tempString[0:tempInt]
    documentId = int(tempString);

    #for documentId in range(1, documentNumber+1):  ## In case for looking all files and build cosine matrixes for all txt files 

    first = 0
    second = 0
    third = 0

    resultString = ""

    for i in range(len(newsDocuments[documentId])):

        resultString = resultString + str(lexRankScores[documentId][i]) + " "
        if lexRankScores[documentId][i] > lexRankScores[documentId][first]:
            third = second
            second = first
            first = i
        elif lexRankScores[documentId][i] > lexRankScores[documentId][second]:
            third = second
            second = i
        elif lexRankScores[documentId][i] > lexRankScores[documentId][third]:
            third = i

    string = newsDocuments[documentId][first] + "\n" + newsDocuments[documentId][second] + "\n" + newsDocuments[documentId][third] #string going to be written on txt
    mySummaryText[documentId] = string # 3 sentences as a dictionary

    ########## Sentences in case we want to see them ###########
    """print("\nMy Summary")
    string = "1st sentence: " + newsDocuments[documentId][first]
    print(string)
    string = "2nd sentence" + newsDocuments[documentId][second]
    print(string)
    string = "3rd sentence: " + newsDocuments[documentId][third]
    print(string)"""
    
    print(resultString)  # Printing the result



## method for calculating rouge score. Only for report
def calculateRogueScores(fileName):

    global documentNumber, mySummaryText, summaryDocuments

    resultString = ""

    tempString = fileName
    tempInt =  fileName.index(".")
    tempString = tempString[0:tempInt]
    #documentId = int(tempString);


    mySummary = []
    goldSummary = []

    mySummary.append("My Summaries")
    goldSummary.append("Gold Summaries")

    if os.path.exists("seperated"):
        os.chdir(os.getcwd() + "/seperated")
    else:
        os.mkdir("seperated")
        os.chdir(os.getcwd() + "/seperated")
        

    for documentId in range(1, documentNumber+1):
        string = "mySummary" + str(documentId) + ".txt"
        mySummary.append(open(string, "w+"))
        mySummary[documentId].write(mySummaryText[documentId])
        mySummary[documentId].close()
        string = "goldSummary" + str(documentId) + ".txt"
        goldSummary.append(open(string, "w+"))
        for  i in range(len(summaryDocuments[documentId])):
            goldSummary[documentId].write(summaryDocuments[documentId][i] + "\n")
        if i == 1:
            goldSummary[documentId].write(" " + "\n")

        goldSummary[documentId].close()

        results = subprocess.Popen("rouge -f ./mySummary" + str(documentId) + ".txt ./goldSummary" + str(documentId) + ".txt --avg", shell=True,  stdout=subprocess.PIPE)

        try:
            jsonObject = json.loads(results.communicate()[0])
            rouge1 = jsonObject['rouge-1']['f']
            rouge2 = jsonObject['rouge-2']['f']
            rougel = jsonObject['rouge-l']['f']

            resultString = resultString + str(documentId) + " " + str(rouge1) + " " + str(rouge2) + " " + str(rougel) + "\n"

        except ValueError:
            resultString = resultString + str(documentId) + "---" + "\n" 


    resultFile = open("resultFile", "w+")
    resultFile.write(resultString)
    resultFile.close()


if len(sys.argv) != 3:
    print("\nInvalid number of parameters! Ex:python info_retrival.py /home/barin/Downloads/cmpe493/project3/Dataset/ 1.txt\n")


path = sys.argv[1]
fileName = sys.argv[2]

os.chdir(path)


tokenizer()
calculateTfxIdf()
buildGraph(fileName)
powerMethod(fileName)
output(fileName)
#calculateRogueScores(fileName)  ## Not working only for report