////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: main.c
// Other Files: main.c
// Semester:         CS 537 Fall 2020
//
// Author:   	     Adam McAvoy       
// Email:            ammcavoy@wisc.edu
// CS Login:         mcavoy
//
////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "graph.h"
#include "proc.h"

const int BUFFER_SIZE = 4096;

// This method handles all of the functionality of making 
// a process, with the various calls necessary including
// creating and iterating through a dependency graph and
// its nodes and then running the commands within each node
//
int main(int argc, char* argv[]){
	Graph *graph = NULL;
	GraphNode *temp;
	GraphNode *processToBuild;
	char* clean = "clean";

	// if there are multiple commands than parse them
	CommandLineObject *inputs = parseCommandLine(argc, argv, BUFFER_SIZE);
	// parse makefile or specifified make file and return back a graph
	if(inputs == NULL){
		graph = parseMakefile(BUFFER_SIZE, NULL);
	} else{
		graph = parseMakefile(BUFFER_SIZE, inputs->newFilename);
	}

	if(graph == NULL){
		fprintf(stderr, "Error: Unable to determine dependencies.\n");
		exit(-1);
	}
	// put the order that the makefile should be built in into a 2d array
	char **orderedArray = getGraphOrdering(graph);
	
	if(argc == 1){
		for(int i = 0; i < graph->numInOrderedArray; i++){
			// find the next node that should be processed
			temp = findNode(graph, orderedArray[i]);
			if(temp == NULL){
				fprintf(stderr, "Unable to find build process for %s.\n", orderedArray[i]);
				exit(-1);
			}
			// determine if the current node actually needs to be rebuilt
			if(isNotDependencyOfClean(temp)){
				if(needToAddNode(temp) && (strcmp(temp->name,clean))){
					buildProcess(temp->commands, temp->numOfCommands,BUFFER_SIZE);
				}else if(strcmp(temp->name,clean) != 0){
					fprintf(stderr, "'%s' is up to date.\n",temp->name);
				}
			}		
		}
	}else{
		if(inputs == NULL){
			fprintf(stderr, "Error: Unable to determine arguments.\n");
			exit(-1);
		}
		if(inputs->newFilename != NULL){
			for(int i = 0; i < graph->numInOrderedArray; i++){
				// find the next node that should be processed
				temp = findNode(graph, orderedArray[i]);
				if(temp == NULL){
					fprintf(stderr, "Unable to find build process for %s.\n", orderedArray[i]);
					exit(-1);
				}
				// determine if the current node actually needs to be rebuilt
				if(isNotDependencyOfClean(temp)){
					if(needToAddNode(temp) && (strcmp(temp->name,clean))){
						buildProcess(temp->commands, temp->numOfCommands,BUFFER_SIZE);
					}else if(strcmp(temp->name,clean) != 0){
						fprintf(stderr, "'%s' is up to date.\n",temp->name);
					}
				}		
			}
		}
		// go through all the specified process to build
		for(int m = 0; m < inputs->numProcessesToBuild; m++){
			processToBuild = findNode(graph, inputs->processToBuild[m]);
			if(processToBuild == NULL){
				fprintf(stderr, "Unable to find build process for %s.\n", inputs->processToBuild[m]);
				exit(-1);
			}
			// this will build through the specified processes in the proper order
			for(int k = 0; k < graph->size; k++){
				if(isRelative(graph, processToBuild, orderedArray[k])){
					temp = findNode(graph, orderedArray[k]);
					if(temp == NULL){
						fprintf(stderr, "Unable to find build process for %s.\n", orderedArray[k]);
						exit(-1);
					}
					if(needToAddNode(temp)){
						buildProcess(temp->commands, temp->numOfCommands,BUFFER_SIZE);
					}else{
						fprintf(stderr, "'%s' is up to date.\n",temp->name);
					}
				}
			}
			if(needToAddNode(processToBuild)){
				buildProcess(processToBuild->commands, processToBuild->numOfCommands,BUFFER_SIZE);
			}else{
				fprintf(stderr, "'%s' is up to date.\n",processToBuild->name);
			}
		}
	}
}
