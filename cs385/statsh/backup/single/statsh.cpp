// strings and c-strings
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

enum pipeRedirect {PIPE, REDIRECT, NEITHER};

int main ()
{
  pipeRedirect pr = NEITHER;
  int numPipes = 0;

  std::string input = "";	
  while(input != "exit") {  
  	std::cout<<"\n$ ";
  	std::getline(std::cin,input);
  	if(input == "exit")
		exit(EXIT_FAILURE);
	char * cstr = new char [input.length()+1];
  	std::strcpy (cstr, input.c_str());
  	char * p = std::strtok (cstr," ");
	int count = 0;
  	while (p!=0)
  	{
		count++;
		std::cout<<p<<std::endl;
   	 	p = strtok(NULL," ");
  	}
	char * words[count];
	static int j = 0;
	int indices[count];
	p = std::strtok (cstr," ");
	std::strcpy (cstr, input.c_str());	
	for(int i = 0; i < count; i++) {
		words[i] = p;
		if(strcmp(words[i],"|") == 0 ) {
			indices[j] = i;
			j++;
			numPipes++;
		}
		p = strtok(NULL," ");
	}
	
	if(numPipes > 0) {
		pr = PIPE;
		std::cout<<indices[0]<<std::endl;
		char * left[((indices[0])+1)];
		char * right[(count-(indices[0]))];
		int holder = 0;

		std::cout<<"size of left cmd: "<<(indices[0]+1)<<std::endl;
		std::cout<<"size of right cmd: "<<(count - indices[0])<<std::endl;

		for(int i = 0; i < indices[0]; i++) {
			if(strcmp(words[i],"|") != 0)
			{
				left[holder] = words[i];
				holder++;
			}
		}
		left[holder] = NULL;
		holder = 0;
		for(int i = (j + 1); i < count; i++) {
			right[holder] = words[i];
			holder++;
		}
		right[holder] = NULL;
		
		int fds[2];
		pipe(fds);
		pid_t pid;

		if (fork() == 0) {
			dup2(fds[0], 0);
			close(fds[1]);
			//std::cout<<right[0]<<std::endl;
			//execvp(right[0], right);
			execvp(left[0],left);
			perror("fuck1"); 
		}
		else if ((pid = fork()) == 0) {
			dup2(fds[1],1);
			close(fds[0]);
			//std::cout<<left[0]<<std::endl;
			//execvp(left[0],left);
			execvp(right[0],right);
			perror("fuck2");
		}
		else {
			waitpid(pid, NULL, 0);
		}
		//break;	
		
	}
	
	/*if (pr == PIPE) {
		
	}*/
	
	pid_t pid = fork();
	if(pid == 0) {
		if(words[0] == "exit")
			exit(EXIT_FAILURE);
		execvp(words[0],words);
	}
	else {
		wait(NULL);
		std::cout<<"Child Process Complete"<<std::endl;
	}
	
  	delete[] cstr;	
  }
  // cstr now contains a c-string copy of str
  return 0;
}
