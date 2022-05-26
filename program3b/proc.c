////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: proc.c
// Other Files
// Semester:         CS 537 Fall 2020
//
// Author:          Adam McAvoy 
// Email:           ammcavoy@wisc.edu
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


/* Take in the commands for a node and run fork/execvp to build program
 * with proper specs
 * 
 * Params:
 * 		commands
 * 		numCommands
 * Returns:
 * 		None
 */
int buildProc(char **commands, int numCommands, int BUFFER_SIZE) {

	pid_t procID;
	int pStatus;
	int execErrorFlag = 0;
	int numStrings = 0;
	char** splitCommands = (char**) malloc(numCommands * BUFFER_SIZE * sizeof(char*));
	if(splitCommands == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}

	for(int i = 0; i < numCommands; i++){
		if((procID = fork()) < 0){
			fprintf(stderr, "Error: forking new process failed\n");
			exit(-1);
		}else if(procID == 0){
			// overwrite parent proc with new commands
			numStrings = splitCmds(commands[i], BUFFER_SIZE, splitCommands);
			if(numStrings){
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
			while(wait(&pStatus) != procID){}
		}
	}
	free(splitCommands);
	return 0;
}

/* split the commands up and insert the split commands into an array or
 * already parsed commands
 * 
 * Param:
 * 		command - the command that is going to be split and added to parsed Commands
 * 		BUFFER_SIZE - the buffer size of the command
 * 		parsedCommands -  the array where the new command will be added to 
 * Return:
 * 		numOfString 
 * 
 */
int splitCmds(char* command, int BUFFER_SIZE, char** parsedCommands){
	int index = 0;
	char c = '\0';
	char *currString = NULL;
	int StringIndex = 0;
	int LineIndex = 0;
	int numOfStrings = 0;

	// for all of the commands split them up and put them in a new 2d array for the execvp
	LineIndex = 0;
	// cycle through the entire line
	while((c = command[LineIndex]) != '\0'){
		if(StringIndex == 0){
			currString = (char*) malloc(BUFFER_SIZE * sizeof(char));
			if(currString == NULL){
				fprintf(stderr, "Error: Unable to malloc memory.\n");
				exit(-1);
			}
		}
		
		// if current char is a space than we reached the end of current string
		if(c == ' '){
			currString[StringIndex] = '\0';
			*(parsedCommands+index) = currString;
			numOfStrings++;
			index++;
			StringIndex = 0;
			// store string in new split commands array
		}else{
			currString[StringIndex] = c;
			StringIndex++;
		}
		LineIndex++;

	}
	// if the command is valid add it to the array
	if(StringIndex > 0){
		currString[StringIndex] = '\0';
		*(parsedCommands+index) = currString;
		numOfStrings++;
		index++;
	}
	
	*(parsedCommands+index) = NULL;
	index++;

	return numOfStrings;
}

