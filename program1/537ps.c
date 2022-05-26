#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "parsecmdline.h"
#include "parsestat.h"
#include "getprocs.h"

/*
 * the main funciton of the program
 * @Author - Adam Mcavoy
 */
int main(int argc, char *argv[]) {
        params* opts = parseCMDLine(argc, argv);  //calls function to get user input in params* struct

        if((opts == NULL) | (opts->fail == 1)) {  //checks that opts was initialized correctly
                fprintf(stderr, "Usage: 537ps -p <PID> -[sUSvc]\n ");
		return 0;
        }

	
	if(opts->p == 1){  //if the -p option was selected 
		if(getproc(opts->pid) == 0){  //checks that pid is valid
			printf("PID:%s could not be found\n", opts->pid);
			return 0;
		}		
		else {  //if valid go through and check params and print out that data
			printf("%s: ", opts->pid);  //by calling the respective functions

                        if(opts->state == 1){
                                printf("%s ", getstate(opts->pid));
                        }
                        if(opts->usr_time == 1) {
                                printf("utime=%s ", getusrtime(opts->pid));
                        }
                        if(opts->sys_time == 1) {
                                printf("stime=%s ", getsystime(opts->pid));
                        }
                        if(opts->virt_mem == 1) {
                                printf("vmem=%s ", getvirtmem(opts->pid));
                        }
                        if(opts->cmd_line == 1) {
                                printf("[%s]", getcmdline(opts->pid));
                        }
                        printf("\n");
		}	
	}	
	else {  //if the -p function not selected
		linkednode *head = (linkednode*)malloc(sizeof(linkednode*));  //gets linkedlist of pids available to usr
		head = usrproclist();
		linkednode *current = head;

		while(current->pid != NULL) {  //loop through linked list
			printf("%s: ", current->pid);  //go through params and print out the data
						       //with respect to the user input by calling the 	
			if(opts->state == 1){          //respective funcitons	
				printf("%s ", getstate(current->pid));
			}
			if(opts->usr_time == 1) {
				printf("utime=%s ", getusrtime(current->pid));
			}
			if(opts->sys_time == 1) {
				printf("stime=%s ", getsystime(current->pid));
			}
			if(opts->virt_mem == 1) {
				printf("vmem=%s ", getvirtmem(current->pid));
			}
			if(opts->cmd_line == 1) {
				printf("[%s]", getcmdline(current->pid));
			}
			printf("\n");
			current = current->next;
		}
	}
}

