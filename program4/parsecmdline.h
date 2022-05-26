#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//@Author - Adam McAvoy


//Struct used to hold the params for the options that can be used in call
typedef struct params{	
	int p;	// 1 if -p used
	char* page_sz;	// the page size if -p used
	int m;	// 1 if -m used
	char* mem_sz;	// the mem size if the -m used
	int fail; 	// 1 if there was invalid input
	char* filename;
}params;  	


/*
 * Parses command line setting params for specific options
 */
params* parseCMDLine(int argc, char *argv[]);

