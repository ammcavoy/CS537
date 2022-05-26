
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//@Author - Adam McAvoy


//Struct used to hold the params for the options that can be used in call
typedef struct params{	
	int p;	// 1 if -p used
	char* pid;	// the PID if -p used
	int state;	// 1 if -s used
	int usr_time;	// 0 if -U used
	int sys_time;	// 1 if -S used
	int virt_mem;	// 1 if -v used 
	int cmd_line;	// 0 if -c used
	int fail; 	// 1 if there was invalid input
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
	
	//set usr_time and cmd_line to default to 1 
	paramset->usr_time = 1;
	paramset->cmd_line = 1;

	//the getopt loop that checks to see what params should be turned on/off
	while((opt = getopt(argc, argv, "p:s::U::S::v::c::")) != -1) {
		switch(opt) {
			case 'p':
				paramset->p = 1;
				paramset->pid = optarg;  
				if (atoi(optarg) <= 0) {
					free(paramset);
					fprintf(stderr, "PID has to be greater than 0\n");
					return NULL;
				}
				break;
			case 's' :
				if(optarg == NULL) paramset->state = 1;
				else if(optarg != NULL && strcmp(optarg, "-") == 0) paramset->fail = 0;
				else if(optarg != NULL && strcmp(optarg, "-") != 0) paramset->fail = 1; 
				break;
			case 'U' :
				if(optarg == NULL) paramset->usr_time = 1;
                                else if(optarg != NULL && strcmp(optarg, "-") == 0) paramset->usr_time = 0;
				else if(optarg != NULL && strcmp(optarg, "-") != 0) paramset->fail = 1;
                                break;
			case 'S' :
				if(optarg == NULL) paramset->sys_time = 1;
                                else if(optarg != NULL && strcmp(optarg, "-") == 0) paramset->sys_time = 0;
                                else if(optarg != NULL && strcmp(optarg, "-") != 0) paramset->fail = 1;
				break;
			case 'v' :
				if(optarg == NULL) paramset->virt_mem = 1;
                                else if(optarg != NULL && strcmp(optarg, "-") == 0) paramset->virt_mem = 0;
                                else if(optarg != NULL && strcmp(optarg, "-") != 0) paramset->fail = 1;
				break;
			case 'c' :
				if(optarg == NULL) paramset->cmd_line = 1;
				else if(optarg != NULL && strcmp(optarg, "-") == 0) paramset->cmd_line = 0;
                                else if(optarg != NULL && strcmp(optarg, "-") != 0) paramset->fail = 1;
				break;
			default: /* '?' */
				paramset->fail = 1;
                		return paramset;	
		}
	}

	//if invalid input set fail param to true
	if (optind < argc) {
		paramset->fail = 1;
	}

	return paramset;

}

