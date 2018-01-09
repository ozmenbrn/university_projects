// $ lamboot                                      :for start MPI
// $ mpicc <file.c>                               :compiles
// $ mpirun -np <procNum> ./a.out <input.txt>     :int for procNum,  input.txt for toxenizied speech  

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define speech_merge_send_tag 10
#define speech_merge_recv_tag 11
#define splited_speech_send_tag 20
#define splited_speech_recv_tag 21
#define max_num_procs 4
#define speech_size 8000
#define splited_speech_size 4000


//Constructor Speech
struct Speech{
	
	int wordOcurance;
	char words[25];

} speech[speech_size], splited_speech[splited_speech_size], reduced_speech[speech_size];


//write file
void writeIntermediateFile(char *str, int numWords){

	int i;
	FILE *output = fopen(str, "w");

	if (output == NULL){
	    printf("Error opening file!\n");
	    exit(1);
	}

	for(i = 0; i < numWords; i++){
		fprintf(output, "%s, %i\n", speech[i].words, speech[i].wordOcurance);
	}

	fclose(output);
}

//write file
void writeReducedFile(char *str, int numWords){

	int i;
	FILE *output = fopen(str, "w");

	if (output == NULL){
	    printf("Error opening file!\n");
	    exit(1);
	}

	for(i = 0; i < numWords; i++){
		fprintf(output, "%s, %i\n", reduced_speech[i].words, reduced_speech[i].wordOcurance);
	}

	fclose(output);
}


//merged slaved inputs in root process
int addToSpeech(struct Speech *arr, int splited_size, int tempSize){

	int i;

	for(i = 0; i < splited_size; i++){
		strcpy(speech[tempSize].words, arr[i].words);
		speech[tempSize].wordOcurance = arr[i].wordOcurance;

		tempSize++;
	}
	return tempSize;
}


//Check the characters in each array element. In sort it compares STRING (char array)
int isBigger(struct Speech left, struct Speech right){

	int isTrue = 2, tempCount = 0;
	while(isTrue == 2){

		if(left.words[tempCount] == '\0'){
			isTrue = 1;
		}
		else if(right.words[tempCount] == '\0'){
			isTrue = 0;
		}	
		
		if(left.words[tempCount] > right.words[tempCount]){
			isTrue = 1;
			//printf("%s, %s\n", left.words, right.words);
		}
		else if(left.words[tempCount] < right.words[tempCount]){
			isTrue = 0;
		}

		tempCount++;
	} 

	return isTrue;
} 

