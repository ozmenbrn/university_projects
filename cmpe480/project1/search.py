#!/usr/bin/python

import os, sys, math
import Queue
from decimal import *


coordinates = [] #Start and End coordinates, list turns into variables later
map2D = []  #Map in 2D list
discoveredMatrix = []

isMountain = False

startX = -1
startY = -1
endX = -1
endY = -1

def readInput():

	global coordinates, map2D, startX, startY, endX, endY, discoveredMatrix, isMountain

	#print(sys.argv[1])
	openFile = open(sys.argv[1], "r")
	#openFile = open("data/examples/labyrinth/labyrinth_example_1.txt", "r")
	#openFile = open("data/examples/mountain/mountain_example_1.txt", "r")
	
	file = openFile.read()

	count = 0
	counter = 0
	isStartedMap = False	#First coordinates, then map

	tempStr = ""

	while(isStartedMap == False):
		if(file[count] != " " and file[count] != "\n" and file[count] != "\r"):
			tempStr = tempStr + file[count]		#Add number if it is 2 digit number or float number
		else:
			if(len(tempStr) != 0):
				coordinates.append(tempStr)
				tempStr = ""
		count = count + 1

		if(len(coordinates) == 4):	#End and start coordinates total
			isStartedMap = True 	#After giving coordinates, decide whether to start map

	#count = count + 1  #Skips the first \n
	if(file[count] == "\n"):
		count = count + 1

	map2D.append([]) #First row
	discoveredMatrix.append([])

	weightCounter = 0

	tempStr = ""

	for i in range(count, len(file)):
		if(file[i] != " " and file[i] != "\n" and file[i] != "\r"): #Skip the unneccesary symbols
			tempStr = tempStr + file[i]	#Check if it is float number or not, if so, add and build a double. 
			if(file[i] == "."):
				isMountain = True

		else:	
			if(len(tempStr) != 0):
				discoveredMatrix[counter].append(0)
				weightCounter = weightCounter + 1
				if(isMountain):
					map2D[counter].append(float(tempStr))	#Add the element to the map
					tempStr = ""
				else:
					map2D[counter].append(int(tempStr))	#Add the element to the map
					tempStr = ""

		if(file[i] == "\n"):	#If it is end of the row, then open one more row and increment it to this row.
			isFirstRow = False
			map2D.append([])
			discoveredMatrix.append([])
			counter = counter + 1
		
	if(len(map2D[counter]) == 0): #In case input data ends with \n
		map2D.pop(counter)
		discoveredMatrix.pop(counter)

	else:
		discoveredMatrix[counter].append(0)
		if(isMountain):
			map2D[counter].append(float(tempStr))
		else:
			map2D[counter].append(int(tempStr))

	#print(coordinates)
	#print(map2D)

	startX = int(coordinates[0])
	startY = int(coordinates[1])
	endX = int(coordinates[2])
	endY = int(coordinates[3])

	openFile.close()



