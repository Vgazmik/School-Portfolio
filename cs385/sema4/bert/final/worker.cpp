/*
Author: Shane Lopez
Course: CS385
Project: 2, IPC
*/
#ifndef _WORKER_
#include "msg.h"
#define MIN 5
#define MAX 7
#pragma GCC diagnostic ignored "-Wwrite-strings" 

int main(int argc, char **argv) {
	if(argc < MIN || argc > MAX)
		return -1;
	int workerID = atoi(argv[1]);//take in args passed by master
	int nBuffers = atoi(argv[2]);
	float sleepTime = atof(argv[3]);
	int msgID = atoi(argv[4]);
	int shmID = atoi(argv[5]);
	int semID;
	bool lock = false;
	if(argc == MAX) {
		semID = atoi(argv[6]);
		lock = true;
	}
	if(nBuffers < 0 || sleepTime < 0.0)
		return -1;
	for(int i = 1; i < 10; i++) {
		if((i > 1) && (i != nBuffers) && (nBuffers % i == 0)) 
			return -1; //nBuffers is not a positive prime
	}//Data check
	
	msg* m = (msg*)malloc(sizeof(msg));//create hello mesage
	m->type = 1;
	m->id = workerID;
	m->stime = sleepTime;
	if(msgsnd(msgID,m,30,0) < 0) {//send hello message
		perror("msgsnd");
		exit(-1);
	}
	/////////////////////////Hello message sent
	int *shm_ptr = (int*)shmat(shmID,NULL,0);//Attach to shared memory
	struct sembuf sem_op;//prepare for semaphore operation
	int j = workerID;
	int bSlp;
	int aSlp;
	int writes = 0;
	int add = (1 << (workerID-1));
	msg* d = (msg*)malloc(sizeof(msg));//create read_error message
	d->type = 3;
	d->id = workerID;
	d->stime = sleepTime;
	
	while(writes < nBuffers) {	
		//First two accesses are Reads, third is Write
		//First Read
		if(lock) { //lock semaphore
			sem_op.sem_num = j;
			sem_op.sem_op = -1;
			sem_op.sem_flg = 0;
			semop(semID,&sem_op,1);
		}
		bSlp = shm_ptr[j];
		//std::cout<<bSlp<<std::endl;
		usleep((__useconds_t)(sleepTime*1000000));
		aSlp = shm_ptr[j];
		//std::cout<<aSlp<<std::endl;
		if(bSlp != aSlp) { //if value changed, report to msg Q
			d->bufC = j;
			d->before = bSlp;
			d->after = aSlp;
			std::cout<<"\n\t+-{Worker "<<workerID
				 <<"} reporting Read error in buffer "<<j
				 <<": "<<bSlp<<" -> "<<aSlp<<std::endl;
			if(msgsnd(msgID,d,30,0) < 0) {//send report of change
				perror("msgsnd");
				exit(-1);
			}
			d->before = -1;
			d->after = -1;
		}
		if(lock) { //unlock semaphore
			sem_op.sem_num = j;
			sem_op.sem_op = 1;
			sem_op.sem_flg = 0;
			semop(semID,&sem_op,1);
		}		
		//increment j accordingly
		j += workerID;
		if (j >= nBuffers) 
			j = j%(nBuffers);
		//Second Read
		if(lock) {
			sem_op.sem_num = j;
			sem_op.sem_op = -1;
			sem_op.sem_flg = 0;
			semop(semID,&sem_op,1);
		}
		bSlp = shm_ptr[j];
		//std::cout<<bSlp<<std::endl;
		usleep((__useconds_t)(sleepTime*1000000));
		aSlp = shm_ptr[j];
		//std::cout<<aSlp<<std::endl;
		if(bSlp != aSlp) { //if value changed, report to msg Q
			d->bufC = j;
			d->before = bSlp;
			d->after = aSlp;
			std::cout<<"\n\t+-{Worker "<<workerID
				 <<"} reporting Read error in buffer "<<j
				 <<": "<<bSlp<<" -> "<<aSlp<<std::endl;
			if(msgsnd(msgID,d,30,0) < 0) {//send report of change
				perror("msgsnd");
				exit(-1);
			}
			d->before = -1;
			d->after = -1;
		}
		if(lock) { //unlock semaphore
			sem_op.sem_num = j;
			sem_op.sem_op = 1;
			sem_op.sem_flg = 0;
			semop(semID,&sem_op,1);
		}
		//increment j accordingly
		j += workerID;
		if (j >= nBuffers) 
			j = j%(nBuffers);
		//First Write
		if(lock) {
			sem_op.sem_num = j;
			sem_op.sem_op = -1;
			sem_op.sem_flg = 0;
			semop(semID,&sem_op,1);
		}
		bSlp = shm_ptr[j];
		//std::cout<<bSlp<<std::endl;
		usleep((__useconds_t)(sleepTime*1000000));
		aSlp = bSlp + add;
		//std::cout<<aSlp<<std::endl;
		shm_ptr[j] = aSlp;
		//report write
		std::cout<<"\n*-{Worker "<<workerID<<"} wrote to buffer "
			 <<j<<": "<<bSlp<<" + "<<add<<" -> "<<aSlp<<std::endl;
		//detect write error
		//usleep((sleepTime*1000000));//testing, testing
		/*if(shm_ptr[j] != aSlp) {//send write error msg
			d->bufC = j;
			d->before = aSlp;
			d->after = shm_ptr[j];
			d->type = 4;
			std::cout<<"\n\t{Worker "<<workerID
				 <<"} reporting Write error in buffer "<<j
				 <<": "<<aSlp<<" -> "<<d->after<<std::endl;
			if(msgsnd(msgID,d,30,0) < 0) {//send report of change
				perror("msgsnd");
				exit(-1);
			}
			d->before = -1;
			d->after = -1;
			d->type = 3;
		}*/
		writes++;
		if(lock) { //unlock semaphore
			sem_op.sem_num = j;
			sem_op.sem_op = 1;
			sem_op.sem_flg = 0;
			semop(semID,&sem_op,1);
		}
		//increment j accordingly
		j += workerID;
		if (j >= nBuffers) 
			j = j%(nBuffers);
		//Rinse and Repeat
	}	
	
	shmdt(shm_ptr);//Detach from shared memory
	/////////////////////////
	msg* l = (msg*)malloc(sizeof(msg));//create goodbye message
	l->type = 2;
	l->id = workerID;
	l->stime = sleepTime;
	if(msgsnd(msgID,l,30,0) < 0) {//goodbye message sent
		perror("msgsnd");
		exit(-1);
	}
	
	exit(0);//all done!
	
	/*std::cout<<"---------------------------------------"<<std::endl;	
	std::cout<<"\n"<<"Worker: "<<workerID<<std::endl;
	std::cout<<"#Bufs: "<<nBuffers<<std::endl;
	std::cout<<"Sleep: "<<sleepTime<<std::endl;
	std::cout<<"msgQID: "<<msgID<<std::endl;
	//std::cout<<shmID<<std::endl;
	std::cout<<"---------------------------------------"<<std::endl;*/

}
#define _WORKER_
#endif
