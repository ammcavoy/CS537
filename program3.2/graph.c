////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: graph.c
// Other Files
// Semester:         CS 537 Fall 2020
//
// Author:      Adam McAvoy
// Email:       ammcavoy@wisc.edu.edu
// CS Login:	mcavoy         
//
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "graph.h"

// This method creates a GraphNode and intializes
// its various data members
//
// @param size
// @return node
//
GraphNode* createGraphNode(int size){
	GraphNode* node = (GraphNode*) malloc(size*sizeof(GraphNode));
	if(node == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}
	node->name = (char*) malloc(size*sizeof(char));
	if(node->name == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}
	node->numOfDependencies = 0;
	node->numOfCommands = 0;
	node->numOfParents = 0;
	node->checked = 0;

	node->dependencies = (char**) malloc(size*sizeof(char*));
	if(node->dependencies == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}
	node->commands = (char**) malloc(size*sizeof(char*));
	if(node->commands == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}
	node->parents = (char**) malloc(size*size*sizeof(char*));
	if(node->parents == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}
	
	return node;
}

// This method creates a dependency graph and
// initializes its data members
//
// @param size
// @return graph
//
Graph* createGraph(int size){
	Graph* graph = (Graph*) malloc(size*sizeof(Graph));
	if(graph == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}
	graph->numInOrderedArray = 0;
	graph->size = 0;
	graph->nodes = (GraphNode**) malloc(size*sizeof(GraphNode*));
	if(graph->nodes == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}

	return graph;
}

void insertNode(Graph *Graph, GraphNode *newNode){
	int graphSize = Graph->size;
	Graph->nodes[graphSize] = newNode;
	(Graph->size)++;
}

// This method checks the graph for cycles
//
// @param graph
// @return N/A
//
int checkGraphForCycle(Graph *graph){

	int graphSize = graph->size;
	int isChild = 0;
	int isParent = 0;
	
	// iterates through the graph
	for(int i = 0; i < graphSize; i++){
		GraphNode *currNode = graph->nodes[i];
		char* currNodeName = currNode->name;
		for(int j = 0; j < graphSize; j++){
			if(i == j){
			// don't need to check the current node against itself
			}else{
				// if it hasn't been checked go through the node and its dependencies
				GraphNode *otherNode = graph->nodes[j];
				char *otherNodeName = otherNode->name;
				for(int eachDepend = 0; eachDepend < otherNode->numOfDependencies; eachDepend++){
					if(!strcmp(currNodeName,otherNode->dependencies[eachDepend])){
						isChild = 1;
					}
				} 
				for(int eachCurrDepend = 0; eachCurrDepend < currNode->numOfDependencies; eachCurrDepend++){
					if(!strcmp(currNode->dependencies[eachCurrDepend],otherNodeName)){
						isParent = 1;
					}
				}
				// if a node is determined as a child and a parent that is an error
				if(isChild && isParent){
					return -1;
				}
				// check current node against all other nodes
				lookAtAllNodes(graph, currNode);
				isChild = 0;
				isParent = 0;
			}
		}
	}
	return 0;
}

// This method takes in a graph and orders it
// based on dependencies first, and then ending with
// the target
//
// @param graph
// @return orderedArray
//
char** getGraphOrdering(Graph *graph){
	
	// ensuring the graph has no cycle
	if(checkGraphForCycle(graph)){
		fprintf(stderr,"Error: Cycle was detected during compilation.\n");
		exit(-1);
	}

	int graphSize = graph->size;
	// this will store the names of the nodes and the order in which they should run
	char** orderedArray = malloc(graph->size * sizeof(char*));
	if(orderedArray == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}
	int* arrayIndex = (int *) malloc(graph->size *sizeof(int));
	if(arrayIndex == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}
	*arrayIndex = 0;

	// iterates through each node and its dependencies to build the array
	for(int i = 0; i < graphSize; i++){
		GraphNode *currNode = graph->nodes[i];
		if(!currNode->checked){
			getLowestNode(graph,currNode,orderedArray,arrayIndex);
		}
	}
	free(arrayIndex);
	return orderedArray;
}

