#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "parsecmdline.h"
//#include "fifo.h"

typedef struct linkednode{//the struct that is used to hold a procs information gathered in the first pass of trace file
	char * pid;
	int start;
	int* starts;
	int start_track;
	int starts_sz;
	int start_count;
	int end;
	int current;
	int blocked;
	struct linkednode * next;
	struct linkednode * parent;
}linkednode;	

typedef struct memnode{// the struct that will act as memory holding the pages that have been loaded from the disk
	char * pid;
	int count;
	int *mem;
	int zero_in;
	struct memnode * next;
}memnode;

typedef struct blockedproc{//the struct that will act as a queue for the blocked procs
	char * pid;
	int tick_count;
	struct blockedproc * next;
}blockedproc;

typedef struct lrunode{//the struct that will be used to determine what page is removed when mem is full
	char * pid;
	char * vpn;
	struct lrunode * next;
	struct lrunode * parent;
}lrunode;

int in_mem(memnode * curr, char * pid, char * vpn, int tbl_sz);
void add_mem(memnode * curr, char * pid, char * vpn, int tbl_sz);
void remove_mem(memnode * curr, char * pid, char * vpn, int tbl_sz);

unsigned long MEM_SZ = 1;
unsigned long PAGE_SZ = 4096;
int BUFFER_SIZE = 50;
FILE *tracefile;

