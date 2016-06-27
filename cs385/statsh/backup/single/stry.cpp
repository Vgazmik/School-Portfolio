#define READ_END 0
#define WRITE_END 1

#include <stdio.h>
#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>



int main ()
{
  //int numPipes = 0;
  std::string input;
  char** T = NULL;
  while((input != "exit") && (input != "bye")) {
        input = "";
	std::cout<<"$ ";
        std::getline(std::cin,input);
	//std::cout<<std::endl;
        if(input == "exit" || input == "bye") {
		std::cout<<"Goodbye!"<<std::endl;
		return 0;
	}
        char * cstr = new char [input.length()+1];
	T = new char*[(input.length()/2)];
        std::strcpy (cstr, input.c_str());
        char * p = std::strtok (cstr,"|");
        int count = 0;
        while (p!=0)
        {
		T[count] = p;
                count++;
		//
		//T = realloc(T,sizeof(char*))
                //std::cout<<p<<std::endl;
                p = strtok(NULL,"|");
        }
        char * words[count+1];
	int numPipes = count - 1;
        //std::cout<<numPipes<<std::endl;
	//static int j = 0;
        //int indices[count];
        std::strcpy (cstr, input.c_str());
	p = std::strtok (cstr,"|");
	std::string strip = "";
        for(int i = 0; i < count; i++) {
                //words[i] = p;
		strip+=p;
                /*if(strcmp(words[i],"|") == 0 ) {
                        indices[j] = i;
                        j++;
                        numPipes++;
                }*/
		words[i] = p;
                p = strtok(NULL,"|");
        }

	words[count] = NULL;
	
	char * cstr2 = new char [strip.length()+1];
        std::strcpy (cstr2, strip.c_str());
        char * r = std::strtok (cstr2," ");
	
	//char * r = std::strtok(words," ");
	int sec = 0;
	while(r!=0) {
		sec++;
		//std::cout<<r<<std::endl;
		r = strtok(NULL," ");
	}
	char * cmds[sec+1];
	int d = 0;
	r = std::strtok(cstr2," ");
	for(int i = 0; i < sec; i++) {
		/*if(strcmp(r,NULL) == 0) {
			std::cout<<"NULL"<<std::endl;
			break;
		}*/
		cmds[i] = r;
		//std::cout<<r<<std::endl;
		d++;
		r = std::strtok(NULL, " ");
	}
	cmds[d] = NULL;
	
	if(numPipes == 0) {
		//char * cmds[count];
		/*for(int i = 0; i < sec; i++) {
			cmds[i] = words[i];
		}*/
		//regular execution
		pid_t pid = fork();
		if(pid < 0)
			perror("Fork Error");
		if(pid == 0) { // child
			if ((strcmp(cmds[0],"exit") == 0) || (strcmp(cmds[0],"bye") == 0))		{
				std::cout<<"Goodbye!"<<std::endl;
				return 0;
			}
			execvp(cmds[0],cmds);
			perror("Execvp Error");
			main();
		}
		else { //parent
			if ((strcmp(cmds[0],"exit") == 0) || (strcmp(cmds[0],"bye") == 0))		{
				std::cout<<"Goodbye!"<<std::endl;
				return 0;
			}
			waitpid(pid, NULL, 0);
			//input = "";
			//std::cout<<"Child Process Complete"<<std::endl;
		}

		delete[] cstr;
		delete[] cstr2;

		//continue;
	}
	/*else {
		continue;//pipe execution
	}*/
  }
}
