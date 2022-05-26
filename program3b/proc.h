////////////////////////////////////////////////////////////////////////////////
// Main File: main.c
// This File: proc.h
// Other Files
// Semester:         CS 537 Fall 2020
//
// Author:           Adam McAvoy
// Email:            ammcaoy@wisc.edu
// CS Login:         mcavoy
//
////////////////////////////////////////////////////////////////////////////////

#ifndef PROCESS
#define PROCESS

int buildProc(char **commands, int numCommands, int BUFFER_SIZE);

int splitCmds(char* command, int BUFFER_SIZE, char** parsedCommands);

#endif

