#include "gen.h"

int main() {

	char *toks[MAX_ARGS], *cmdL[MAX_ARGS], *cmdR[MAX_ARGS];
	Case detect;
	int numArgs;
	//int * pipes = NULL;
	std::cout<<std::endl;
	while(true) {
		
		std::cout<<"$ ";

		numArgs = readIn(toks);
		
		detect = detCase(numArgs,/*pipes,*/toks,cmdL,cmdR);		

		if(detect == PIPE) 
			pipeExe(cmdL,cmdR);
		else if(detect == REDIR) {}
			/*******Code Here******/
		else 
			regExe(toks);
	}
	
}

