#include "strand.h"
using namespace std;

void *search_f(void *ptr);
void graph(line g, unsigned char *m);
line critA(int start, int sz, unsigned char *c);
//double critB(int start, int sz, unsigned char *c);
//double critC(int start, int sz, unsigned char *c);;

int main(int argc, char** argv) {
	cout<<"\n*******************************************"<<endl;
	cout<<"Author: Shane Lopez\nProject: orderSearcher"<<endl;
	cout<<"*******************************************\n"<<endl;
	if(argc < 3) {
		cout<<"Usage: ./orderSearcher inputDataFile nThreads [gspeed]\n"<<endl;
		exit(1);
	}
	else if(argc == 4) {
		//cout<<argv[3]<<"\n"<<endl;
		if(strcmp(argv[3],"-s") == 0)
			speed = SL;
		else if(strcmp(argv[3],"-m") == 0)
			speed = MD;
		else if(strcmp(argv[3],"-f") == 0)
			speed = FS;
		else if(strcmp(argv[3],"-i") == 0)
			speed = IN;
		else {
			cout<<"[gspeed] options:\n\t'-s' for slow\t'-m' for moderate\t'-f' for fast\n"<<endl;
			exit(1);
		}
	}
	else
		speed = MD;
	char *fname = argv[1];
	int nT = atoi(argv[2]);
	int fsz;
	int fd;
	int cs;
	int semID;
	
	/*cout<<"Criteria Select\n"<<endl;
	cout<<"1) std dev\n2) range\n3) other\n"<<endl;
	cin >> cs;
	if(cs > 3 || cs < 1)
		while(cs > 3 || cs < 1) {
			cout<<"Criteria Select\n"<<endl;
			cout<<"1) std dev\n2) range\n3) other\n"<<endl;
			cin >> cs;
		}			
	switch(cs) {
		
		case 1: cs = STD; break;
		case 2: cs = RNG; break;
		case 3: cs = OTH; break;
	}*/ //For implementation of other criteria, not used
	
	semID = semget(IPC_PRIVATE,1,IPC_CREAT|0600);
	if(semID < 0) {
		perror("semget");
		exit(-1);
	}
	union semun s_val;
	s_val.val = 1;
	for(int t = 0; t < 1; t++) {	
		if(semctl(semID,t,SETVAL,s_val) < 0) {
			perror("semctl");
			exit(-1);
		}
	}
	
	cs = STD;
	//this stat struct will give me informtion regarding the input file
	struct stat *buf = (struct stat*)malloc(sizeof(struct stat));
	
	cout<<"\nFile name: "<<fname<<"\n# Threads: "<<nT<<"\n"<<endl;
	if(stat(fname,buf) == 0) {	
		fsz = buf->st_size;
		free(buf);
	}
	else {	
		free(buf);
		perror(fname);
		exit(1);
	}
	cout<<"File Size: "<<fsz<<"\n"<<endl;
	
	usleep((__useconds_t)(2*1000000));//Pause to show user
	
	fd = open(fname, O_RDONLY);//make a file descriptor and 
				   //prepare to sew
	//make threads
	strand send;//custom assignment struct for each thread
	strand *s;
	
	send.sid = semID;
	send.critSelect = cs;
	send.bestLoc = 0;
	send.bestFound = 0.0;
	send.fsz = fsz;
	send.map = (unsigned char*)mmap(0,fsz,PROT_READ, MAP_SHARED, fd, 0);
	// ^^ Memory map input file
	if(send.map == MAP_FAILED) {
		close(fd);
		perror("Mapping Error");
		exit(1);
	}
	int l = 0;
	int nl = 0;
	for(int j = 0; j < fsz;) {
		for(int k = 0; k < 20; k++) {
			/*if(send.map[j] < 100)
				cout<<" "<<(double)send.map[j]<<" ";
			else
				cout<<(double)send.map[j]<<" ";*/
			j++;
			if(j == fsz)
				break;
			if(k == 19) {
				l++;
				if(l%4 ==0) {
					//cout<<"\nLine "<<nl+1<<"---------------------------------------------------------------\n";
					nl++;
				}
				/*else
					cout<<"\n";*/
			}	
		}
	}// calculate number of lines of length 80
	
	if(fsz%80 > 0) {
		nl++;//if elements remain they compose an incomplete line
	}
	
	cout<<"\n#lines: "<<nl<<"\n"<<endl;
	
	send.funcs[0] = &critA;// std deviation criteria, searching for lowest
	//send.funcs[1] = &critB;// not used
	//send.funcs[2] = &critC;// not used
	
	send.bsz = fsz/nT;//how many bytes to alot each thread
	
	int remainder = 0;//nT will not always evenly divide fsz
			  //the remainder is added to the last thread's block size
	if((send.bsz * nT) < fsz) {                               // 
		remainder = (fsz - (send.bsz * nT));		  //
	}							  //
	pthread_t thr[nT];					  //
	if(pthread_rwlock_init(&send.rwlock,NULL) < 0) {	  //
		perror("pthread_rwlock_init()");		  //
		exit(0);					  //
	} //not fully utilized		         		  //
	for(int i = 0; i < nT; i++) {				  //
		pthread_t tmp;					  //
		send.start = (i*send.bsz);			  //
		send.tid = i+1;					  //
								  //
		if(i == nT-1) {					  //
			send.bsz += remainder; ////////////////////
		}
		
		strand tmp_ = send;
		s = &tmp_;
		thr[i] = tmp;

		if(pthread_create(&tmp,NULL,search_f,(void*) s) != 0) {
			cout<<"Thread Error"<<endl;
			exit(1);
		}
		
		pthread_join(tmp, NULL);
	}
	
	/*for(int k = 0; k < nT; k++) {
		pthread_join(thr[k],NULL);
	}*/
	
	cout<<"~~~~~~~~~~~~~~~~<IN MAIN>~~~~~~~~~~~~~~~~\n"<<endl;
	struct sembuf sem_op;
	
	//lock semaphore //sanity
	sem_op.sem_num = 1;
	sem_op.sem_op = -1;
	sem_op.sem_flg = 0;
	semop(semID,&sem_op,1);
	
	cout<<"Best standard deviation found: "<<bestL.dev<<"\n"<<endl;
	usleep((__useconds_t)(1000000));
	graph(bestL,send.map);
	
	//unlock semaphore //sanity
	sem_op.sem_num = 1;
	sem_op.sem_op = 1;
	sem_op.sem_flg = 0;
	semop(semID,&sem_op,1);
	
	void *add = (void*)send.map; // create void pointer for munmap	
	
	if(semctl(semID,0,IPC_RMID)) { //destroy semaphore
		perror("semctl");
		exit(1);
	}
	
	if(munmap(add,fsz) < 0) { //unmap file
		perror("munmap");
		exit(1);
	}
	//Report one final time, after graphing
	cout<<"Best standard deviation found: "<<bestL.dev<<"\n"<<endl;
	usleep((__useconds_t)(1000000));
	
	exit(0);
}

