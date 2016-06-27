#include "gen.h"

Case detCase(int numArgs, int * p, char** tokens, char** cmdL, char** cmdR) {

	Case detect = SINGLE;
	/*******CODE HERE********/
	//int pindices[MAX_ARGS];
	//int rindices[MAX_ARGS];
	int numPipes = 0;
	int ind = 0;//
	//int pcount = 0;
 	int redirs = 0;		
	for(int i = 0; i < numArgs; i++) {
		if(strcmp(tokens[i], "|") == 0) {
			numPipes++;
			detect = PIPE;
			//pindices[pcount] = i;
			//pcount++;
			//p = count;
			ind = i;//
			break;//
		}

		//count = 0;

		else if(strcmp(tokens[i],">>") == 0) {
			detect = REDIR;
			//rindices[redirs] = i;
			redirs++;
			break;
		}	
	}
	
	if(numPipes > 0) {
		for(int i = 0; i < numArgs; i++) {
			if(strcmp(tokens[i],"|") == 0) {
				numPipes++;
			}
		}
		*p = numPipes;
		
		for(int i = 0; i < ind; i++) {
			cmdL[i] = tokens[i];
		}
		cmdL[ind] = NULL;
		std::cout<<cmdL<<std::endl;
		int c = 0;
		for(int j = ind+1; j<numArgs; j++) {
			cmdR[c] = tokens[j];
			c++;		
		}
		cmdR[c] = NULL;
		std::cout<<cmdR<<std::endl;
	}
	
	return detect;

}

int readIn(char** tokens) {
	char *cstr;
	std::string input;
	int numArgs = 0;

	while(std::cin >> input) {
		if((input == "exit") || (input == "bye")) {
			std::cout<<"Goodbye!"<<std::endl;
			exit(0);
		}
		cstr = new char[input.size()+1];
		strcpy(cstr, input.c_str());
		tokens[numArgs] = cstr;

		numArgs++;

		if(std::cin.get() == '\n')
			break;
	}
	
	tokens[numArgs] = NULL;

	return numArgs;
}

void pipeExe(char** cmdL, char** cmdR, int nP) {
	int fds[2]; // file descriptors
	int numP = nP;
	int index = 0;
	pipe(fds);
	pid_t pid;
	////////////////////////////////////////////WORK
	/*if(numP > 1) {
			while(numP > 0) {
			// child process #1
  				if (fork() == 0) {
    				// Reassign stdin to fds[0] end of pipe.
    				dup2(fds[0], 0);

    				// Not going to write in this child process, so we can close this end
    				// of the pipe.
    				close(fds[1]);

    				// Execute the second command.
    				execvp(cmdR[0], cmdR);
	
    				perror("execvp failed");

  				// child process #2
  			}
			else if ((pid = fork()) == 0) {
    				// Reassign stdout to fds[1] end of pipe.
    				dup2(fds[1], 1);

    				// Not going to read in this child process, so we can close this end
    				// of the pipe.
    				close(fds[0]);

    				// Execute the first command.
    				execvp(cmdL[0], cmdL);
    				perror("execvp failed");

  				// parent process
  			}
		 	else {
   				 waitpid(pid, NULL, 0);
			}
			//re parse cmdL and cmdR
			for(unsigned int i = 0; i < (sizeof(cmdR)/sizeof(char**)); i++) {
				if(strcmp(cmdR[i],"|") == 0) {
					index = i;
					break;
				}
			}
		
	        	char * nL[MAX_ARGS], * nR[MAX_ARGS];	
			int counter = 0;
			int nind = 0;
		
			for(unsigned int i = (index+1); i < (sizeof(cmdR)/sizeof(char**)); i++) {
				if(strcmp(cmdR[i],"|") == 0){
					nind = i;
					break;	
				}
				nL[counter] = cmdR[i];
				counter++;
			}
			nL[counter] = NULL;
			counter = 0;
			for(unsigned int i = (nind+1); i < (sizeof(cmdR)/sizeof(char**)); i++) {
				//if(strcmp(cmdR[i],"|") == 0){
				//	nind = i;
				//	break;	
				//}
				nR[counter] = cmdR[i];
				counter++;
			}
			nR[counter] = NULL;
			counter = 0;
			//delete[] cmdL;
			//delete[] cmdR;
			//char *cmdL[MAX_ARGS], *cmdR[MAX_ARGS];
			for(unsigned int i = 0; i < (sizeof(nL)/sizeof(char**)); i++) {
				cmdL[i] = nL[i];
			}
			//cmdL = nL;
			for(unsigned int i = 0; i < (sizeof(nL)/sizeof(char**)); i++) {
			cmdR[i] = nR[i];
			}
			//cmdR = nR;
		}

	}
        /////////////////////////////////////////////////////////////WORK
  	// child process #1
  	*/if (fork() == 0) {
    		// Reassign stdin to fds[0] end of pipe.
    		dup2(fds[0], 0);

    		// Not going to write in this child process, so we can close this end
    		// of the pipe.
    		close(fds[1]);

    		// Execute the second command.
    		execvp(cmdR[0], cmdR);
	
    		perror("execvp failed");

  		// child process #2
  	} 
	else if ((pid = fork()) == 0) {
    		// Reassign stdout to fds[1] end of pipe.
    		dup2(fds[1], 1);

    		// Not going to read in this child process, so we can close this end
    		// of the pipe.
    		close(fds[0]);

    		// Execute the first command.
    		execvp(cmdL[0], cmdL);
    		perror("execvp failed");

  		// parent process
  	} 
	else {
		//pid = wait4(pid,NULL,WNOHANG,rusage);
		waitpid(pid, NULL, 0);
		//wait(NULL);
	}	
}

void regExe(char** tokens) {
	pid_t pid = fork();
	struct rusage sys_res;		
	double usrt = 0.0;
	double syst = 0.0;
		if(pid < 0) {
			perror("fork Error");
		}
		else if(pid == 0) {
			execvp(tokens[0],tokens);
			perror("execvp Error");
			exit(0);
		}
		else {
			//waitpid(pid, NULL, 0);
			wait4(pid,NULL,0,&sys_res);
			usrt = (sys_res.ru_utime.tv_sec + (sys_res.ru_utime.tv_usec / 1000000.0));
			syst = (sys_res.ru_stime.tv_sec + (sys_res.ru_stime.tv_usec / 1000000.0)); 
			std::cout<<usrt<<std::endl;
			std::cout<<syst<<std::endl;
		}	
}	
