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
#ifndef _STRAND_
#define _STRAND_

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#define MAXF 1//3
#define MAXV (255/5)
#define MAXD 5

enum criteria{STD,RNG,OTH};
enum speed{SL = 4,MD = 8,FS = 16,IN = 0};
typedef struct line{
	int start;
	int end;
	double dev;
}line;
typedef struct strand{
	pthread_rwlock_t rwlock;//shared read/write lock
	int sid;//semaphore id;
	int tid;//thread id
	int fsz;//filesize in bytes
	int start;//starting index 
	unsigned char *map;//array of input data bytes
	int bsz;//block size of thread
	int bestLoc;//index of best found
	int critSelect;//criteria to use
	line(*funcs[MAXF])(int,int,unsigned char*);//Array of fnctn pointers
	double bestFound;//best found so far
}strand;

static line bestL;
static int speed;
#endif
