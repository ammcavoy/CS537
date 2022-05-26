#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STATEFIELD 3
#define USRTIMEFIELD 14
#define	SYSTIMEFIELD 15
#define VIRTMEMFIELD 1
//@Author - Adam McAvoy

/*
 * gets the stat file for the process with pid passed as param
 */
FILE* getstatfile(char* pid){
	char *directory = (char*) malloc(sizeof(char) * 3);  //allocate space for the dir name
        if(directory == NULL) {	
       		fprintf(stderr, "Out of memory\n"); 
		return NULL;
        }
	sprintf(directory, "/proc/%s/stat", pid);  //set the directory name with pid 

	FILE *file = fopen(directory, "r");  //open file
	free(directory);

	return file;	//return the file
}
/*
 * gets the statm file for the process with pid passed as param
 */
FILE* getstatmfile(char* pid) {
        char *directory = (char*) malloc(sizeof(char) * 3);  //alloacte space fro the dir name
        if(directory == NULL) {
                fprintf(stderr, "Out of memory\n");
                return NULL;
        }
	sprintf(directory, "/proc/%s/statm", pid);  //set the dir name with pid

        FILE *file = fopen(directory, "r");  //open file
        free(directory);

        return file;  //return the file

}
/*
 * gets the cmdline file for the process with pid passed as param
 */
FILE* getcmdlinefile(char* pid) {
        char *directory = (char*)malloc(sizeof(char) * 3);  
	if(directory == NULL) {
                fprintf(stderr, "Out of memory\n");
                return NULL;
        }
	sprintf(directory, "/proc/%s/cmdline", pid); //set the dir name with pid
	
	FILE *file = fopen(directory, "r");  //open file
        free(directory);

        return file;  //return the file

}
/*
 * gets the field at position j from the file passed as param
 */
char* getfield(FILE* file, int j){
	char buf[1024];  

	int i;
	for(i = 0; i < j; i++) {
		fscanf(file, "%s", buf);
	}

	char* field = malloc(1 + strlen(buf));
	if(field == NULL) {
		fprintf(stderr, "Out of memory\n");
		return NULL;
	}

	field = buf;
	fclose(file);  

	return field;

}
/*
 * gets the state field for the proc with pid as param
 */
char* getstate(char* pid) {
	FILE *statfile = getstatfile(pid);

	if (statfile == NULL) {
		fprintf(stderr, "Unable to open stat file of proc: %s\n", pid);
		return NULL;
	}

	char* state = getfield(statfile, STATEFIELD);
	return state;
}

/*
 * gets the utime field for the proc with pid passed in param
 */
char* getusrtime(char* pid) {
        FILE *statfile = getstatfile(pid);

        if (statfile == NULL) {
                fprintf(stderr, "Unable to open stat file of proc: %s\n", pid);
                return NULL;
        }

        char* usrtime = getfield(statfile, USRTIMEFIELD);
        return usrtime;
}

/*
 * gets the stime field for the proc with pid passed in param
 */
char* getsystime(char* pid) {
        FILE *statfile = getstatfile(pid);

        if (statfile == NULL) {
                fprintf(stderr, "Unable to open stat file of proc: %s\n", pid);
                return NULL;
        }

        char* systime = getfield(statfile, SYSTIMEFIELD);
        return systime;
}

/*
 * gets the vmem field for the proc with pid passed as param
 */
char* getvirtmem(char* pid) {
        FILE *statmfile = getstatmfile(pid);

        if (statmfile == NULL) {
                fprintf(stderr, "Unable to open statm file of proc: %s\n", pid);
                return NULL;
        }

        char* virtmem = getfield(statmfile, VIRTMEMFIELD);
        return virtmem;
}
/*
 * gets the cmdline file for the proc with pid passed as param
 */
char* getcmdline(char* pid) {
	FILE *cmdlinefile = getcmdlinefile(pid);

	if (cmdlinefile == NULL) {
		fprintf(stderr, "Unable to open cmdline file of proc: %s\n", pid);
		return NULL;
	}

	char *cmdline = malloc(sizeof(char) * 256);
	
	fscanf(cmdlinefile, "%[^\n]", cmdline);

	fclose(cmdlinefile);

	return cmdline;
}

