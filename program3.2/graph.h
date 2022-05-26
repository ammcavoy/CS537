////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: dependencyGraph.h
// Other Files
// Semester:         CS 537 Fall 2018
//
// Author:           Ethan Lengfeld
// Email:            elengfeld@wisc.edu
// CS Login:         lengfeld
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          None
//
//
// Online sources:   None
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GRAPH
#define GRAPH

typedef struct{
	char *name;
	char **commands;
	char **dependencies;
	char **parents;
	int numOfDependencies;
	int numOfCommands;
	int numOfParents;
	int checked;
}GraphNode;

typedef struct{
	int numInOrderedArray;
	int size;
	GraphNode **nodes;
}Graph;

GraphNode* createGraphNode();

Graph* createGraph();

void insertNode(Graph *dependencyGraph, GraphNode *newNode);

char** getGraphOrdering(Graph *graph);

void getLowestNode(Graph *graph, GraphNode* node, char** orderedArray, int* arrayIndex);

GraphNode* findNode(Graph *graph, char* name);

int needToAddNode(GraphNode *newNode);

int checkGraphForCycle(Graph *graph);

void lookAtAllNodes(Graph *graph, GraphNode* node);

int isRelative(Graph *graph, GraphNode *currNode, char* element);

int addAlready(GraphNode *currNode, char* parentName);

int isNotDependencyOfClean(GraphNode *node);

#endif