// This method gets the lowest node in the 
// dependency graph passed in for post
// order traversal
//
// @param graph, node, orderedArray, arrayIndex
// @return N/A
//
void getLowestNode(Graph *graph, GraphNode* node, char** orderedArray, int* arrayIndex){
	
	// iterates through the tree to find lowest node
	for(int i = 0; i < node->numOfDependencies; i++){
		GraphNode *dependencyNode = findNode(graph, node->dependencies[i]);
		if(dependencyNode != NULL){
			
			//if the node wasn't checked, check its dependencies
			if(dependencyNode->checked != 1){
				getLowestNode(graph, dependencyNode, orderedArray, arrayIndex);
			}	
		}
	}
	node->checked = 1;
	graph->numInOrderedArray++;
	*(orderedArray+(*arrayIndex)) = node->name;
	(*arrayIndex)++;
}

// This method finds a node in the dependency graph
//
// @param graph, name
// @return N/A
//
GraphNode *findNode(Graph *graph, char *name){
	
	// iterates through the graph to find specific node
	for(int i = 0; i < graph->size; i++){
		if(!strcmp(name, graph->nodes[i]->name)){
			return graph->nodes[i];
		}
	}
	return NULL;
}

// This method creates a new node
// for a dependecy of a target
// 
// @param newNode
// @return 0 if not older, 1 if older than target
// 
int needToAddNode(GraphNode *newNode){
	time_t targetTimeMod;
	time_t dependTimeMod;
	char *targetFileName = newNode->name;
	struct stat targetStat;
	int status = stat(targetFileName, &targetStat);
	if(status){
		return 1;
	}
	targetTimeMod = targetStat.st_mtime;

	// creates a dependency for the parent node
	for(int i = 0; i < newNode->numOfDependencies; i++){
		char* dependencyFileName = newNode->dependencies[i];
		struct stat dependStat;
		// checking the last modified time, in case a rebuild is necessary
		int status = stat(dependencyFileName, &dependStat);
		if(status){
			return 1;
		}
		dependTimeMod = dependStat.st_mtime;
		
		if(targetTimeMod < dependTimeMod){
			// the target file is younger than the dependency thus we have to remake it
			return 1;
		}
	}
	return 0;
}


void lookAtAllNodes(Graph *graph, GraphNode* node){
	// iterates through the tree to find lowest node
	for(int i = 0; i < node->numOfDependencies; i++){
		GraphNode *dependencyNode = findNode(graph, node->dependencies[i]);
		if(dependencyNode != NULL){
			// add its parents nodes to the the current dependency node
			for(int j = 0; j < node->numOfParents; j++){
				if(!addAlready(dependencyNode, node->parents[j])){
					dependencyNode->parents[dependencyNode->numOfParents] = node->parents[j];
					dependencyNode->numOfParents++;
				}
			}
			// add parent node to the list dependency node's parent list
			if(!addAlready(dependencyNode, node->name)){
				dependencyNode->parents[dependencyNode->numOfParents] = node->name;
				dependencyNode->numOfParents++;
			}
			// check that there are is no repetition between depencdencies and parents
			for(int k = 0; k < dependencyNode->numOfDependencies; k++){
				for(int m = 0; m < dependencyNode->numOfParents; m++){
					if(strcmp(dependencyNode->parents[m],dependencyNode->dependencies[k]) == 0){
						fprintf(stderr,"Error: Cycle was detected during compilation involving dependency %s.\n",dependencyNode->dependencies[k]);
						exit(-1);
					}
				}
			}
			// recursive call to go through rest of graph
			lookAtAllNodes(graph, dependencyNode);
		}
	}
}

// this method will determine whether or not a build/node is within the span of another build/node
int isRelative(Graph *graph, GraphNode *currNode, char* element){
	int relative = 0;
	for(int i = 0; i < currNode->numOfDependencies; i++){
		GraphNode *dependencyNode = findNode(graph, currNode->dependencies[i]);
		if(dependencyNode != NULL){
			if(strcmp(dependencyNode->name,element) == 0){
				return 1;
			}
			// recursive call
			relative = isRelative(graph, dependencyNode,element);
		}
	}
	return relative;
}

// this method will determine whether of not a parent's name has been added to the list of parents alrady
int addAlready(GraphNode *currNode, char* parentName){
	for(int i = 0; i < currNode->numOfParents; i++){
		if(strcmp(currNode->parents[i],parentName) == 0){
			return 1;
		}
	}
	return 0;
}

int isNotDependencyOfClean(GraphNode *node){
	char *clean = "clean";
	char *Clean = "Clean";
	for(int i = 0; i < node->numOfParents; i++){
		if(strcmp(node->parents[i],clean) == 0 || strcmp(node->parents[i],Clean) == 0){
			return 0;
		}
	}
	return 1;
}
