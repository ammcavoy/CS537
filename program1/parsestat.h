#ifndef __PARSESTAT_H__
#define __PARSESTAT_H__


FILE* getstatfile(char* pid);

FILE* getstatmfile(char* pid);

FILE* getcmdlinefile(char* pid);

char* getfield(FILE* filename, int k);

char* getstate(char* pid);

char* getsystime(char* pid);

char* getusrtime(char* pid);

char* getvirtmem(char* pid);

char* getcmdline(char* pid);

#endif