int main(int argc, char *argv[]){
	params* input = parseCMDLine(argc, argv);
	
	if(input == NULL || input->fail == 1){
		printf("Usage 537 -p <page size> -m <memory size> tracefile");
		return 0;
	}

	if(input->m == 1){
		MEM_SZ = strtoul(input->mem_sz, NULL, 10);
		//printf("%s\n", input->mem_sz);
	}

	if(input->p == 1){
		PAGE_SZ = strtoul(input->page_sz, NULL, 10);
		//printf("%s\n",input->page_sz);
	}
	
	tracefile = fopen(input->filename, "r");
	
	if(tracefile == NULL){
		fprintf(stderr, "was not able to read tracefile or tracefile does not exist");
		return 0;
	}
	
	int line_count = 0;
	int new_node = 0;
	char proc_id[BUFFER_SIZE], vpn[BUFFER_SIZE];
	int pos = ftell(tracefile);
	fscanf(tracefile, "%s %s", proc_id, vpn);
	char *p_copy = malloc(strlen(proc_id) + 1);
	char *v_copy;

	linkednode *head = (linkednode*)malloc(sizeof(linkednode));
	if(head == NULL){
		fprintf(stderr, "Out of memory\n");
		return 0;
	}
	linkednode *current = head;
	linkednode *tail = head;

	strcpy(p_copy, proc_id);
	current->pid = p_copy;
	current->start = pos;
       	current->starts = (int *)malloc(BUFFER_SIZE * sizeof(int));
	current->starts[0] = pos;
	current->start_track = 1;
	current->start_count = 1;
	current->starts_sz = BUFFER_SIZE;
	current->end = pos; 
	


	while(!feof(tracefile)){
		line_count += 1;
		pos = ftell(tracefile);
		fscanf(tracefile, "%s %s", proc_id, vpn);
		if(strcmp(proc_id, current->pid) == 0){	  
			if(pos - current->end > 15){
				if(current->start_count >= current->starts_sz - 1){
					current->starts_sz = current->starts_sz * 2;
					current->starts = (int *)realloc(current->starts, current->starts_sz * sizeof(int));
				}
				current->starts[current->start_count] = pos;
				current->start_count += 1;
			}
			current->end = pos; 
		}
		else {
			current = head;
			while(current->next != NULL){
				if(strcmp(current->pid, proc_id) == 0){
					if(pos - current->end > 15){
						if(current->start_count >= current->starts_sz  - 1){
							current->starts_sz = current->starts_sz * 2;
							current->starts = (int *)realloc(current->starts, current->starts_sz * sizeof(int));
						}						
						current->starts[current->start_count] = pos;
						current->start_count += 1;
					}
					break;
				}
				current = current->next;
			}
			if(strcmp(current->pid, proc_id) != 0){
				new_node = 1;
			}
		}
		if(new_node){
			current = tail;
			current->next = (linkednode*)malloc(sizeof(linkednode));
			if(current->next == NULL){
				fprintf(stderr, "Out of memory\n");
				return 0;
			}
			
			p_copy = malloc(strlen(proc_id) + 1);	
			strcpy(p_copy, proc_id);		  	
			linkednode * par = current;
			
			current = current->next;
			current->parent = par;
			current->next = NULL;
			current->pid = p_copy;
			current->start = pos;
		       	current->starts = (int *)malloc(BUFFER_SIZE * sizeof(int));
			current->starts[0] = pos;
			current->start_count = 1;
			current->start_track = 1;
			current->starts_sz = BUFFER_SIZE;
			current->current = pos;
			current->end = pos;
			tail = current;
			new_node = 0;	
		}
	}




	if(tail->start == tail->end){
		tail = tail->parent;
		tail->next = NULL;
	}
	
	int num_pgs = (MEM_SZ * 1048576)/ PAGE_SZ;
	int tbl_sz = num_pgs * 1.3;
	if(tbl_sz % 2 == 0) tbl_sz += 1;

	memnode * memhead = (memnode *)malloc(sizeof(memnode));
	if(memhead == NULL){
		fprintf(stderr, "Out of memory\n");
		return 0;
	}

	current = head;	
	p_copy = malloc(strlen(current->pid) + 1);
	strcpy(p_copy, current->pid);
	memhead->pid = p_copy;
	memhead->count = 0;
	memhead->zero_in = 0;
	//count = 0;
	memhead->mem = (int *)calloc(tbl_sz, sizeof(int));
	memhead->next = (memnode *)malloc(sizeof(memnode));
	memnode * memcurr = memhead->next;
	current = current->next;
	while(current != NULL){
		p_copy = malloc(strlen(current->pid) + 1);
		strcpy(p_copy, current->pid);
		memcurr->pid = p_copy;
		memcurr->count = 0;
		memcurr->zero_in = 0;
		memcurr->mem = (int *)calloc(tbl_sz, sizeof(int));
		memcurr->next = (memnode *)malloc(sizeof(memnode));
		if(memcurr->next == NULL){
			fprintf(stderr, "Out of memory\n");
			return 0;
		}		

		memcurr = memcurr->next;
		current = current->next;
	}

	//second loop
	int pgs_in = 0;
	int stats_pgs_in = 0;
	int runnable_procs = 0;
	int pg_faults = 0;
	int ticks = 0; 
	int lines_executed = 0;
	blockedproc * blockedhead = NULL;
	lrunode * lruhead = NULL;
	lrunode * lrutail = NULL;
	while(lines_executed < line_count){
		ticks+=1;
		stats_pgs_in += pgs_in;
		linkednode * execute = NULL;
		current = head;
		int min_line = 0;
		while(current != NULL){
			if(current->blocked == 0){
				runnable_procs += 1;
				if(min_line == 0){
					min_line = current->current;
					execute = current;	
					if(current == head) min_line = -1;
				}
				else if(current->current < min_line){
					min_line = current->current;
					execute = current;
				}
			}
			current = current->next;
		}
		if(execute != NULL){
			fseek(tracefile, execute->current, SEEK_SET);
                	fscanf(tracefile, "%s %s", proc_id, vpn);

			if(strcmp(execute->pid, proc_id) != 0){
				while(execute->starts[execute->start_track] < execute->current) {
					execute->start_track += 1;
					
				}

				execute->current = execute->starts[execute->start_track];
				fseek(tracefile, execute->current, SEEK_SET);
				fscanf(tracefile, "%s %s", proc_id, vpn);
			}

                	if(in_mem(memhead, proc_id, vpn, tbl_sz) == 1){


                        	fseek(tracefile, execute->current, SEEK_SET);
                        	fscanf(tracefile, "%s %s", proc_id, vpn);

				lrunode * mv_head = lruhead;
				lrunode * mv_tail = lrutail;
				if(pgs_in > 1){
					while(1){
						if(strcmp(mv_head->pid, proc_id) == 0 && strcmp(mv_head->vpn, vpn) == 0){
							if(mv_head->parent != NULL){

								lrunode * lrupar = mv_head->parent;
								lrunode * lruchild = mv_head->next;
								lruchild->parent = lrupar;
								lrupar->next = lruchild;
								lrutail->next = mv_head;
								mv_head->next = NULL;
								mv_head->parent = lrutail;
								lrutail = lrutail->next;
								break;
							}
							else{
								lruhead = lruhead->next;
								lruhead->parent = NULL;
								lrutail->next = mv_head;
								mv_head->next = NULL;
								mv_head->parent = lrutail;
								lrutail = lrutail->next;;
								break;
							}
						}
						else{
							mv_head = mv_head->next;
						}
						
						if(strcmp(mv_tail->pid, proc_id) == 0 && strcmp(mv_tail->vpn, vpn) == 0){
							if(mv_tail->next == NULL){
								break;
							}
							else{
								lrunode * lruparr = mv_tail->parent;
								lrunode * lruchildd = mv_tail->next;
								lruparr->next = lruchildd;
								lruchildd->parent = lruparr;
								lrutail->next = mv_tail;
								mv_tail->next = NULL;
								mv_tail->parent = lrutail;
								lrutail = lrutail->next;
								break;								
							}
						}
						else{
							mv_tail = mv_tail->parent;
						}
						
					}
				}	



                        	execute->current = ftell(tracefile);



                        	lines_executed += 1;
                        	if(execute->current > execute->end){
                                	current = head;
                                	if(strcmp(head->pid, proc_id) == 0){
                                        	head = head->next;
                                	}
                                	else{
                                        	while(strcmp(current->pid, proc_id) != 0){
							current = current->next;
						}
						if(current == head){
							head = head->next;
							head->parent = NULL;
						}
						else if(current->next == NULL){
							linkednode * par = current->parent;
							par->next = NULL;
						}
						else{
							linkednode * par = current->parent;
							linkednode * child = current->next;
							par->next = child;
							child->parent = par;
						}
                                	}

					memnode * removale = memhead;
					memnode * parent;
					while(strcmp(proc_id, removale->pid) != 0){
						parent = removale;
						removale = removale->next;
					}
					if(removale == memhead){
						pgs_in -= (removale->count + removale->zero_in);
						memhead = memhead->next;
					}
					else{
						pgs_in -= (removale->count + removale->zero_in);
						parent->next = removale->next;
						free(removale);
					}
					lrunode * lrucurr = lruhead;
                                        lrunode * lruparent = NULL;
                                        while(lrucurr != NULL){
                                                if(strcmp(proc_id, lrucurr->pid) == 0){
                                                        if(lruparent == NULL){
                                                                lruhead = lruhead->next;
                                                                lrucurr = lruhead;
                                                        }
                                                        else{
                                                                lruparent->next = lrucurr->next;
                                                                lrucurr = lrucurr->next;
                                                        }
                                                }
                                                else{
                                                        lruparent = lrucurr;
                                                        lrucurr = lrucurr->next;
                                                }

                                        }

                        	}
                	}

			else{
        	               	pg_faults += 1;
 	               		execute->blocked = 1;
                	        if(blockedhead == NULL){
                                	blockedhead = (blockedproc *)malloc(sizeof(blockedproc));
                                	blockedhead->next = NULL;
                                	p_copy = malloc(sizeof(proc_id) + 1);
                                	strcpy(p_copy, proc_id);
                                	blockedhead->pid = p_copy;
                                	blockedhead->tick_count = 2000000;
                                	blockedhead->next = NULL;
                        	}
                        	else{
                                	blockedproc * currblocked = blockedhead;
                                	while(currblocked->next != NULL) currblocked = currblocked->next;
                                	currblocked->next = (blockedproc *)malloc(sizeof(blockedproc));
                                	currblocked = currblocked->next;
                                	p_copy = malloc(sizeof(proc_id) + 1);
                                	strcpy(p_copy, proc_id);
                                	currblocked->pid = p_copy;
                                	currblocked->tick_count = 2000000;
                                	currblocked->next = NULL;
                        	}
                	}
		}



		else{
                        ticks += blockedhead->tick_count;
			stats_pgs_in += pgs_in * blockedhead->tick_count;

			current = head;
                        while(strcmp(current->pid,blockedhead->pid) != 0) current = current->next;
			current->blocked = 0;
                        fseek(tracefile, current->current, SEEK_SET);
                        fscanf(tracefile, "%s %s", proc_id, vpn);
			p_copy = malloc(sizeof(proc_id) + 1);
                        v_copy = malloc(sizeof(vpn) + 1);
                        strcpy(p_copy, proc_id);
                        strcpy(v_copy, vpn);
			
                        if(lruhead == NULL){
                        	lruhead = (lrunode *)malloc(sizeof(lrunode));
                                lrutail = lruhead;
                                lruhead->next = NULL;
				lruhead->parent = NULL;
                                lruhead->pid = p_copy;
                                lruhead->vpn = v_copy;
                        }
                        else{
                        	lrutail->next = (lrunode *)malloc(sizeof(lrunode));
                                lrunode * parr = lrutail;
				lrutail = lrutail->next;
				lrutail->parent = parr;
                                lrutail->next = NULL;
                                lrutail->pid = p_copy;
                                lrutail->vpn = v_copy;
                        }

                        //THIS IS WHERE THE REPLACEMENT WILL HAPPEN WITH THEM ALL
                       
			if(pgs_in < num_pgs){
				pgs_in += 1;
				add_mem(memhead, proc_id, vpn, tbl_sz);
			}
                        else{
                        	p_copy = lruhead->pid;
                                v_copy = lruhead->vpn;
				remove_mem(memhead, p_copy, v_copy, tbl_sz);
                                add_mem(memhead, proc_id, vpn, tbl_sz);
                                lruhead = lruhead->next;
                     	}

                        blockedhead = blockedhead->next;
			continue;
		}

		if(blockedhead != NULL){
                	blockedhead->tick_count -= 1;
                        if(blockedhead->tick_count == 0){
                        	current = head;
				//might have error with this loop
                                while(strcmp(current->pid, blockedhead->pid) != 0) current = current->next;
                                current->blocked = 0;
                                fseek(tracefile, current->current, SEEK_SET);
                                fscanf(tracefile, "%s %s", proc_id, vpn);
	
                                p_copy = malloc(sizeof(proc_id) + 1);
                                v_copy = malloc(sizeof(proc_id) + 1);
                                strcpy(p_copy, proc_id);
                                strcpy(v_copy, vpn);

                                if(lruhead == NULL){
                                        lruhead = (lrunode *)malloc(sizeof(lrunode));
                                        lrutail = lruhead;
                                        lruhead->next = NULL;
					lruhead->parent = NULL;
                                        lruhead->pid = p_copy;
                                        lruhead->vpn = v_copy;
                                }
                                else{
                                        lrutail->next = (lrunode *)malloc(sizeof(lrunode));
                                        lrunode * parr = lrutail;
					lrutail = lrutail->next;
					lrutail->parent = parr;
                                        lrutail->next = NULL;
                                        lrutail->pid = p_copy;
                                        lrutail->vpn = v_copy;
                                }

                                //THIS IS WHERE THE REPLACEMENT WILL HAPPEN WITH THEM ALL
                                if(pgs_in < num_pgs){
                                        pgs_in += 1;
                                        add_mem(memhead, proc_id, vpn, tbl_sz);
                                }
                                else{
                                        p_copy = lruhead->pid;
                                        v_copy = lruhead->vpn;
                                        remove_mem(memhead, p_copy, v_copy, tbl_sz);
                                        add_mem(memhead, p_copy, v_copy, tbl_sz);
                                        lruhead = lruhead->next;
                                //SHIT
                                }

                                blockedhead = blockedhead->next;
                        }
                }

	}


	printf("Running time: %d\n", ticks);
	printf("AMU: %f\n", (float)stats_pgs_in/ticks);
	printf("sum pgs: %d\n", stats_pgs_in);
	printf("ARP: %f\n", (float)runnable_procs/ticks);
	printf("sum procs: %d\n", runnable_procs);
	printf("TMR: %d\n", lines_executed);
	printf("TPI: %d\n", pg_faults);
	free(head);
	tail = NULL;
}	

