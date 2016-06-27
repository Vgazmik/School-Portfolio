#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/*union semun is defined by including <sys/sem.h>*/
#else
/*according to X/OPEN we have to define it ourselves*/
union semun {
	int val;               /*value for SETVAL */
	struct semid_ds *buf;  /*buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /*array for GETALL, SETALL */
			       /*Linux specific part: */
	struct seminfo *__buf; /*buffer for IPC_INFO*/
};
#endif
#ifndef _msg_
#define _msg_
#include <iostream>
#include <iomanip>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
//#define MIN_ARGS 5
//#define MAX_ARGS 7
#define READ 0
#define WRITE 1
typedef struct msg {
	long type;
	double stime;
	int id;
	int bufC;
	int before;
	int after;
}msg;
#endif
