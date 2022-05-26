////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: graph.h
// Other Files
// Semester:         CS 537 Fall 2020
//
// Author:           Adam McAvoy
// Email:            ammcavoy@wisc.edu
// CS Login:         mcavoy
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GRAPH
#define GRAPH


typedef struct{
	char *name;
	char **commands;
	char **dependencies;
	char **parents;
	int numDependencies;
	int numCommands;
	int numParents;
	int checked;
}GraphNode;

typedef struct{
	int numInOrderedArray;
	int size;
	GraphNode **nodes;
}Graph;

Graph* createGraph();

GraphNode* createGraphNode();

void insertNode(Graph *dependencyGraph, GraphNode *newNode);

char** getOrdering(Graph *graph);

void getLowestNode(Graph *graph, GraphNode* node, char** orderedArray, int* arrayIndex);

GraphNode* findNode(Graph *graph, char* name);

int needToAddNode(GraphNode *newNode);

int checkCycles(Graph *graph);

void lookAtAllNodes(Graph *graph, GraphNode* node);

int isRelative(Graph *graph, GraphNode *currNode, char* element);

int alreadyAdded(GraphNode *currNode, char* parentName);

int isNotDependencyOfClean(GraphNode *node);

#endif

