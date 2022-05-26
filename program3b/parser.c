////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: parser.c
// Other Files
// Semester:         CS 537 Fall 2020
//
// Author:           Adam McAvoy
// Email:            ammcavoy@wisc.edu
// CS Login:         mcavoy
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "graph.h"
#include "parser.h"

const char *lowerMakeFile = "makefile";
const char *upperMakeFile = "Makefile";
const char *readBit = "r";
const char *options =  "f::";



/* parses the command line object and returns a Command Line Object 
 * with the necessary contents to create process
 * 
 * Param:
 * 		argc, argv[], BUFFER_SIZE
 * Returns
 * 		inputs -  the command line object with the contents of the commands
 * 
 */
Graph* parseMakefile(int BUFFER_SIZE, char *newFileName){
	FILE *makefile;
	char c = '\0';
	char* target = NULL;
	char* dependency = NULL;
	char* command = NULL;
	int gotTargetFlag = 0;
	int newCmdFlag = 0;
	int commentFlag = 0;
	int spaceFlag = 0;
	int targetIndex = 0;
	int dependencyIndex = 0;
	int commandIndex = 0;
	int lineNum = 0;
	char skippedChar = '\0';
	int commandMalloc = 1;
	int tabMissing = 0;
	
	
	if(newFileName != NULL){
		if((makefile = fopen(newFileName, readBit))){
		}else{
			fprintf(stderr,"Error: Could not find makefile\n");
			exit(-1);
		}
	}else if((makefile = fopen(lowerMakeFile, readBit))){
		// open the makefile specified as "makefile"
	}else if((makefile = fopen(upperMakeFile, readBit))){
		// open the makefile specified as "Makefile"
	}else{
		fprintf(stderr,"Error: Could not find makefile\n");
		exit(-1);
	}

	Graph *dependencyGraph = createGraph(BUFFER_SIZE);

	// iterates through stdin until file end is reached
	while( (c = getc(makefile)) != EOF){
		if(c == '\0'){
			fprintf(stderr, "%d Error: Null Character detected.\n", lineNum);
			exit(-1);
		}
		lineNum++;
		GraphNode *node = createGraphNode(BUFFER_SIZE);
		// get the line with the target and the dependencies
		target = (char *) calloc(BUFFER_SIZE, sizeof(char));
		if(target == NULL){
			fprintf(stderr, "Error: Unable to malloc.\n");
			exit(-1);
		}
		// skip through the new lines or commands
		while( c == '\n' || c == '#'){
			while(c == '\n'){
				c = getc(makefile);
				lineNum++;
			}
			// skip commands
			while(c == '#'){
				while(c != '\n'){
					c = getc(makefile);
				}
				lineNum++;
				c = getc(makefile);
			}
		}
		// get targets and dependencies
		while(c != '\n' || c != EOF){
			// reached the end of the target
			if(c == ':'){
				gotTargetFlag = 1;
				targetIndex = 0;
				node->name = target;
				while(c == ' '){
					c = getc(makefile);
				}
			}
			// get the target
			if(!gotTargetFlag){
				if(targetIndex >= BUFFER_SIZE){
					//printf("targer: %d",targetIndex);
					//printf("buffer: %d",BUFFER_SIZE);
					fprintf(stderr, "%d: Error: Target is longer than bufffer.\n", lineNum);
					exit(-1);
				}
				// add on the cahr that may have skipped from below
				if(skippedChar != '\0'){
					target[targetIndex] = skippedChar;
					targetIndex++;
					skippedChar = '\0';
				}
				if(c != ' '){
					target[targetIndex] = c;
					targetIndex++;
				}
			}else if (c != ':' && c != ' ' && gotTargetFlag){
				// get the dependencies
				dependency = (char *) calloc(BUFFER_SIZE, sizeof(char));
				if(dependency == NULL){
					fprintf(stderr, "Error: Unable to malloc.\n");
					exit(-1);
				}
				// get all of the dependencies
				while(c != ' ' && c != '\n'){
					if(c == '\0'){
						fprintf(stderr, "%d Error: Null Character detected between dependencies.\n", lineNum);
						exit(-1);
					}	
					if(dependencyIndex >= BUFFER_SIZE){
						fprintf(stderr, "%d: Error: Dependency is longer than bufffer.\n", lineNum);
						exit(-1);
					}
					dependency[dependencyIndex] = c;
					dependencyIndex++;
					c = getc(makefile);
				}
				// if the dependency is valid than add it to dependencies
				if(dependency != NULL && dependency[0] != '\0' && dependencyIndex != 0){
					node->dependencies[node->numDependencies] = dependency;
					(node->numDependencies)++;
				}else{
					free(dependency);
				}
				dependencyIndex = 0;				
			}
			if(targetIndex >= BUFFER_SIZE){
				fprintf(stderr, "%d: Error: Target is longer than buffer.\n", lineNum);
				exit(-1);
			}

			if(c == '\n'){
				break;
			}
			c = getc(makefile);		
		}
		// never got a semicolon 
		if(!gotTargetFlag){
			if(tabMissing){
				fprintf(stderr, "Error: Command line should start with a tab.\n");
				fprintf(stderr, "%d: Invalid line: \"%s\"\n", lineNum, target);
				exit(-1);
			}else{
				fprintf(stderr, "%d: Invalid line: \"%s\"\n", lineNum, target);
				exit(-1);
			}
		}

		tabMissing = 0;
		gotTargetFlag = 0;
		newCmdFlag = 0;
		spaceFlag = 0;
		c = getc(makefile);
		// get the commands
		while(1){
			if(c == '\0'){
				fprintf(stderr, "%d Error: Null Character detected between commands.\n", lineNum);
				exit(-1);
			}
			if(!newCmdFlag){
				// found a tab assume this is a command
				if(c == '\t'){
					lineNum++;
					newCmdFlag = 1;
					c = getc(makefile);
					if(c == '#'){
						commentFlag = 1;
					}
				}else if(c == '\n' || c == EOF){
					// skip the empty lines
					while(c == '\n' || c == '\0'){
						lineNum++;
						c = getc(makefile);
					}
					// skip the comment lines
					while(c == '#'){
						while(c != '\n'){
							c = getc(makefile);
						}
						lineNum++;
						c = getc(makefile);
					}
					// tab was found so a new command flag can be set
					if(c == '\t'){
						newCmdFlag = 1;
						c = getc(makefile);
					}else{
						if(c != '\n'){
							skippedChar = c;
						}
						break;
					}
				// get comment
				}else if(c == '#'){
					while(c != '\n'){
						c = getc(makefile);
					}
					lineNum++;
				} else{
					if(c != '\n'){
						skippedChar = c;
					}
					if(c != '#'){
						tabMissing = 1;
					}
					break;
				}

			}else{
				//printf("cmdindex: %d\n", commandIndex);
				while(c == ' ' || c == '\t'){
					c = getc(makefile);
				}
				while(c != '\n'){
					//printf("char: %d = %d\n", commandIndex, c);
					while(c == ' ' || c == '\t'){
						c = getc(makefile);
						spaceFlag = 1;
					}
					if(spaceFlag){
						ungetc(c, makefile);
						c = ' ';
					}
					if(c == '\0'){
						fprintf(stderr, "%d Error: Null Character detected between dependencies.\n", lineNum);
					}
					//get command
					if(commandMalloc){
						command = (char *) calloc(BUFFER_SIZE, sizeof(char));
						if(command == NULL){
							fprintf(stderr, "Error: Unable to malloc memory.\n");
							exit(-1);
						}
						commandMalloc = 0;
					}
					// verify that this command isn't exceding the buffer limit
					if(commandIndex >= BUFFER_SIZE){
						//printf("cmdIndex: %d\n", commandIndex);
						//printf("buffer: %d\n", BUFFER_SIZE);
						fprintf(stderr, "%d: Error: Command is longer than bufffer.\n", lineNum);
						exit(-1);
					}
					if(commandIndex == 0 && c == '#'){
						commentFlag = 1;
					}
					command[commandIndex] = c;
					//printf("new char:%d\n", c);
					commandIndex++;
					c = getc(makefile);
					spaceFlag = 0;
				}
				// verify that the current command is valid and than add it the list of commands
				if(command != NULL && !commentFlag){
					command[commandIndex] = '\0';
					node->commands[node->numCommands] = command;
					(node->numCommands)++;
					//int i = 0;
					//for(i = 0; command[i] != '\0'; i++)
					//{
   					//	printf("%c", command[i]);
					//}
					//printf("command^\n");
				}else{
					free(command);
				}
				commandIndex = 0;
				newCmdFlag = 0;
				commentFlag = 0;
				c = getc(makefile);
				commandMalloc = 1;
			}
			command = NULL;
		}
		insertNode(dependencyGraph, node);
	}
	return dependencyGraph;
}



