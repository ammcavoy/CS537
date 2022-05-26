#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

//linked list struct that will hold the pid's of all the uids
typedef struct linkednode{
	char *pid;
	struct linkednode* next;
}linkednode;

/*
 * functions that returns a linked list of all the pid's unders same uid in procs
 * @authors - Adam McAvoy 
 */
linkednode*  usrproclist() {
	DIR *procs; 		//pointer to the /proc directory
	FILE *statusfile;	//pointer to the current status file being read
	struct dirent *procinfo;//the current proccess directory 	
	int i;	
	char uidline[256];	//char that will hold the uid element info
	char filename[1024];	//char that will hold the filename of uid 
	linkednode *head = (linkednode*)malloc(sizeof(linkednode));  //the head of the linkded list with pid's
	linkednode *current = head;  //reference to head to be used in the main loop


	if((procs = opendir("/proc")) == NULL) {  // open the procs directory
		free(current);
		fprintf(stderr, "Cannot open /procs\n");
		return NULL;
	}
	
	while((procinfo = readdir(procs)) != NULL) { //loop that reads the next dir in procs
		if(procinfo->d_name == NULL){  //checks that nextdir was read correctly
			free(current);
			fprintf(stderr, "Cannot access d_name of proc\n");
			return NULL;
		}
		if((atoi(procinfo->d_name)) != 0) {  //checks that the d_name is a number
			sprintf(filename, "/proc/%s/status", procinfo->d_name);	//gets the file name of the statusfile	
						
			statusfile = fopen(filename, "r");  //opens statusfile
			
			if(statusfile != NULL) {
				for(i = 0; i < 9; i++) { 
					fgets(uidline ,50 , statusfile);
				}
				fscanf(statusfile, "%s %s", uidline, uidline); //scans the statusfile for uid
				if ((signed)getuid() == atoi(uidline)) { //checks that uid's match
					current->pid = procinfo->d_name;  //adds the pid to the linked list of pid's
					current->next = (linkednode*)malloc(sizeof(linkednode));
					current = current->next;
					current->next = NULL;
					
				}
				fclose(statusfile);
			}
		}
	}
	return head;
}
/*
 * checks that the proc with pid passed as param is valid 0=not 1=is valid
 */
int getproc(char* pid) {
	DIR *procs;  //pointer to the /procs dir
	struct dirent *procinfo;  //the current process directory
	
	if((procs = opendir("/proc")) == NULL){  //opens procs and checks it worked
		fprintf(stderr, "Cannot open /procs\n");
		return 0;
	}

	while((procinfo = readdir(procs)) != NULL) {  //loops through procs
		if(procinfo->d_name == NULL) {  //checks that nextdir was read correctly
			fprintf(stderr, "Cannot access d_name of proc");
		       	return 0;	
		}

		if(pid == procinfo->d_name){  //if the pid of current proc matches desired proc
			closedir(procs);  
			return 1;  //returns 1 because the pid's matched
		}

	}
	closedir(procs);
	return 0;
}
