////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: makeParser.h
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

#ifndef PARSER
#define PARSER

#include "graph.h"

typedef struct{
	char *newFilename;
	char **processToBuild;
	int numProcessesToBuild;
}CommandLineObject;

CommandLineObject* parseCommandLine(int argc, char* argv[], int BUFFER_SIZE);

Graph* parseMakefile(int BUFFER_SIZE, char *newFilename);

char** splitUpCommands(char** commands, int numOfCommands, int BUFFER_SIZE);

#endif