/* parses the command line object and returns a Command Line Object 
 * with the necessary contents to create process
 * 
 * Param:
 * 		argc, argv[], BUFFER_SIZE
 * Returns
 * 		inputs -  the command line object with the contents of the commands
 * 
 */
CommandLine* parseCommandLine(int argc, char* argv[], int BUFFER_SIZE){
	CommandLine *inputs = malloc(BUFFER_SIZE * sizeof(CommandLine));
	if(inputs == NULL){
		fprintf(stderr, "Error: Unable to malloc.\n");
		exit(-1);
	}
	inputs->numProcesses = 0;
	int i = 0;
	char opt = '\0';
	int foundFileFlag = 0;
	char* fileName = NULL;
	int index = 0;
	char *tempString = NULL;
	int FFlag = 0;

	while(i < argc){
		// get options 
		opt = getopt (argc, argv, options);
		if(opt != -1){
			switch(opt)
			{

			// -f option
			case 'f':
				if(optarg != NULL){
					fileName = optarg;
				}
				break;
			case '?':
				fprintf(stderr, "Error: Unsupported option specified.\n");
				// error
				exit(-1);
				break;
				
			default:
				break;
			}
		}
	i++;
	}
	
	inputs->process = (char**) malloc(BUFFER_SIZE * sizeof(char*));
	if(inputs->process == NULL){
		fprintf(stderr, "Error: Unable to malloc.\n");
		exit(-1);
	}
	for(int i = 1; i < argc; i++){
		// if the file marker is found
		if(strcmp(argv[i],"-f") == 0){
			foundFileFlag = 1;  	//set the flag
		}else if(foundFileFlag){  	//and get the filename 
			fileName = argv[i];
			foundFileFlag = 0;
		}else{
			index = 0;
			tempString = (char *) calloc(BUFFER_SIZE, sizeof(char));
			if(tempString == NULL){
				fprintf(stderr, "Error: Unable to malloc.\n");
				exit(-1);
			}
			while(argv[i][index] != '\0'){
				tempString[index] = argv[i][index];
				if(strcmp("-f",tempString) == 0){
					FFlag = 1;
					break;
				}
				index++;
			}
			free(tempString);
			if(!FFlag){
				inputs->process[inputs->numProcesses] = argv[i];
				(inputs->numProcesses)++;
			}
			FFlag = 0;
		}
	}

	inputs->newFilename = fileName;

	if(inputs->numProcesses == 0 && fileName == NULL){
		free(inputs->process);
		free(inputs);
		inputs = NULL;
	}

	return inputs;
}

