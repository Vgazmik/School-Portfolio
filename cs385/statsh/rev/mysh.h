/******************
@Author Shane Lopez, slopez23
@Project statsh 
header file, includes needed libraries 
and function declarations.
******************/
#ifndef _MYSH_
#define _MYSH_

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/resource.h>

const int MAX_ARGS = 256;

struct lnode { //linked list sruct
	std::string cmd;
	double utime;
	double stime;
        lnode *next;
};
class hol { //wrapper object class
        public:
                int size;
                char **arg_;
		int numPipes;
		int numDirs;
		int pipedx[MAX_ARGS];
                std::string rawIn;
		hol();
};

hol read(char**,lnode*);
void exe(char**,lnode*);
void pipeExe(hol,lnode*);
void redirExe(hol,lnode*);
void stats(lnode*);
char* trim(char*);
#endif