//Sort speech both for slave and root processes.
//Finnaly this function helps also to make the last merge on root process
void sortSpeech(struct Speech *arr, int arraySize){

	int i, j;
	struct Speech temp;

	for(i = 0; i < arraySize-1; i++){
		for(j = 0; j < arraySize-i-1; j++){
			if(isBigger(arr[j], arr[j+1]) == 1){
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}
}

//check which one small through word occurances to bubble sort the reduced file
int isSmallerCount(struct Speech left, struct Speech right){

	if(left.wordOcurance < right.wordOcurance){
		return 1;
	}
	else{
		return 0;
	}

}


//sorting algorithm for reduced speech
void sortReducedSpeech(struct Speech *arr, int arrSize){

	int i, j;
	struct Speech temp;

	for(i = 0; i < arrSize-1; i++){
		for(j = 0; j < arrSize-i-1; j++){
			if(isSmallerCount(arr[j], arr[j+1]) == 1){
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}

}

//reduces the repetitive words and update their word occurances, then 
//write it onto reducedFile.txt
void reduceSpeech(struct Speech *arr, int arraySize){

	int cur = 0, i;

	reduced_speech[cur] = arr[cur];

	//check if there are repetitive words
	for(i = 1; i < arraySize; i++){
		if(strcmp(arr[i].words, arr[i-1].words) != 0){
			cur++;
			reduced_speech[cur] = arr[i];
		}
		else{
			reduced_speech[cur].wordOcurance++;
		}

	}

	cur++;
	sortReducedSpeech(reduced_speech, cur);

	writeReducedFile("reducedFile.txt", cur);


}


int main(int argc, char **argv) 
{
	//initilaze variables.
  	int myRank, rc, numProcs, slave_id, root_process = 0, splited_size;
  	int tempSize = 0, i = 0, j = 0, numWords = 0, s;
  	int sendArray [speech_size], wordOccArray [splited_speech_size], *rbuf; 
  	char ch; 

  	MPI_Status status;	//To keep status of MPI send and recieve
  	MPI_Request req;   
  	FILE *input;  //Reed input file


  	//initialize MPI
  	rc = MPI_Init(&argc, &argv);  
  	rc = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  	rc = MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

  	//get the arv as an input file
  	if ((input = fopen(argv[1], "r")) == NULL) {
        if (myRank == 0) {
        	printf("%s: Couldn't open file %s\n", argv[0], argv[1]);
        }
        MPI_Finalize();
        exit(1);
    }

	if(myRank == root_process) {

		//Read the file
		while((ch=fgetc(input))!=EOF){
			if(ch == '\n'){
  				i++;
  				j = 0;
  				numWords++;
  			}
  			else{
  				speech[i].words[j] = ch;
  				j++;
  			}
  		}

  		fclose(input);

  		// Split and keep the line numbers to send other processes.
  		int wordSizePerProc[numProcs];
		for(i = 1; i < numProcs; i++){
			wordSizePerProc[i] = numWords/numProcs;	
			tempSize = tempSize + wordSizePerProc[i];	
		} i = 0;
		wordSizePerProc[i] = numWords - tempSize;

		//Send words to slaves to update the wordOccurance value
		tempSize = wordSizePerProc[root_process];
		for(slave_id = 1; slave_id < numProcs; slave_id++) {

			rc = MPI_Send( &wordSizePerProc[slave_id], 1, MPI_INT, slave_id, splited_speech_send_tag, MPI_COMM_WORLD);
			rc = MPI_Send( &speech[tempSize], wordSizePerProc[slave_id], MPI_LONG_DOUBLE_INT , slave_id, splited_speech_send_tag, MPI_COMM_WORLD);
			tempSize = tempSize + wordSizePerProc[slave_id];
	    }

	    //root process, the words which root process reads and update value.
	    for(i = 0; i < wordSizePerProc[root_process]; i++){
	    	speech[i].wordOcurance = 1;
	    }

	    //Reciave updated values from the slave processes and add it to speech array in root process.
	    tempSize = wordSizePerProc[root_process];
	    for(slave_id = 1; slave_id < numProcs; slave_id++) {

  	    	rc = MPI_Recv( &splited_size, 1, MPI_INT, slave_id, splited_speech_recv_tag, MPI_COMM_WORLD, &status);
	    	rc = MPI_Recv( &splited_speech, splited_size, MPI_LONG_DOUBLE_INT , slave_id, splited_speech_recv_tag, MPI_COMM_WORLD, &status);
	    	tempSize = addToSpeech(splited_speech, splited_size, tempSize);	//Function for gathering all arrays come from slave processes.
     	}

     	writeIntermediateFile("intermediateMappedFile.txt", numWords);	//Function for creating the first intermediate file.

     	//Split the input again and send them to slaves to be sorted. (Merger Sort).
	    tempSize = wordSizePerProc[root_process];
		for(slave_id = 1; slave_id < numProcs; slave_id++) {

			rc = MPI_Send( &wordSizePerProc[slave_id], 1, MPI_INT, slave_id, speech_merge_send_tag, MPI_COMM_WORLD);
			rc = MPI_Send( &speech[tempSize], wordSizePerProc[slave_id], MPI_LONG_DOUBLE_INT , slave_id, speech_merge_send_tag, MPI_COMM_WORLD);
			tempSize = tempSize + wordSizePerProc[slave_id];

	    }

	    sortSpeech(speech, wordSizePerProc[root_process]);  //Sort speech where rootProcess' part takes place

	    writeIntermediateFile("intermediateMergedFile.txt", numWords);

	    //Reciave sorted arrays from other processes to merge it in root process.
	    tempSize = wordSizePerProc[root_process];
	    for(slave_id = 1; slave_id < numProcs; slave_id++) {

  	    	rc = MPI_Recv( &splited_size, 1, MPI_INT, slave_id, speech_merge_recv_tag, MPI_COMM_WORLD, &status);
	    	rc = MPI_Recv( &splited_speech, splited_size, MPI_LONG_DOUBLE_INT , slave_id, speech_merge_recv_tag, MPI_COMM_WORLD, &status);
	    	tempSize = addToSpeech(splited_speech, splited_size, tempSize);	//Function for gathering all arrays come from slave processes.
     	}

     	sortSpeech(speech, numWords);	//Last merge on root process

     	writeIntermediateFile("lastMergedFile.txt", numWords);	//Function for second intermediate file

     	reduceSpeech(speech, numWords);	//Reduce the file and write it onto reducedFile.txt


  	} else {

  		//Reciave part of speech array into slave processes.
  	    rc = MPI_Recv( &splited_size, 1, MPI_INT, root_process, splited_speech_send_tag, MPI_COMM_WORLD, &status);
	    rc = MPI_Recv( &splited_speech, splited_size, MPI_LONG_DOUBLE_INT , root_process, splited_speech_send_tag, MPI_COMM_WORLD, &status);

	    //Slave processes to update the wordOccurance Value
	    for(s = 0; s < splited_size; s++){
	    	splited_speech[s].wordOcurance = 1;
	    }

	    //Send the mapped words to the root process.
	    rc = MPI_Send( &splited_size, 1, MPI_INT, root_process, splited_speech_recv_tag, MPI_COMM_WORLD);
		rc = MPI_Send( &splited_speech, splited_size, MPI_LONG_DOUBLE_INT , root_process, splited_speech_recv_tag, MPI_COMM_WORLD);

		//Reciave part of speech array into slave processes to do merge sort.
  	    rc = MPI_Recv( &splited_size, 1, MPI_INT, root_process, speech_merge_send_tag, MPI_COMM_WORLD, &status);
	    rc = MPI_Recv( &splited_speech, splited_size, MPI_LONG_DOUBLE_INT , root_process, speech_merge_send_tag, MPI_COMM_WORLD, &status);

	    sortSpeech(splited_speech, splited_size);	//Sort parts of speech where unqie for every process

	    //send sorted arrays to the root process for last merge.
	    rc = MPI_Send( &splited_size, 1, MPI_INT, root_process, speech_merge_recv_tag, MPI_COMM_WORLD);
		rc = MPI_Send( &splited_speech, splited_size, MPI_LONG_DOUBLE_INT , root_process, speech_merge_recv_tag, MPI_COMM_WORLD);

  	}

  	if(myRank == root_process){
  		printf("\nProgram Outputs: \n1) intermediateMappedFile: First output that gives mapped inputs \n");
  		printf("2) intermediateMergedFile: Each processes did their parts on sorting \n3) lastMergedFile: Last merge on root process\n"); 
  		printf("4) reducedFile: Last output which words are reduced and values updated\n\n");
  	}

  	printf(" ");

  	rc = MPI_Finalize();

  	return 0;

}