def implementDFS():
	global map2D, startY, startX, endX, endY, discoveredMatrix, isMountain

	stack = []
	stack.append([startX, startY])
	discoveredMatrix[startX][startY] = 1

	dictComeFrom = dict()

	euclideanDist = 0.0

	#dictComeFrom[1,1] = [2, 1]
	#dictComeFrom[1,12] = [3, 2]

	#print(dictComeFrom)

	currentX = startX
	currentY = startY

	isFinished = False

	while(isFinished == False and len(stack) != 0):

		if(isMountain == False):	#That means we are looking labyrinth
			if(currentX - 1 >= 0): #Check whether we are on the top side of the board
				if(map2D[currentX - 1][currentY] != 1 and discoveredMatrix[currentX - 1][currentY] != 1): #That means if upside is not wall
					stack.append([currentX - 1, currentY])
					if(currentX - 1, currentY) not in dictComeFrom:
						dictComeFrom[currentX - 1, currentY] = [currentX, currentY]
			if(currentY - 1 >= 0):
				if(map2D[currentX][currentY - 1] != 1 and discoveredMatrix[currentX][currentY - 1] != 1):
					stack.append([currentX, currentY - 1])
					if(currentX, currentY - 1) not in dictComeFrom:
						dictComeFrom[currentX, currentY - 1] = [currentX, currentY]
			if(currentY + 1 < len(map2D[0])):
				if(map2D[currentX][currentY + 1] != 1 and discoveredMatrix[currentX][currentY + 1] != 1):
					stack.append([currentX, currentY + 1])
					if(currentX, currentY + 1) not in dictComeFrom:
						dictComeFrom[currentX, currentY + 1] = [currentX, currentY]
			if(currentX + 1 < len(map2D)):
				if(map2D[currentX + 1][currentY] != 1 and discoveredMatrix[currentX + 1][currentY] != 1):
					stack.append([currentX + 1, currentY])
					if(currentX + 1, currentY) not in dictComeFrom:
						dictComeFrom[currentX + 1, currentY] = [currentX, currentY]
		
		else:      #That means we are looking mountain
			if(currentX - 1 >= 0): #Check whether we are on the top side of the board
				if(abs(map2D[currentX - 1][currentY] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX - 1][currentY] != 1): #That means if upside is not wall
					stack.append([currentX - 1, currentY])
					if(currentX - 1, currentY) not in dictComeFrom:
						dictComeFrom[currentX - 1, currentY] = [currentX, currentY]
			if(currentY - 1 >= 0):
				if(abs(map2D[currentX][currentY - 1] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX][currentY - 1] != 1):
					stack.append([currentX, currentY - 1])
					if(currentX, currentY - 1) not in dictComeFrom:
						dictComeFrom[currentX, currentY - 1] = [currentX, currentY]
			if(currentY + 1 < len(map2D[0])):
				if(abs(map2D[currentX][currentY + 1] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX][currentY + 1] != 1):
					stack.append([currentX, currentY + 1])
					if(currentX, currentY + 1) not in dictComeFrom:
						dictComeFrom[currentX, currentY + 1] = [currentX, currentY]
			if(currentX + 1 < len(map2D)):
				if(abs(map2D[currentX + 1][currentY] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX + 1][currentY] != 1):
					stack.append([currentX + 1, currentY])
					if(currentX + 1, currentY) not in dictComeFrom:
						dictComeFrom[currentX + 1, currentY] = [currentX, currentY]

		#if (stack[len(stack)-1][0], stack[len(stack)-1][1]) not in dictComeFrom:
		#	dictComeFrom[stack[len(stack)-1][0], stack[len(stack)-1][1]] = [currentX, currentY]

		currentX = stack[len(stack)-1][0]
		currentY = stack[len(stack)-1][1]
		discoveredMatrix[currentX][currentY] = 1

		#print(stack)

		if(stack[len(stack)-1][0] == endX and stack[len(stack)-1][1] == endY):
			isFinished = True
		else:
			stack.pop()


	#for i in range(0, len(map2D)):
	#	print(map2D[i])

	#print("---------------")

	lastElement = sys.argv[1].split("/")[-1]
	fileName = lastElement.split(".")
	#print(sys.argv[1].split("."))

	fileWrite = open(fileName[0] + "_dfs_out.txt","w") 

	for i in range(0, len(map2D)):
		for j in range(0, len(map2D[i])):
			fileWrite.write(str(discoveredMatrix[i][j]))
			if(j == len(map2D[i]) - 1):
				fileWrite.write("\n")
			else:
				fileWrite.write(" ")
		
		#fileWrite.write(discoveredMatrix[i])
		#print(discoveredMatrix[i])
	#print(stack)
	#print(discoveredMatrix)
	#print(map2D)
	#print(dictComeFrom)

	backtraceStack = []

	currentX = endX
	currentY = endY

	backtraceStack.append([currentX, currentY])

	isBacktraceFinished = False

	while(isBacktraceFinished == False):

		tempX = currentX
		tempY = currentY

		currentX = dictComeFrom[tempX, tempY][0]
		currentY = dictComeFrom[tempX, tempY][1]

		euclideanDist = euclideanDist + math.sqrt((tempX - currentX) * (tempX - currentX) + (tempY - currentY) * (tempY - currentY) + 
			(map2D[currentX][currentY] - map2D[tempX][tempY]) * (map2D[currentX][currentY] - map2D[tempX][tempY]))

		backtraceStack.append([currentX, currentY])

		if(currentX == startX and currentY == startY):
			isBacktraceFinished = True

	
	pathLength = len(backtraceStack)

	#print(pathLength)  # Length of the path from start point to end point
	fileWrite.write(str(pathLength))
	fileWrite.write("\n")

	while(len(backtraceStack) > 0):
		#print(backtraceStack[-1])
		fileWrite.write(str(backtraceStack[-1][0]))
		fileWrite.write(" ")
		fileWrite.write(str(backtraceStack[-1][1]))
		fileWrite.write("\n")
		backtraceStack.pop()

	twoDigit = Decimal(10) ** -2

	if(isMountain == False):	
		pathLength = pathLength - 1
		pathLength =  Decimal(pathLength).quantize(twoDigit)
		fileWrite.write(str(pathLength))
		#print(pathLength)
	else:
		euclideanDist = Decimal(euclideanDist).quantize(twoDigit)
		fileWrite.write(str(euclideanDist))
		#print(euclideanDist)


	#fileWrite.write("Hello World") 

	fileWrite.close() 