void graph(line g, unsigned char *m) {
	double val;
	char rep[80][MAXV];
	int row, col;
	
	//cout<<"MAXV: "<<MAXV<<"\n"<<endl;
	
	for(int h = 0; h < 80; h++) {
		for(int l = 0; l < MAXV; l++) {
			rep[h][l] = '-';
		}
	}//initialize graph array to all '-'
	
	row = 0;
	int a,b;
	for(int i = g.start; i <= g.end; i++ ) {
		val = m[i];
		for(int j = 0; j < MAXV; j++) {
			a = 255-(j*MAXD);
			b = 255-((j+1)*MAXD);
			if(val <= a && val > b) { // calculate column
				//cout<<b<<" < "<<val<<" <= "<<a<<"\n"<<endl;
				col = (MAXV-1)-j; 
				//cout<<val<<" at col "<<col<<" and row ";
				break;
			}
			
		}
		rep[row][col] = 'X';
		//cout<<row<<"\n"<<endl;
		row++;//increase row, graph is vertical
	}
	int nX = 0;
	for(int b = 0; b < 80; b++) {
		for(int v = 0; v < MAXV; v++) {
			if(rep[b][v] == 'X') {
				nX++;
				//cout<<"X @ map["<<b<<"]["<<v<<"]\n"<<endl;
				break;
			}
		}
	}
	
	//if(nX != g.end-g.start)
		//if(g.end != 79)
			//cout<<" Plotting Error\n"<<endl;
	
	int med = (255/2)/MAXD;
	for(int k = 0; k <= (g.end-g.start); k++) {
		if(k%10 == 0)
			for(int d = 0; d < MAXV; d++) {
				if(d == 0)
					cout<<"0";
				else if(d == MAXV-1)
					cout<<"255 <- 1 '~' equal to "<<MAXD<<"\n"<<endl;
				else if(d == med-1) {
					cout<<255/2;
					d+=2;
				}
				else
					cout<<"~";
			}
		for(int n  = 0; n < MAXV; n++) {
			cout<<rep[k][n];
			if(rep[k][n] == 'X') { 
				cout<<"\n"<<endl;
				n = 0;
				if(speed!= IN)
					usleep((__useconds_t)(1000000/speed));
				break;
			}
		}
	}//graph line elements in groups of ten
	
	for(int d = 0; d < MAXV; d++) {
		if(d == 0)
			cout<<"0";
		else if(d == MAXV-1)
			cout<<"255 <- 1 '~' equal to "<<MAXD<<"\n"<<endl;
		else if(d == med-1) {
			cout<<255/2;
			d+=2;
		}
		else
			cout<<"~";
	}
}

