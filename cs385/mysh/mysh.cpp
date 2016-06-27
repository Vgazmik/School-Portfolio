/***********************
@Author Shane Lopez, slopez23
@Project statsh

Function definitions for statsh
***********************/
#include "mysh.h" //custom header
#include <iomanip> //formatted output
#define READ 0 //redirection
#define WRITE 1 //redirection

hol::hol() { 
        size = 0;
        arg_ = NULL;
	numPipes = 0;
	numDirs = 0;
	rawIn = "";
}
// wrapper object constructor

// returns a wrapper object containing tokens and input size information
hol read(char** tokens, lnode* head) {
	hol t = hol();
	char *cstr;
	std::string in;
	int numArgs = 0;
	int count = 0;
	char path[128];
	
	std::cout<<getcwd(path,128)<<"$$ "; //prints "path$$ "
	while(std::cin >> in) {
		if(in == "stats"){
			t.size = -1;
			t.arg_ = NULL;
			return t;
		} 

		head->cmd += (in + " ");
		cstr = new char[in.size()+1];
		strcpy(cstr, in.c_str());
		tokens[numArgs] = cstr;
		t.rawIn += (in + " ");
		if(in == "|") {
			t.numPipes++;
			t.pipedx[count] = numArgs;
			count++;
		}
		if(in == ">" || in == "<") {
			t.numDirs++;
		}
		numArgs++;

		if(std::cin.get() == '\n')
			break;
	}
		/*for(int i = 0; i < numArgs; i++) {
			std::cout<<tokens[i]<<std::endl;
		}*/

	tokens[numArgs] = NULL;
	t.size = numArgs;
	t.arg_=tokens;
	return t;
}
void exe(char **tokens, lnode *t) { //standard fork-execute procedure
	pid_t pid;
	struct rusage sys_res;
	double usrt = 0.0;
	double syst = 0.0;
	
	pid = fork();

	if(pid < 0) {
		perror("FORK");
	}
	else if(pid == 0) {
		execvp(tokens[0],tokens);
		perror("EXEC");
	}
	else {
		wait4(pid,NULL,0,&sys_res);
		usrt = (sys_res.ru_utime.tv_sec + (sys_res.ru_utime.tv_usec / 1000000.0));
		syst = (sys_res.ru_stime.tv_sec + (sys_res.ru_stime.tv_usec / 1000000.0)); 
		//std::cout<<"user time: "<<usrt<<std::endl;
		//std::cout<<"system time: "<<syst<<std::endl;
		t->utime = usrt;
		t->stime = syst;
	}
}
void pipeExe(hol wrap, lnode *t) { //pipeline execution procedure, each pipe is
				   //made as it is needed and closed when not
                                   //in use.
	
	struct rusage sys_res;
	double usrt = 0.0;
	double syst = 0.0;
	
	char *cstr;
	char *p;
	char **cmds;
	int i = 0;
	int pipes = wrap.numPipes;
	pid_t pids[(pipes+1)];
	cmds = new char *[(pipes+1)];	
	cstr = new char[((wrap.rawIn.size()) + 1)];
	strcpy(cstr, wrap.rawIn.c_str());	
	p = strtok(cstr,"|");
	while(p != NULL) {
		cmds[i] = p;
		p = strtok(NULL,"|");
		i++;	
	}
	/*for(int j = 0; j < (pipes+1); j++) {
		std::cout<<cmds[j]<<std::endl;
	}*/
	i = 0;
	int oldfs[2] = {-1,-1};
	int newfs[2] = {-1,-1};
	
	//std::cout<<"# pipes: "<<pipes<<std::endl;
	while(i < pipes+1){
		p = trim(cmds[i]);
		char *a;
		char *arg[MAX_ARGS];
		int j = 0;
		a = strtok(p," ");
		while(a != NULL) {
			arg[j] = a;
			a = strtok(NULL," ");
			j++;
		}
		arg[j] = NULL;
		//std::cout<<p<<std::endl;
		if(i < pipes) {
			pipe(newfs);
		}
		pids[i] = fork();
		if(pids[i] < 0)
			perror("Forking Error")	;
		else if(pids[i] == 0) {

			if(i > 0) {
				dup2(oldfs[0],READ);
				close(oldfs[0]);
				close(oldfs[1]);
			}
			if(i < pipes) {
				close(newfs[0]);
				dup2(newfs[1],WRITE);
				close(newfs[1]);
			}
			
			execvp(arg[0],arg);
		}
		else {
			if(i > 0) {
				close(oldfs[0]);
				close(oldfs[1]);
			}
			if(i < pipes) {
				oldfs[0] = newfs[0];
				oldfs[1] = newfs[1];
				newfs[0] = -1;
				newfs[1] = -1;
			}
		}
		
		i++;	
	}
	
	for(int n = 0; n < (pipes+1); n++) {
		wait4(pids[n],NULL,0,&sys_res);
		usrt += (sys_res.ru_utime.tv_sec + (sys_res.ru_utime.tv_usec / 1000000.0));
		syst += (sys_res.ru_stime.tv_sec + (sys_res.ru_stime.tv_usec / 1000000.0)); 
	}
	
	t->utime = usrt;
	t->stime = syst;
	
}
void redirExe(hol wrap, lnode* item) { //redirect command procedure, incomplete
	//int rds[2];
	int i = 0;
	//int y = 0;
	//int rd;
	//char c;
	//pid_t pid;
	char *cstr;
	char *p;
	char**redirs;
	//cmds = new char*[];
	cstr = new char[((wrap.rawIn.size()) + 1)];
	strcpy(cstr, wrap.rawIn.c_str());	
	p = strtok(cstr,"<>");
	while(p != NULL) {
		//cmds[i] = p;
		p = strtok(NULL,"<>");
		i++;	
	}
	redirs = new char*[i];
	i = 0;
	strcpy(cstr, wrap.rawIn.c_str());
	p = strtok(cstr,"<>");
	while(p != NULL) {
		redirs[i] = p;
		p = strtok(NULL,"<>");
		i++;	
	}
	redirs[i] = NULL;
	
	for(int j = 0; j < i; j++) {
		//std::cout<<redirs[j]<<std::endl;
		redirs[j] = trim(redirs[j]);
		std::cout<<redirs[j]<<std::endl;
	}
	i = 0;
	char **outs;
	outs = new char*[(wrap.numDirs+1)];
	strcpy(cstr, wrap.rawIn.c_str());
	p = strtok(cstr,">");
	while(p != NULL) {
		outs[i] = p;
		p = strtok(NULL,">");
		i++;	
	}
	outs[i] = NULL;
	for(int j = 0; j < i; j++) {
		outs[j] = trim(outs[j]);
	}
	
	i = 0;
	char **ins;
	ins = new char*[(wrap.numDirs+1)];
	strcpy(cstr, wrap.rawIn.c_str());
	p = strtok(cstr,"<");
	while(p != NULL) {
		ins[i] = p;
		p = strtok(NULL,"<");
		i++;	
	}
	ins[i] = NULL;
	for(int j = 0; j < i; j++) {
		ins[j] = trim(ins[j]);
	}
		
	//pipe(rds);
		
	if(outs[0] == redirs[0]) {
		std::cout<<redirs[0]<<" > "<<redirs[1]<<std::endl;
		
	}
	
	
}
void stats(lnode* head) { // stats funcitonality, iterates through linked list
                          // and prints stored name and execution time of 
                          // each command
	if(head == NULL) {
		std::cout<<"You haven't done anything yet!"
		<<std::endl;
		return;
	}

	lnode* tmp;	
	tmp = head;
	
	while(tmp != NULL) {
		std::cout<<std::setw(40)<<std::left<<tmp->cmd<<" user time: "
		<<std::setprecision(3)<<tmp->utime<<" sys time: "
		<<std::setprecision(3)<<tmp->stime<<std::endl;
		tmp = tmp->next;
	}
}
char *trim(char *token) { //this function trims leading and trailing whitespace
                          //from a char*, used to clean input for execvp
	size_t length = 0;
	char *fp = token - 1;
	char *ep = NULL;
	
	if(token == NULL)
		return NULL;
	if(token[0] == '\0')
		return token;
	length = strlen(token);
	ep = token + length;
	
	while(isspace(*(++fp)));
	while(isspace(*(--ep)) && ep != fp);
	
	if(token + length - 1 != ep)
		*(ep + 1) = '\0';
	else if(fp != token && ep == fp)
		*token = '\0';
	ep = token;
	if(fp != token)
	{
		while(*fp) *ep++ = *fp++;
		*ep = '\0';
	}
	return token;
}
