#ifndef _MASTER_
#include "msg.h"
#define MIN_ARGS 5
#define MAX_ARGS 7

float randF(float min, float max, int seed) {
	static int used = 0; //Determines whether to seed

	if(used == 0) {
		if(seed == -1)
			srand(time(NULL));
		else
			srand(seed);
		used++;
	}	
	
	float random = ((float)rand()) / (float)RAND_MAX;	
	float range = max - min;
	//std::cout<<used<<std::endl; // Debuggery
	return (random*range) + min;
}

int main(int argc, char **argv) {
	if(argc < MIN_ARGS || argc > MAX_ARGS) {
		std::cout<<"Usage:\n./master nBuffers nWorkers "
			 <<"sleepMin sleepMax [random seed] [-lock or -nolock]"
			 <<std::endl;
		
		return -1; //Too many or too few command line arguments
	}
	int nBuffers;
	int nWorkers;
	float sleepMin;
	float sleepMax;
	int randSeed;
	bool lock = false;
	int semID;
	int thing1[2]; // pipe 1 file descriptor
	int thing2[2]; // pipe 2 file descriptor
	pid_t pid; //pid for forked sort process
	float random; // random float value, Ahhh!
	char buffer[80]; // buffer for write/read		
		
	/*******************Variable Initialization*******************/
	nBuffers = atoi(argv[1]);
	nWorkers = atoi(argv[2]);
	sleepMin = atof(argv[3]);
	sleepMax = atof(argv[4]);
	float arr2[nWorkers]; // holder of rand ID's in sorted order
	if(argc >= 5)
		randSeed = atoi(argv[5]);
	else
		randSeed = -1;
	if(argc == 7)
		if(strcmp(argv[6],"-lock") == 0) {
			lock = true;
			//std::cout<<"\nLocked"<<std::endl;
		}
		else if(strcmp(argv[6],"-nolock") == 0) {
			lock = false;
			//std::cout<<"\nUnlocked"<<std::endl;
		}
		else {
			lock = false;
			//std::cout<<"\nDefault: Unlocked"<<std::endl;
		}
	/*******************Variable Initialization*******************/

	if(nBuffers < 0 || sleepMin < 0 || sleepMax < 0 
                        || sleepMin > sleepMax || nBuffers > 32
			|| nWorkers > nBuffers) //Data check
		return -1; 
	for(int i = 1; i < 10; i++) {
		if((i > 1) && (i != nBuffers) && (nBuffers % i == 0)) 
			return -1; //nBuffers is not a positive prime < 32
	}
	/********Sanity Check**********
	std::cout<<"\nBuffers: "<<nBuffers<<std::endl;
	std::cout<<"Workers: "<<nWorkers<<std::endl;
	std::cout<<"SleepMin: "<<sleepMin<<std::endl;
	std::cout<<"SleepMax: "<<sleepMax<<std::endl;
	std::cout<<"Rand Seed: "<<randSeed<<std::endl;
	std::cout<<"Locked: "<<lock<<std::endl;
	*******************************/
	//header();
	std::cout<<"\nShane Lopez, slopez23"
		 <<"\n\nIPC\n\nSimulating with "<<nBuffers<<" Buffers and "
		 <<nWorkers<<" Workers"<<std::endl;
	std::cout<<"-----------------------------------------------"<<std::endl;
	
	if(pipe(thing1) < 0 || pipe(thing2) < 0) { // Create Pipes
		std::cerr << "Piping Error" << std::endl;
		return -1;
	}
		
	if((pid = fork()) < 0) { // Fork
		std::cerr << "Forking Error" << std::endl;
		return -1;
	}
	else if(pid == 0) { // Child
		close(thing1[WRITE]); // close unused pipe ends
		close(thing2[READ]);
		
		dup2(thing1[READ],READ); // dup2 appropriate ends
		dup2(thing2[WRITE],WRITE);
		
		execlp("sort","sort","-nr",NULL); // execute sort
	}
	else { // Parent
		close(thing1[READ]); // close unused pipe ends
		close(thing2[WRITE]);
		
		for(int j = 0; j < nWorkers; j++) {
			random = randF(sleepMin,sleepMax,randSeed); 
			//std::cout<<"Random "<< j <<": "<<random<<std::endl;
			if(j == 0)
				sprintf(buffer,"%f\n",random);
			else
				sprintf((buffer+strlen(buffer)),"%f\n",random);
		}//Fills Buffer
				
		write(thing1[WRITE],buffer,strlen(buffer));
		
		/*std::cout<<"\nRandom Floats: \n";//<<buffer<<std::endl;
		for(int h = 0; h < strlen("Random Floats: "); h++)
			std::cout<<"-"; // Fancy
		std::cout<<std::endl; // Take it down a notch
		std::cout<<buffer<<std::endl;*/
		close(thing1[WRITE]); // Close pipe end after use	
		
		read(thing2[READ],buffer,strlen(buffer));
		
		close(thing2[READ]); // Close pipe end after use
		
		char *p;
		p = strtok(buffer,"\n"); // tok'n on that str
		//std::cout<<p<<std::endl; // debuggery
		/*std::cout<<"\n{Sorted} Random Floats: \n";
		for(int h = 0; h < strlen("{Sorted} Random Floats: "); h++)
			std::cout<<"-";	// Fancy
		std::cout<<std::endl;*/
		for(int j = 0; j < nWorkers; j++) { //prints and fills array
			arr2[j] = atof(p);
			p = strtok(NULL,"\n");
			//std::cout<</*std::setprecision(4)<<*/arr2[j]<<std::endl;
		}
		//std::cout<<"\n"<<std::endl;	
	}
	/**************************Begin Msg Q*****************************/	
	
	int msgQID;
	msg sendMsg;
	msg recMsg;
	
	if((msgQID = msgget(IPC_PRIVATE,IPC_CREAT | 0600)) == 0) {
		perror("msgget");
		exit(-1);
	}
	/**************************Fork Exec*******************************/
	pid_t IDs[nWorkers];
	struct rusage sys_res;
	struct msqid_ds MSG;
	struct shmid_ds SHM;
	int shmID;
	
	shmID = shmget(IPC_PRIVATE,sizeof(int)*nBuffers,0600 | IPC_CREAT);
	int *shm_ptr = (int*)shmat(shmID,NULL,0);
	
	for(int p = 0; p < nBuffers; p++) {
		shm_ptr[p] = 0;
	}

	shmdt(shm_ptr);
	
	if(lock) {
		semID = semget(IPC_PRIVATE,nBuffers,IPC_CREAT|0600);
		if(semID < 0) {
			perror("semget");
			exit(-1);
		}
		union semun s_val;
		s_val.val = 1;
		for(int t = 0; t < nBuffers; t++) {	
			if(semctl(semID,t,SETVAL,s_val) < 0) {
				perror("semctl");
				exit(-1);
			}
		}
	}
		
	for(int i = 0; i < nWorkers; i++) {
		pid_t pid = fork();
		IDs[i] = pid;
		if(pid < 0) {
			perror("fork");
			exit(-1);
		}	
		else if(pid == 0) { // execute worker
			char a[10];
			char b[10];
			char c[10];
			char d[10];
			char e[10];
			
			sprintf(a,"%d\n",(i+1));
			sprintf(b,"%d\n",nBuffers);
			sprintf(c,"%f\n",(arr2[i]));
			sprintf(d,"%d\n",msgQID);
			sprintf(e,"%d\n",shmID);
			if(lock) {
				char f[10];
				sprintf(f,"%d\n",semID);
				execlp("./worker","worker",a,b,c,d,e,f,NULL);
			}	
			else
				execlp("./worker","worker",a,b,c,d,e,NULL);
		}
	}
	//implicitly in parent
	int children = nWorkers;
	int r_err = 0;
	int w_err = 0;
	size_t SZ = (size_t)(sizeof(struct msg) - sizeof(long int));
	msg* recm = (msg*)malloc(sizeof(msg));
	////////////////////
	while(children > 0) {
		if(msgrcv(msgQID,recm,30,-4,0) < 0) {
			perror("msgrcv");
			exit(-1);
		}
		//std::cout<<recm->id<<std::endl;
		switch(recm->type) {
			/*case 4: std::cout<<"\n[Master]--Worker "<<recm->id
					 <<" reported Write error in buffer "
					 <<recm->bufC<<"- before sleep: "
					 <<recm->before<<" after sleep: "
					 <<recm->after<<std::endl;
				//w_err++;
				break;*/
			case 3: std::cout<<"\n[Master]--{Worker "<<recm->id
					 <<"} reported Read error in buffer "
					 <<recm->bufC<<"- before sleep: "
					 <<recm->before<<" after sleep: "
					 <<recm->after<<std::endl;
				r_err++;
				//wait4(IDs[((recm->id)-1)],NULL,0,&sys_res);
				break;
			case 2:	std::cout<<"\n[Master]--{Worker "<<recm->id
					 <<"} says bye!"<<std::endl;
				wait4(IDs[((recm->id)-1)],NULL,0,&sys_res);
				children--;
				break;
			case 1: std::cout<<"\n[Master]--{Worker "<<recm->id
					 <<"} says hello!"<<std::endl;
				std::cout<<"Sleep time for worker "<<recm->id
			                 <<": "<<recm->stime<<std::endl;
				break;
			default: std::cout<<"\n[Master]--<Unspecified msg type>"
					  <<std::endl;
				break;
		}
			
	}
	
	std::cout<<"\n"<<std::endl;
	
	shm_ptr = (int*)shmat(shmID,NULL,0);
	int desired = (pow(2,nWorkers)-1);
	//int w_err = 0;
	
	std::cout<<"\n[Master]--<Reporting Buffer Contents>\n"<<std::endl;
	for(int p = 0; p < nBuffers; p++) {
		if(p > 9)
			std::cout<<"Buffer "<<p<<": "<<shm_ptr[p]<<std::endl;
		else
			std::cout<<"Buffer  "<<p<<": "<<shm_ptr[p]<<std::endl;
		if(shm_ptr[p] != desired)
			w_err++;
		
	}
	std::cout<<std::endl;
	int *badB = new int[w_err];
	int counter = 0;
	
	for(int p = 0; p < nBuffers; p++) {
		if(shm_ptr[p] != desired) {
			badB[counter] = p;
			counter++;
			if(counter == w_err)
				break;
		}
	}
	
	int examine;
	int sieve;
	int b;
	int *b_bits = new int[32];
	int c = 0;
	int f = 0;
	int werr = 0;
	for(int r = 0; r < 32; r++) {
		b_bits[r] = -1;
	}
	
	for(int g = 0; g < w_err; g++) {
		b = badB[g];
		examine = shm_ptr[b];
		/////////////////////////////////////////////////////
		if((badB[g]) > 9)
			std::cout<<"\tError in buffer "<<b<<": "<<examine<<" != "
				 <<desired<<std::endl;
		else
			std::cout<<"\tError in buffer  "<<b<<": "<<examine<<" != "	
				 <<desired<<std::endl;
		/////////////////////////////////////////////////////
		//Examine bits
		sieve = (examine ^ desired);//sift the bad bits
		for(int v = 0; v < 32; v++) {
			if((sieve & (1 << v)) >= 1) {
				b_bits[c] = v;
				c++;
			}
		}
		//w_err += c;
		//werr+=c;
		std::cout<<"\t\tBad Bits: ";
		while(f < c) {
			if(f < (c-1))
				std::cout<<"bit "<<b_bits[f]<<", ";
			else
				std::cout<<"bit "<<b_bits[f]<<" ";
			werr++;
			f++;
		}
		std::cout<<"\n"<<std::endl;
		
	}

	std::cout<<"\n"<<r_err<<" Read Errors "<<werr
		 <<" Write Errors"<<std::endl;

	shmdt(shm_ptr);	
	
	if(lock)
		if(semctl(semID,0,IPC_RMID)) {
			perror("semctl");
			exit(-1);
		}
	
	if(shmctl(shmID,IPC_RMID,0) < 0) {
		perror("shmctl");
		exit(-1);
	}
		
	if(msgctl(msgQID,IPC_RMID,0) < 0) { //close message Q
		perror("msgctl");
		exit(-1);
	}
	
	std::cout<<"-----------------------------------------------"<<std::endl;	
	exit(0);
}
#define _MASTER_
#endif
