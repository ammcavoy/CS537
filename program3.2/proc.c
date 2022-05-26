////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: processCreation.c
// Other Files
// Semester:         CS 537 Fall 2018
//
// Author:          Adam McAvoy 
// Email:           @wisc.edu
// CS Login:        mcavoy
//
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "proc.h"
#include <errno.h>

// This method take in the commands specific to a node
// and runs fork()/execvp() to build a program with 
// the proper specifications
//
// @param commands
// @return N/A
//
int buildProcess(char **commands, int numOfCommands, int BUFFER_SIZE) {

	pid_t processId;
	int pStatus;
	int execErrorFlag = 0;
	int numStringsInLine = 0;
	char** splitCommands = (char**) malloc(numOfCommands * BUFFER_SIZE * sizeof(char*));
	if(splitCommands == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}

	for(int i = 0; i < numOfCommands; i++){
		if((processId = fork()) < 0){
			fprintf(stderr, "Error: forking new process failed\n");
			exit(-1);
		}else if(processId == 0){
			// overwrite the parent process with the new commands
			numStringsInLine = splitCommandsAtSpaces(commands[i], BUFFER_SIZE, splitCommands);
			if(numStringsInLine){
				execErrorFlag = execvp(splitCommands[0], splitCommands);
				// if child process fails, stop the build
				if(execErrorFlag){
					fprintf(stderr, "Error: Failure to execute because %s\n", strerror(errno));
					fprintf(stderr, "Error: Execvp failed.\n");
					exit(-1);
				}
			}
		}else{
			// parent process will wait until the child process has completed
			while(wait(&pStatus) != processId){}
		}
	}
	free(splitCommands);
	return 0;
}

int splitCommandsAtSpaces(char* command, int BUFFER_SIZE, char** parsedCommands){
	int index = 0;
	char c = '\0';
	char *currString = NULL;
	int currStringIndex = 0;
	int currLineIndex = 0;
	int numOfStrings = 0;

	// for all of the commands split them up and put them in a new 2d array for the execvp
	currLineIndex = 0;
	// cycle through the entire line
	while((c = command[currLineIndex]) != '\0'){
		if(currStringIndex == 0){
			currString = (char*) malloc(BUFFER_SIZE * sizeof(char));
			if(currString == NULL){
				fprintf(stderr, "Error: Unable to malloc memory.\n");
				exit(-1);
			}
		}
		
		// if current char is a space than we reached the end of current string
		if(c == ' '){
			currString[currStringIndex] = '\0';
			*(parsedCommands+index) = currString;
			numOfStrings++;
			index++;
			currStringIndex = 0;
			// store string in new split commands array
		}else{
			currString[currStringIndex] = c;
			currStringIndex++;
		}
		currLineIndex++;
	}

	// if the command isn't empty than add it to the array
	if(currStringIndex > 0){
		currString[currStringIndex] = '\0';
		*(parsedCommands+index) = currString;
		numOfStrings++;
		index++;
	}
	
	*(parsedCommands+index) = NULL;
	index++;

	return numOfStrings;
}
