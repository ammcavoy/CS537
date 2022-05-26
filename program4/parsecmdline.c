#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//@Author - Adam McAvoy


//Struct used to hold the params for the options that can be used in call
typedef struct params{	
	int p;	// 1 if -p used
	char * page_sz;	// the page size if -p used
	int m;	// 1 if -m used
	char * mem_sz;	// the mem size if the -m used
	int fail; 	// 1 if there was invalid input
	char * filename;
}params;  	


/*
 * Parses command line setting params for specific options
 */
params* parseCMDLine(int argc, char *argv[]){
	int opt;
	opterr = 0;  //

	params *paramset = malloc(sizeof(params)); 
	if(paramset == NULL) {
		fprintf(stderr, "Out of memory\n");
		return NULL;
	}
	
	//set the initial values of the parameters
	paramset->p = 0;
	paramset->m = 0;
	paramset->fail = 0;

	//the getopt loop that checks to see what params should be turned on/off
	while((opt = getopt(argc, argv, ":p:m:")) != -1) {
		switch(opt) {
			case 'p':
				if(optarg != NULL) paramset->p = 1;
				else{
					free(paramset);
					fprintf(stderr, "Page size parameter missing following '-p'\n");
					return NULL;
				}
				paramset->page_sz = optarg;  
				if (atoi(optarg) <= 0) {
					free(paramset);
					fprintf(stderr, "Page size has to be greater than 0\n");
					return NULL;
				}
				break;
			case 'm' :
				if(optarg != NULL) paramset->m = 1;
				else{
					free(paramset);
					fprintf(stderr, "Memory size parameter missing following '-m'\n");
					return NULL;
				}
				paramset->mem_sz = optarg;
				if(atoi(optarg) <= 0) {
					free(paramset);
					fprintf(stderr, "Memory size has to be greater than 0.\n");
					return NULL;
				}
				break;
			default: /* '?' */
				paramset->fail = 1;
				return paramset;
		}
	}
	
	paramset->filename = argv[argc - 1];

	//if invalid input set fail param to true
	if (optind < argc - 1) {
		printf("bitch\n");
		paramset->fail = 1;
	}

	return paramset;
}

