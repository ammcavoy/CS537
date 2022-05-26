#ifndef __PARSECMDLINE_H__
#define __PARSECMDLINE_H__

//Struct used to hold the flags for the options that can be used in call
typedef struct params{
        int p;      // 1 if -p used
        char* pid;      // the PID 
        int state;      // 1 if -s used
        int usr_time;   // 1 if -U used
        int sys_time;   // 1 if -S used
        int virt_mem;   // 1 if -v used
        int cmd_line;   // 1 if -c used
        int fail;       // 1 if the input is not valid
}params;        


/*
 * Parses command line setting flags for specific options
 */
params* parseCMDLine(int, char**);

#endif