def implementBFS():

	global map2D, startY, startX, endX, endY, discoveredMatrix, isMountain

	queue = Queue.Queue()

	queue.put([startX, startY])

	discoveredMatrix[startX][startY] = 1

	dictComeFrom = dict()

	euclideanDist = 0.0

	#dictComeFrom[1,1] = [2, 1]
	#dictComeFrom[1,12] = [3, 2]

	#print(dictComeFrom)

	currentX = startX
	currentY = startY

	isFinished = False

	while(isFinished == False and not queue.empty()):

		if(isMountain == False):	#That means we are looking labyrinth

			if(currentX + 1 < len(map2D)):
				if(map2D[currentX + 1][currentY] != 1 and discoveredMatrix[currentX + 1][currentY] != 1):
					queue.put([currentX + 1, currentY])
					if(currentX + 1, currentY) not in dictComeFrom:
						dictComeFrom[currentX + 1, currentY] = [currentX, currentY]

			if(currentY + 1 < len(map2D[0])):
				if(map2D[currentX][currentY + 1] != 1 and discoveredMatrix[currentX][currentY + 1] != 1):
					queue.put([currentX, currentY + 1])
					if(currentX, currentY + 1) not in dictComeFrom:
						dictComeFrom[currentX, currentY + 1] = [currentX, currentY]

			if(currentY - 1 >= 0):
				if(map2D[currentX][currentY - 1] != 1 and discoveredMatrix[currentX][currentY - 1] != 1):
					queue.put([currentX, currentY - 1])
					if(currentX, currentY - 1) not in dictComeFrom:
						dictComeFrom[currentX, currentY - 1] = [currentX, currentY]

			if(currentX - 1 >= 0): #Check whether we are on the top side of the board
				if(map2D[currentX - 1][currentY] != 1 and discoveredMatrix[currentX - 1][currentY] != 1): #That means if upside is not wall
					queue.put([currentX - 1, currentY])
					if(currentX - 1, currentY) not in dictComeFrom:
						dictComeFrom[currentX - 1, currentY] = [currentX, currentY]
		
		else:      #That means we are looking mountain
			if(currentX + 1 < len(map2D)):
				if(abs(map2D[currentX + 1][currentY] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX + 1][currentY] != 1):
					queue.put([currentX + 1, currentY])
					if(currentX + 1, currentY) not in dictComeFrom:
						dictComeFrom[currentX + 1, currentY] = [currentX, currentY]

			if(currentY + 1 < len(map2D[0])):
				if(abs(map2D[currentX][currentY + 1] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX][currentY + 1] != 1):
					queue.put([currentX, currentY + 1])
					if(currentX, currentY + 1) not in dictComeFrom:
						dictComeFrom[currentX, currentY + 1] = [currentX, currentY]

			if(currentY - 1 >= 0):
				if(abs(map2D[currentX][currentY - 1] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX][currentY - 1] != 1):
					queue.put([currentX, currentY - 1])
					if(currentX, currentY - 1) not in dictComeFrom:
						dictComeFrom[currentX, currentY - 1] = [currentX, currentY]

			if(currentX - 1 >= 0): #Check whether we are on the top side of the board
				if(abs(map2D[currentX - 1][currentY] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX - 1][currentY] != 1): #That means if upside is not wall
					queue.put([currentX - 1, currentY])
					if(currentX - 1, currentY) not in dictComeFrom:
						dictComeFrom[currentX - 1, currentY] = [currentX, currentY]
			
			
			

		#if (stack[len(stack)-1][0], stack[len(stack)-1][1]) not in dictComeFrom:
		#	dictComeFrom[stack[len(stack)-1][0], stack[len(stack)-1][1]] = [currentX, currentY]

		firstElement = queue.get()
		currentX = firstElement[0]
		currentY = firstElement[1]
		discoveredMatrix[currentX][currentY] = 1

		#print(stack)

		if(currentX == endX and currentY == endY):
			isFinished = True


	#for i in range(0, len(map2D)):
	#	print(map2D[i])

	#print("---------------")

	lastElement = sys.argv[1].split("/")[-1]
	fileName = lastElement.split(".")
	#print(sys.argv[1].split("."))

	fileWrite = open(fileName[0] + "_bfs_out.txt","w") 

	for i in range(0, len(map2D)):
		for j in range(0, len(map2D[i])):
			fileWrite.write(str(discoveredMatrix[i][j]))
			if(j == len(map2D[i]) - 1):
				fileWrite.write("\n")
			else:
				fileWrite.write(" ")
		
		#fileWrite.write(discoveredMatrix[i])
		#print(discoveredMatrix[i])
	#print(stack)
	#print(discoveredMatrix)
	#print(map2D)
	#print(dictComeFrom)

	backtraceStack = []

	currentX = endX
	currentY = endY

	backtraceStack.append([currentX, currentY])

	isBacktraceFinished = False

	while(isBacktraceFinished == False):

		tempX = currentX
		tempY = currentY

		currentX = dictComeFrom[tempX, tempY][0]
		currentY = dictComeFrom[tempX, tempY][1]

		euclideanDist = euclideanDist + math.sqrt((tempX - currentX) * (tempX - currentX) + (tempY - currentY) * (tempY - currentY) + 
			(map2D[currentX][currentY] - map2D[tempX][tempY]) * (map2D[currentX][currentY] - map2D[tempX][tempY]))

		backtraceStack.append([currentX, currentY])

		if(currentX == startX and currentY == startY):
			isBacktraceFinished = True

	
	pathLength = len(backtraceStack)

	#print(pathLength)  # Length of the path from start point to end point
	fileWrite.write(str(pathLength))
	fileWrite.write("\n")

	while(len(backtraceStack) > 0):
		#print(backtraceStack[-1])
		fileWrite.write(str(backtraceStack[-1][0]))
		fileWrite.write(" ")
		fileWrite.write(str(backtraceStack[-1][1]))
		fileWrite.write("\n")
		backtraceStack.pop()

	twoDigit = Decimal(10) ** -2

	if(isMountain == False):	
		pathLength = pathLength - 1
		pathLength =  Decimal(pathLength).quantize(twoDigit)
		fileWrite.write(str(pathLength))
		#print(pathLength)
	else:
		euclideanDist = Decimal(euclideanDist).quantize(twoDigit)
		fileWrite.write(str(euclideanDist))
		#print(euclideanDist)


	#fileWrite.write("Hello World") 

	fileWrite.close() 




