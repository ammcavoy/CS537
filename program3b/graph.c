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
	graph->size = 0;
	graph->numInOrderedArray = 0;
	graph->nodes = (GraphNode**) malloc(size*sizeof(GraphNode*));
	if(graph->nodes == NULL){
		fprintf(stderr, "Error: Unable to malloc memory.\n");
		exit(-1);
	}

	return graph;
}


/* Creates a graph node and inits the data fields
 * 
 * Param:
 * 		size
 * Returns
 * 		node
 * 
 */
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
	node->checked = 0;
	node->numParents = 0;
	node->numDependencies = 0;
	node->numCommands = 0;
	
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


/* inserts a node into the graph
 * 
 * Param:
 * 		Graph - the graph having a node inserted into 
 * 		newNode - the node being inserted
 * Returns
 * 		N/A
 * 
 */
void insertNode(Graph *Graph, GraphNode *newNode){
	int graphSize = Graph->size;
	Graph->nodes[graphSize] = newNode;
	(Graph->size)++;
}


/* checks the nodes in the graph and their children and parents
 * to determine if there is a cycle in the graph
 * 
 * Param:
 * 		graph
 * Returns
 * 		N/A 
 */
int checkCycles(Graph *graph){

	int graphSize = graph->size;
	int isParent = 0;
	int isChild = 0;
	
	// iterates through the graph
	for(int i = 0; i < graphSize; i++){
		GraphNode *currNode = graph->nodes[i];
		char* currNodeName = currNode->name;
		for(int j = 0; j < graphSize; j++){
			if(i == j){
			// don't need to compare nodes to themselves
			}else{
				// if it hasn't been checked go through the node and its dependencies
				GraphNode *otherNode = graph->nodes[j];
				char *otherNodeName = otherNode->name;
				for(int eachCurrDepend = 0; eachCurrDepend < currNode->numDependencies; eachCurrDepend++){
					if(!strcmp(currNode->dependencies[eachCurrDepend],otherNodeName)){
						isParent = 1;
					}
				}
				for(int eachDepend = 0; eachDepend < otherNode->numDependencies; eachDepend++){
					if(!strcmp(currNodeName,otherNode->dependencies[eachDepend])){
						isChild = 1;
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


/* This method takes in a graph and orders it
 * based on dependencies first, and then ending with
 * the target
 * 
 * Param:
 * 		graph
 * Returns
 * 		orderedArray 
 */
char** getOrdering(Graph *graph){
	
	// ensuring the graph has no cycle
	if(checkCycles(graph)){
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


/* This method gets the lowest node in the 
 * dependency graph passed in for post
 * order traversal
 * 
 * Param:
 * 		graph
 * 		node
 * 		orderedArray
 * 		arrayIndex
 * Returns
 * 		N/A 
 */
void getLowestNode(Graph *graph, GraphNode* node, char** orderedArray, int* arrayIndex){
	
	// iterates through the tree to find lowest node
	for(int i = 0; i < node->numDependencies; i++){
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


/* This method finds a node in the dependency graph
 * 
 * Param:
 * 		name
 * Returns
 * 		node with name passed as param
 */
GraphNode *findNode(Graph *graph, char *name){
	
	// iterates through the graph to find specific node
	for(int i = 0; i < graph->size; i++){
		if(!strcmp(name, graph->nodes[i]->name)){
			return graph->nodes[i];
		}
	}
	return NULL;
}


/* This method creates a new node
 * for a dependecy of a target
 * 
 * Param:
 * 		newNode
 * Returns
 * 		0 if not older, 1 if older than target
 */
int needToAddNode(GraphNode *newNode){
	time_t targetTimeMod;
	time_t dependTimeMod;
	char *FileName = newNode->name;
	struct stat targetStat;
	int status = stat(FileName, &targetStat);
	if(status){
		return 1;
	}
	targetTimeMod = targetStat.st_mtime;

	// creates dependency for the parent
	for(int i = 0; i < newNode->numDependencies; i++){
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

/* compares node to all the other nodes in the graph
 * 
 * Param:
 * 		graph
 * 		node
 * Returns
 * 		N/A
 */
void lookAtAllNodes(Graph *graph, GraphNode* node){
	for(int i = 0; i < node->numDependencies; i++){
		GraphNode *depNode = findNode(graph, node->dependencies[i]);
		if(depNode != NULL){
			// add its parents nodes to the the current dependency node
			for(int j = 0; j < node->numParents; j++){
				if(!alreadyAdded(depNode, node->parents[j])){
					depNode->parents[depNode->numParents] = node->parents[j];
					depNode->numParents++;
				}
			}
			// add parent node to the list dependency node's parent list
			if(!alreadyAdded(depNode, node->name)){
				depNode->parents[depNode->numParents] = node->name;
				depNode->numParents++;
			}
			// check for repetition between depencdencies and parents
			for(int k = 0; k < depNode->numDependencies; k++){
				for(int m = 0; m < depNode->numParents; m++){
					if(strcmp(depNode->parents[m],depNode->dependencies[k]) == 0){
						fprintf(stderr,"Error: Cycle was detected during compilation involving dependency %s.\n",depNode->dependencies[k]);
						exit(-1);
					}
				}
			}
			// recursive call to go through rest of graph
			lookAtAllNodes(graph, depNode);
		}
	}
}


/* this method will determine whether or not a build/node is within the span of 
 * another build/node
 * 
 * Param:
 * 		graph
 * 		currNode
 * 		element
 * Returns
 * 		orderedArray 
 */
int isRelative(Graph *graph, GraphNode *currNode, char* element){
	int relative = 0;
	for(int i = 0; i < currNode->numDependencies; i++){
		GraphNode *depNode = findNode(graph, currNode->dependencies[i]);
		if(depNode != NULL){
			if(strcmp(depNode->name,element) == 0){
				return 1;
			}
			// recursive call
			relative = isRelative(graph, depNode,element);
		}
	}
	return relative;
}

/* checks if a node has already been inserted
 * 
 * Param:
 * 		graph
 * 		currNode
 * Returns
 * 		int 1 - already added, 0 - not 
 */
int alreadyAdded(GraphNode *currNode, char* parentName){
	for(int i = 0; i < currNode->numParents; i++){
		if(strcmp(currNode->parents[i],parentName) == 0){
			return 1;
		}
	}
	return 0;
}


int isNotDependencyOfClean(GraphNode *node){
	char *clean = "clean";
	char *Clean = "Clean";
	for(int i = 0; i < node->numParents; i++){
		if(strcmp(node->parents[i],clean) == 0 || strcmp(node->parents[i],Clean) == 0){
			return 0;
		}
	}
	return 1;
}

