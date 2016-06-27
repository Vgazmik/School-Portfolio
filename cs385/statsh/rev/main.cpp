/*****************
@Author Shane Lopez, slopez23
@Project statsh
This shell executes basic pipelined commands 
and can traverse the file directory.
*****************/
#include "mysh.h" 
//Contains function and class declarations as well as a node struct
//for storing a linked list of old commands
int main() { 
	
	std::cout<<"Shane Lopez, slopez23"<<std::endl;
	
	lnode *root; //root list node
	lnode *tmp; //tracking list node
	lnode *tmp2;

	root = NULL;
	
	while(true) { //looping parse-execute cycle
		tmp = new lnode;
		char *tokens[MAX_ARGS]; //array of pointers for storing 
					//user input tokens
		hol a = hol(); //wrapper class object for returning size
		a = read(tokens,tmp); //wrapper object fed by read function
		
		//read will return a hol object with size = -1 
		//if stats is entered
		if(a.size < 0) {
			std::cout<<"\n";
			stats(root);
			std::cout<<"\n";
			continue;
		}
		if(strcmp(tokens[0],"exit") == 0 || strcmp(tokens[0],"bye") == 0) {
			std::cout<<"Goodbye!"<<std::endl;
			exit(0); //exit if desired
		}
		

		tmp->next = NULL; //building a linked list of commands
		if(root == NULL) {
			root = tmp;
		}
		else {
			tmp2 = root;
			while(tmp2->next != NULL)
				tmp2 = tmp2->next;
			tmp2->next = tmp;
		}
		
		if(strcmp(tokens[0],"cd") == 0) {
			chdir(tokens[1]);
			tmp->utime = 0.0;
			tmp->stime = 0.0;
			continue;
		}
		
		//std::cout<<tmp->cmd<<std::endl; //debug purposes
		if(a.numPipes == 0 && a.numDirs == 0)
			exe(tokens,tmp); //normal execution
		else if(a.numPipes > 0) {
			pipeExe(a,tmp);//pipeline execution
		}
		//else if(a.numDirs > 0) {
		//	redirExe(a,tmp);
		//}
	}
}