void *search_f(void *ptr) {
	strand *me;
	me = (strand*)ptr;
	
	int id = me->tid;// who am I?
	int st = me->start;// where to begin...
	int sz = me->bsz; // how much work to do...
	int sem = me->sid; // which semaphore to use
	
	line ret;//line to send back
	
	line(*f)(int,int,unsigned char*) = me->funcs[(me->critSelect)];
	// ^^ function to use as criteria

	cout<<"\n*******************************************"<<endl;
	cout<<"Thread "<<id<<" says hello!\n"<<endl; // say hi!
	//cout<<"t"<<me->tid<<" Block Size: "<<me->bsz<<"\n"<<endl;
	//cout<<"Starting Index: "<<st<<"\n"<<endl;
	
	struct sembuf sem_op; //for semaphore operation
	ret = f(st,sz,me->map); //call criteria evaluation function
	
	//lock semaphore
	sem_op.sem_num = 1;
	sem_op.sem_op = -1;
	sem_op.sem_flg = 0;
	semop(sem,&sem_op,1);
	
	//if best local line is "better" than current global best, 
	//replace global best with local best 
	if(ret.dev < bestL.dev || bestL.start == bestL.end){
		bestL = ret;	
		cout<<"Thread  "<<me->tid<<" reports a new best standard deviation: "<<ret.dev<<"\n"<<endl;
		usleep((__useconds_t)(1000000));
		graph(ret,me->map);
	}
	
	//unlock semaphore
	sem_op.sem_num = 1;
	sem_op.sem_op = 1;
	sem_op.sem_flg = 0;
	semop(sem,&sem_op,1);
	
	cout<<"Thread "<<id<<" says goodbye!\n"<<endl; //
	//cout<<"Criteria Function returned "<<ret.dev<<endl;
	cout<<"*******************************************\n"<<endl;
}

line critA(int start, int sz, unsigned char *c) {
	int s = start;
	int end = (start + sz) - 1;
	double dev = 0;
	double X = 0.0;
	int l = 0;
	int nl = sz/80;
	if(sz%80 > 0)
		nl++;
	double x[nl];//means of each line
	double std[nl];//std dev's of each line
	line record[nl];
	double best = 0.0;
	int bl = 0;
	//cout<<"In critA!\n"<<endl;
	//cout<<"S: "<<s<<" E: "<<end<<"\n"<<endl;
	
	//int n = 0;
		
	for(int j = s; j <= end;) {
		for(int k = 0; k < 80; k++) {	
			if(k == 0)
				record[l].start = j;
			X+=(double)c[j];
			
			if(k == 79) {
				record[l].end = j;
				X/=80.0;
				x[l] = X;
				X=0.0;
				//cout<<"\nMean of line "<<l+1<<": "<<x[l]<<"\n"<<endl;
				l++;
			}
			
			j++;		
			
			if(j == end+1) {
				record[l].end = end;
				X/=(k+1);
				x[l] = X;
				X=0.0;
				//cout<<"\nMean of line "<<l+1<<": "<<x[l]<<"\n"<<endl;
				break;
			}
		}
	}
	
	l = 0;
	double tmp = 0.0;
	double tmp2 = 0.0;
	for(int j = s; j <= end;) {
		for(int k = 0; k < 80; k++) {
			//tmp = (double)c[j] - x[l];
			//dev+=pow(tmp,2.0);
			dev+=(pow(((double)c[j]-(x[l])),2.0));
			if(k == 79) {
				tmp = dev;
				dev/=80.0;
				tmp2 = sqrt(dev);
		 		dev = tmp2;
				std[l] = dev;
				record[l].dev = std[l];
				dev=0.0;
				//cout<<"sqrt(sum(pow(Xi-X,2.0))/N)\nX = "<<x[l]<<"\nsum(pow(Xi-X,2.0)) = "<<tmp<<"\nN = 80\n"<<endl;
				//cout<<"Std. Dev. of line "<<l+1<<": "<<std[l]<<"\n"<<endl;
				l++;
			}
			j++;
			if(j == end+1) {
				tmp = dev;
				dev/=(k+1);
				tmp2 = sqrt(dev);
		 		dev = tmp2;
				std[l] = dev;
				record[l].dev = std[l];
				dev=0.0;
				//cout<<"sqrt(sum(pow(Xi-X,2.0))/N)\nX = "<<x[l]<<"\nsum(pow(Xi-X,2.0)) = "<<tmp<<"\nN = "<<k+1<<"\n"<<endl;
				//cout<<"\nStd. Dev. of line "<<l+1<<": "<<std[l]<<"\n"<<endl;
				break;
			}	
		}
	}
	
	/*for(int h = 0; h < nl; h++) {
		cout<<"Line "<<h+1<<"\nstart: "<<record[h].start<<"\nend: "
		    <<record[h].end<<"\nstd dev: "<<record[h].dev<<"\n"<<endl;
	}
	*/
	for(int m = 0; m < nl; m++) {
		//cout<<"dev of line "<<m+1<<": "<<record[m].dev<<"\n"<<endl;
		if(m == 0)
			best = record[m].dev;
		else if (record[m].dev<best) {
			best = record[m].dev;
			bl = m;
		}
	}
	
	//cout<<"best dev @ line "<<bl+1<<"\n"<<endl;
	
	//return best;
	return record[bl];
}

/*line critB(int start, int sz, unsigned char *c) {
	cout<<"In critB!\n"<<endl;
	//return 2.5;
}

line critC(int start, int sz, unsigned char *c) {
	cout<<"In critC!\n"<<endl;
	//return 3.5;
}*/
