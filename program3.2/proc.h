////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: processCreation.h
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

#ifndef PROCESS
#define PROCESS

int buildProcess(char **commands, int numOfCommands, int BUFFER_SIZE);

int splitCommandsAtSpaces(char* command, int BUFFER_SIZE, char** parsedCommands);

#endif