def implementAstar():

	global map2D, startY, startX, endX, endY, discoveredMatrix, isMountain

	discoveredMatrix[startX][startY] = 1

	dictComeFrom = dict()
	gValues = dict()
	fValues = dict()
	hValues = dict()

	euclideanDist = 0.0

	currentX = startX
	currentY = startY


	gValues[startX, startY] = 0
	hValues[startX, startY] = math.sqrt((endY - startY) * (endY - startY) + (endX - startX) * (endX - startX))
	fValues[startX, startY] = gValues[startX, startY] + hValues[startX, startY]

	isFinished = False

	while(isFinished == False):

		if(isMountain == False):	#That means we are looking labyrinth

			if(currentX + 1 < len(map2D)): #Goes downward
				if(map2D[currentX + 1][currentY] != 1 and discoveredMatrix[currentX + 1][currentY] != 1):				
					if(currentX + 1, currentY) in gValues:
						if(gValues[currentX + 1, currentY] > gValues[currentX, currentY] + 1):
							gValues[currentX + 1, currentY] = gValues[currentX, currentY] + 1
							hValues[currentX + 1, currentY] = abs(endX - (currentX + 1)) + abs(endY - (currentY))
							#print(math.sqrt((endY - currentY) * (endY - currentY) + (endX - currentX + 1) * (endX - currentX + 1)))
							fValues[currentX + 1, currentY] = gValues[currentX + 1, currentY] + hValues[currentX + 1, currentY]

							#if(currentX + 1, currentY) not in dictComeFrom:
							dictComeFrom[currentX + 1, currentY] = [currentX, currentY]
					else:
						gValues[currentX + 1, currentY] = gValues[currentX, currentY] + 1
						hValues[currentX + 1, currentY] = abs(endX - (currentX + 1)) + abs(endY - (currentY))
						#print(math.sqrt((endY - currentY) * (endY - currentY) + (endX - currentX + 1) * (endX - currentX + 1)))
						fValues[currentX + 1, currentY] = gValues[currentX + 1, currentY] + hValues[currentX + 1, currentY]

						#if(currentX + 1, currentY) not in dictComeFrom:
						dictComeFrom[currentX + 1, currentY] = [currentX, currentY]

			if(currentY + 1 < len(map2D[0])): #Goes Right
				if(map2D[currentX][currentY + 1] != 1 and discoveredMatrix[currentX][currentY + 1] != 1):
					#if(currentX, currentY + 1) not in gValues:
					if(currentX, currentY + 1) in gValues:
						if(gValues[currentX, currentY + 1] > gValues[currentX, currentY] + 1):
							gValues[currentX, currentY + 1] = gValues[currentX, currentY] + 1
							hValues[currentX, currentY + 1] = abs(endX - (currentX)) + abs(endY - (currentY + 1))
							fValues[currentX, currentY + 1] = gValues[currentX, currentY + 1] + hValues[currentX, currentY + 1]

							#if(currentX, currentY + 1) not in dictComeFrom:
							dictComeFrom[currentX, currentY + 1] = [currentX, currentY]	
					else:
						gValues[currentX, currentY + 1] = gValues[currentX, currentY] + 1
						hValues[currentX, currentY + 1] = abs(endX - (currentX)) + abs(endY - (currentY + 1))
						fValues[currentX, currentY + 1] = gValues[currentX, currentY + 1] + hValues[currentX, currentY + 1]

						#if(currentX, currentY + 1) not in dictComeFrom:
						dictComeFrom[currentX, currentY + 1] = [currentX, currentY]					

			if(currentY - 1 >= 0):  #Goes Left
				if(map2D[currentX][currentY - 1] != 1 and discoveredMatrix[currentX][currentY - 1] != 1):	
					if(currentX, currentY - 1) in gValues:
						if(gValues[currentX, currentY - 1] > gValues[currentX, currentY] + 1):			
					#if(currentX, currentY - 1) not in gValues:
							gValues[currentX, currentY - 1] = gValues[currentX, currentY] + 1
							hValues[currentX, currentY - 1] = abs(endX - (currentX)) + abs(endY - (currentY - 1))
							fValues[currentX, currentY - 1] = gValues[currentX, currentY - 1] + hValues[currentX, currentY - 1]

							#if(currentX, currentY - 1) not in dictComeFrom:
							dictComeFrom[currentX, currentY - 1] = [currentX, currentY]
					else:
						gValues[currentX, currentY - 1] = gValues[currentX, currentY] + 1
						hValues[currentX, currentY - 1] = abs(endX - (currentX)) + abs(endY - (currentY - 1))
						fValues[currentX, currentY - 1] = gValues[currentX, currentY - 1] + hValues[currentX, currentY - 1]

						#if(currentX, currentY - 1) not in dictComeFrom:
						dictComeFrom[currentX, currentY - 1] = [currentX, currentY]
					

			if(currentX - 1 >= 0): #Check whether we are on the top side of the board
				if(map2D[currentX - 1][currentY] != 1 and discoveredMatrix[currentX - 1][currentY] != 1): #That means if upside is not wall
					#if(currentX - 1, currentY) not in gValues:
					if(currentX - 1, currentY) in gValues:
						if(gValues[currentX - 1, currentY] > gValues[currentX, currentY] + 1):	
							gValues[currentX - 1, currentY] = gValues[currentX, currentY] + 1
							hValues[currentX - 1, currentY] = abs(endX - (currentX - 1)) + abs(endY - (currentY))
							fValues[currentX - 1, currentY] = gValues[currentX - 1, currentY] + hValues[currentX - 1, currentY]

							#if(currentX - 1, currentY) not in dictComeFrom:
							dictComeFrom[currentX - 1, currentY] = [currentX, currentY]	
					else:
						gValues[currentX - 1, currentY] = gValues[currentX, currentY] + 1
						hValues[currentX - 1, currentY] = abs(endX - (currentX - 1)) + abs(endY - (currentY))
						fValues[currentX - 1, currentY] = gValues[currentX - 1, currentY] + hValues[currentX - 1, currentY]

						#if(currentX - 1, currentY) not in dictComeFrom:
						dictComeFrom[currentX - 1, currentY] = [currentX, currentY]	
					
		
		else:      #That means we are looking mountain
			if(currentX + 1 < len(map2D)):
				if(abs(map2D[currentX + 1][currentY] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX + 1][currentY] != 1):
					if(currentX + 1, currentY) in gValues:
						if(gValues[currentX + 1, currentY] > (gValues[currentX, currentY] + math.sqrt((currentY - currentY) * (currentY - currentY) + (currentX - (currentX + 1)) * (currentX - (currentX + 1)) + (map2D[currentX][currentY] - map2D[currentX + 1][currentY]) * (map2D[currentX][currentY] - map2D[currentX + 1][currentY])))):
							gValues[currentX + 1, currentY] = gValues[currentX, currentY] + math.sqrt((currentY - currentY) * (currentY - currentY) + (currentX - (currentX + 1)) * (currentX - (currentX + 1)) + (map2D[currentX][currentY] - map2D[currentX + 1][currentY]) * (map2D[currentX][currentY] - map2D[currentX + 1][currentY]))
							hValues[currentX + 1, currentY] = math.sqrt((endY - currentY) * (endY - currentY) + (endX - (currentX + 1)) * (endX - (currentX + 1)) + (map2D[endX][endY] - map2D[currentX + 1][currentY]) * (map2D[endX][endY] - map2D[currentX + 1][currentY]))
							fValues[currentX + 1, currentY] = gValues[currentX + 1, currentY] + hValues[currentX + 1, currentY]
							dictComeFrom[currentX + 1, currentY] = [currentX, currentY]
					else:
						gValues[currentX + 1, currentY] = gValues[currentX, currentY] + math.sqrt((currentY - currentY) * (currentY - currentY) + (currentX - (currentX + 1)) * (currentX - (currentX + 1)) + (map2D[currentX][currentY] - map2D[currentX + 1][currentY]) * (map2D[currentX][currentY] - map2D[currentX + 1][currentY]))
						hValues[currentX + 1, currentY] = math.sqrt((endY - currentY) * (endY - currentY) + (endX - (currentX + 1)) * (endX - (currentX + 1)) + (map2D[endX][endY] - map2D[currentX + 1][currentY]) * (map2D[endX][endY] - map2D[currentX + 1][currentY]))
						fValues[currentX + 1, currentY] = gValues[currentX + 1, currentY] + hValues[currentX + 1, currentY]
						dictComeFrom[currentX + 1, currentY] = [currentX, currentY]

			if(currentY + 1 < len(map2D[0])):
				if(abs(map2D[currentX][currentY + 1] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX][currentY + 1] != 1):
					if(currentX, currentY + 1) in gValues:
						if(gValues[currentX, currentY + 1] > (gValues[currentX, currentY] + math.sqrt((currentY - (currentY + 1)) * (currentY - (currentY + 1)) + (currentX - currentX) * (currentX - currentX) + (map2D[currentX][currentY] - map2D[currentX][currentY + 1]) * (map2D[currentX][currentY] - map2D[currentX][currentY + 1])))):
							gValues[currentX, currentY + 1] = gValues[currentX, currentY] + math.sqrt((currentY - (currentY + 1)) * (currentY - (currentY + 1)) + (currentX - currentX) * (currentX - currentX) + (map2D[currentX][currentY] - map2D[currentX][currentY + 1]) * (map2D[currentX][currentY] - map2D[currentX][currentY + 1]))
							hValues[currentX, currentY + 1] = math.sqrt((endY - (currentY + 1)) * (endY - (currentY + 1)) + (endX - currentX) * (endX - currentX) + (map2D[endX][endY] - map2D[currentX][currentY + 1]) * (map2D[endX][endY] - map2D[currentX][currentY + 1]))
							fValues[currentX, currentY + 1] = gValues[currentX, currentY + 1] + hValues[currentX, currentY + 1]
							dictComeFrom[currentX, currentY + 1] = [currentX, currentY]
					else:
						gValues[currentX, currentY + 1] = gValues[currentX, currentY] + math.sqrt((currentY - (currentY + 1)) * (currentY - (currentY + 1)) + (currentX - currentX) * (currentX - currentX) + (map2D[currentX][currentY] - map2D[currentX][currentY + 1]) * (map2D[currentX][currentY] - map2D[currentX][currentY + 1]))
						hValues[currentX, currentY + 1] = math.sqrt((endY - (currentY + 1)) * (endY - (currentY + 1)) + (endX - currentX) * (endX - currentX) + (map2D[endX][endY] - map2D[currentX][currentY + 1]) * (map2D[endX][endY] - map2D[currentX][currentY + 1]))
						fValues[currentX, currentY + 1] = gValues[currentX, currentY + 1] + hValues[currentX, currentY + 1]
						dictComeFrom[currentX, currentY + 1] = [currentX, currentY]

			if(currentY - 1 >= 0):
				if(abs(map2D[currentX][currentY - 1] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX][currentY - 1] != 1):
					if(currentX, currentY - 1) in gValues:
						if(gValues[currentX, currentY - 1] > (gValues[currentX, currentY] + 1 + (map2D[currentX][currentY - 1] - map2D[currentX][currentY]) * (map2D[currentX][currentY - 1] - map2D[currentX][currentY]))):
							gValues[currentX, currentY - 1] = gValues[currentX, currentY] + 1 + (map2D[currentX][currentY - 1] - map2D[currentX][currentY]) * (map2D[currentX][currentY - 1] - map2D[currentX][currentY])
							hValues[currentX, currentY - 1] = math.sqrt((endY - (currentY - 1)) * (endY - (currentY - 1)) + (endX - currentX) * (endX - currentX) + (map2D[endX][endY] - map2D[currentX][currentY - 1]) * (map2D[endX][endY] - map2D[currentX][currentY - 1]))
							fValues[currentX, currentY - 1] = gValues[currentX, currentY - 1] + hValues[currentX, currentY - 1]
							dictComeFrom[currentX, currentY - 1] = [currentX, currentY]
					else:
						gValues[currentX, currentY - 1] = gValues[currentX, currentY] + 1 + (map2D[currentX][currentY - 1] - map2D[currentX][currentY]) * (map2D[currentX][currentY - 1] - map2D[currentX][currentY])
						hValues[currentX, currentY - 1] = math.sqrt((endY - (currentY - 1)) * (endY - (currentY - 1)) + (endX - currentX) * (endX - currentX) + (map2D[endX][endY] - map2D[currentX][currentY - 1]) * (map2D[endX][endY] - map2D[currentX][currentY - 1]))
						fValues[currentX, currentY - 1] = gValues[currentX, currentY - 1] + hValues[currentX, currentY - 1]
						dictComeFrom[currentX, currentY - 1] = [currentX, currentY]

			if(currentX - 1 >= 0): #Check whether we are on the top side of the board
				if(abs(map2D[currentX - 1][currentY] - map2D[currentX][currentY]) <= 1 and discoveredMatrix[currentX - 1][currentY] != 1): #That means if upside is not wall
					if(currentX - 1, currentY) in gValues:
						if(gValues[currentX - 1, currentY] > (gValues[currentX, currentY] + 1 + (map2D[currentX - 1][currentY] - map2D[currentX][currentY]) * (map2D[currentX - 1][currentY] - map2D[currentX][currentY]))):
							gValues[currentX - 1, currentY] = gValues[currentX, currentY] + 1 + (map2D[currentX - 1][currentY] - map2D[currentX][currentY]) *(map2D[currentX - 1][currentY] - map2D[currentX][currentY])
							hValues[currentX - 1, currentY] = math.sqrt((endY - currentY) * (endY - currentY) + (endX - (currentX - 1)) * (endX - (currentX - 1)) + (map2D[endX][endY] - map2D[currentX - 1][currentY]) * (map2D[endX][endY] - map2D[currentX - 1][currentY]))
							fValues[currentX - 1, currentY] = gValues[currentX - 1, currentY] + hValues[currentX - 1, currentY]
							dictComeFrom[currentX - 1, currentY] = [currentX, currentY]
					else:
						gValues[currentX - 1, currentY] = gValues[currentX, currentY] + 1 + (map2D[currentX - 1][currentY] - map2D[currentX][currentY]) * (map2D[currentX - 1][currentY] - map2D[currentX][currentY])
						hValues[currentX - 1, currentY] = math.sqrt((endY - currentY) * (endY - currentY) + (endX - (currentX - 1)) * (endX - (currentX - 1)) + (map2D[endX][endY] - map2D[currentX - 1][currentY]) * (map2D[endX][endY] - map2D[currentX - 1][currentY]))
						fValues[currentX - 1, currentY] = gValues[currentX - 1, currentY] + hValues[currentX - 1, currentY]
						dictComeFrom[currentX - 1, currentY] = [currentX, currentY]
			
			
		#print("Current Node: ", currentX,currentY)
		#print("g(n) => ", gValues[currentX, currentY])
		#print("h(n) => ", hValues[currentX, currentY])
		#print("f(n) => ", fValues[currentX, currentY])
		#print("-----------------------")

		
		#for values in fValues.values():
		#	print(values)
		tempFValue = 99999
		tempX = -1
		tempY = -1
		for keys in fValues:
			if(discoveredMatrix[keys[0]][keys[1]] != 1):
				if(fValues[keys] < tempFValue):
					tempFValue = fValues[keys]
					tempX = keys[0]
					tempY = keys[1]
				elif(fValues[keys] == tempFValue):
					if((keys[0]*len(map2D[0]) + keys[1]) > tempX*len(map2D[0]) + tempY):
						tempX = keys[0]
						tempY = keys[1]


		currentX = tempX
		currentY = tempY

		if(currentX == -1 or currentY == -1):
			isFinished = True
		else:
			discoveredMatrix[currentX][currentY] = 1


		#firstElement = queue.get()
		#currentX = firstElement[0]
		#currentY = firstElement[1]

		#print(stack)

		if(currentX == endX and currentY == endY):
			isFinished = True


	#for i in range(0, len(map2D)):
	#	print(map2D[i])

	#print("---------------")

	lastElement = sys.argv[1].split("/")[-1]
	fileName = lastElement.split(".")
	#print(sys.argv[1].split("."))

	fileWrite = open(fileName[0] + "_a_star_out.txt","w") 

	for i in range(0, len(map2D)):
		for j in range(0, len(map2D[i])):
			fileWrite.write(str(discoveredMatrix[i][j]))
			if(j == len(map2D[i]) - 1):
				fileWrite.write("\n")
			else:
				fileWrite.write(" ")
		
		#fileWrite.write(discoveredMatrix[i])
		##print(discoveredMatrix[i])
	#print(stack)
	#print(discoveredMatrix)
	#print(map2D)
	#print(dictComeFrom)

	backtraceStack = []

	currentX = endX
	currentY = endY

	backtraceStack.append([currentX, currentY])

	isBacktraceFinished = False

	while(isBacktraceFinished == False):

		tempX = currentX
		tempY = currentY

		currentX = dictComeFrom[tempX, tempY][0]
		currentY = dictComeFrom[tempX, tempY][1]

		euclideanDist = euclideanDist + math.sqrt((tempX - currentX) * (tempX - currentX) + (tempY - currentY) * (tempY - currentY) + 
			(map2D[currentX][currentY] - map2D[tempX][tempY]) * (map2D[currentX][currentY] - map2D[tempX][tempY]))

		backtraceStack.append([currentX, currentY])

		if(currentX == startX and currentY == startY):
			isBacktraceFinished = True

	
	pathLength = len(backtraceStack)

	#print(pathLength)  # Length of the path from start point to end point
	fileWrite.write(str(pathLength))
	fileWrite.write("\n")

	while(len(backtraceStack) > 0):
		#print(backtraceStack[-1])
		fileWrite.write(str(backtraceStack[-1][0]))
		fileWrite.write(" ")
		fileWrite.write(str(backtraceStack[-1][1]))
		fileWrite.write("\n")
		backtraceStack.pop()

	twoDigit = Decimal(10) ** -2

	if(isMountain == False):	
		pathLength = pathLength - 1
		pathLength =  Decimal(pathLength).quantize(twoDigit)
		fileWrite.write(str(pathLength))
		#print(pathLength)
	else:
		euclideanDist = Decimal(euclideanDist).quantize(twoDigit)
		fileWrite.write(str(euclideanDist))
		#print(euclideanDist)


	#fileWrite.write("Hello World") 

	fileWrite.close() 

	#print("ComeFrom: ", dictComeFrom)



def main():
	global coordinates, map2D, startY, startX, endX, endY, discoveredMatrix

	readInput()

	implementDFS()

	discoveredMatrix = []
	map2D = []
	coordinates = []

	#print(">>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<")

	readInput()

	implementBFS()

	discoveredMatrix = []
	map2D = []
	coordinates = []

	#print(">>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<")

	readInput()

	implementAstar()

	print("Output files are created under search.py path")



main()




