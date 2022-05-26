////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: parser.h
// Other Files
// Semester:         CS 537 Fall 2020
//
// Author:           Adam McAvoy
// Email:            ammcavoyo@wisc.edu
// CS Login:         mcavoy
//
////////////////////////////////////////////////////////////////////////////////

#ifndef PARSER
#define PARSER

#include "graph.h"

typedef struct{
	char *newFilename;
	char **process;
	int numProcesses;
}CommandLine;

CommandLine* parseCommandLine(int argc, char* argv[], int BUFFER_SIZE);

Graph* parseMakefile(int BUFFER_SIZE, char *newFilename);


#endif

