#ifndef _GEN_
#define _GEN_

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>
#include <cstring>

const int MAX_ARGS = 256;

enum Case {PIPE, REDIR, SINGLE};

Case detCase(int,/*int*,*/char**,char**,char**);

int readIn(char**);

void pipeExe(char**,char**);

void regExe(char**);
#endif
