#ifndef __GETPROCS_H__
#define __GETPROCS_H__

typedef struct linkednode{
        char *pid;
        struct linkednode* next;
}linkednode;


linkednode*  usrproclist();

int getproc(char* pid);

#endif