//checks to see if a vpn is in a node for the corresponding pid
//params	curr - the head node of the mem struct
//		  	pid -  the pid of the proc being set to dirty/clean
//			vpn - the vpn of the proc being set to dirty/clean
//			tbl_sz - the table size, use in finding the vpns index
//return 1 if found 0 if not
int in_mem(memnode * curr, char * pid, char * vpn, int tbl_sz){
	int vpni = atoi(vpn);
	while(curr != NULL){
		if(curr->pid == NULL) return -1; //printf("janky fix\n");

		if(strcmp(curr->pid, pid) == 0) break;
		curr = curr->next;
	}
	if(vpni == 0){
                if(curr->zero_in == 1) return 1;
                else return 0;
        }
	
	if(curr->count == 0) return 0;
	if(curr->mem[vpni % tbl_sz] == vpni) return 1;
	else if(curr->mem[vpni % tbl_sz] == 0) return 0;
	else{
		int index = ((vpni % tbl_sz) + 1) % tbl_sz;
		printf("looping\n");
		while(1){
			if(curr == NULL){
				printf("BIG SHIT\n");
			}
			if(curr->mem[index] == vpni) return 1;
			else if(curr->mem[index] == 0) return 0;
			index =  (index + 1) %  tbl_sz;		
		}
	}
}
//put a vpn into a node for the corresponding pid
//params	curr - the head node of the mem struct
//		  	pid -  the pid of the proc being set to dirty/clean
//			vpn - the vpn of the proc being set to dirty/clean
//			tbl_sz - the table size, use in finding the vpns index
//return N/A
void add_mem(memnode * curr, char * pid, char * vpn, int tbl_sz){
	int vpni = atoi(vpn);
	while(curr != NULL){
        	if(strcmp(curr->pid, pid) == 0) break;
                curr = curr->next;
        }
	if(vpni == 0){
                curr->zero_in = 1;
                return;
        }

	if(curr->mem[vpni % tbl_sz] == 0){
		curr->mem[vpni % tbl_sz] = vpni;
		curr->count += 1;
		return;
	}
	else{
		int index = ((vpni % tbl_sz) + 1) % tbl_sz;
		while(1){
			if(curr->mem[index] == 0){
				curr->mem[index] = vpni;
				curr->count += 1;
				return;
			}
			index = (index + 1) % tbl_sz;
		}
	}
}

//removes a vpn from the node for the corresponding pid
//params	curr - the head node of the mem struct
//		  	pid -  the pid of the proc being set to dirty/clean
//			vpn - the vpn of the proc being set to dirty/clean
//			tbl_sz - the table size, use in finding the vpns index
//return N/A
void remove_mem(memnode * curr, char * pid, char * vpn, int tbl_sz){
	int vpni = atoi(vpn);
        while(curr != NULL){
                if(strcmp(curr->pid, pid) == 0) break;
                curr = curr->next;
        }
	if(vpni == 0){
                curr->zero_in = 0;
                return;
        }
        if(curr->mem[vpni % tbl_sz] == vpni){
                curr->mem[vpni % tbl_sz] = 0;
                curr->count -= 1;
		return;
        }
        else{
                int index = ((vpni % tbl_sz) + 1) % tbl_sz;
                while(1){
                        if(curr->mem[index] == vpni){
                                curr->mem[index] = 0;
                                curr->count -= 1;
				return;
                        }
                        index = (index + 1) % tbl_sz;
                }
        }

}

