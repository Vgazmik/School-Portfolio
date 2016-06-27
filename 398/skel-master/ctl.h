#pragma diagnostic ignored "[-Wwrite-strings]"

#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

FILE *fp;

std::string f = "outline.c";
std::string o = "proto";
std::string x = "./proto";

struct lib{
	std::string name;
	bool        cst;
};

struct fn{
	std::string dec;
	int         len;
	std::string type;
};

std::string cname = "conf";    //config file name
std::string hname;
std::string fname;
std::vector<lib> libs;         //headers to include
std::vector<fn> fncs;          //function declarations

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

void type() {
	
	fprintf(fp,"int main(int argc, char** argv)\n");
	fprintf(fp,"{\n\tprintf(\"\\nHello, Master!\\n\\n\");\n\n\treturn 0;\n");
	fprintf(fp,"}\n");
}

void compile() 
{
	execlp("gcc","gcc","-o",o.c_str(),f.c_str(),NULL);
	printf("Error: Compilation\n");
	exit(1);
}

void run()
{
	execlp(x.c_str(),o.c_str(),NULL);
	printf("Error: Execution\n");
	exit(1);
}

void ftype(fn *w) {
	
	char *arr = new char[w->dec.length()+1];
	strcpy(arr,w->dec.c_str());
	char *p;
	
	p = strtok(arr," ");
	w->type = std::string(trim(p));
	//printf("%s\n",p);
}

void wh(lib l) {
	
	if(l.cst) 
		fprintf(fp,"#include \"%s\"\n",l.name.c_str());
	else
		fprintf(fp,"#include <%s>\n",l.name.c_str());
}
void wf(fn u) {
	
	if(strstr(u.dec.c_str(),"main"))
		type();
	else {
		ftype(&u);
		//printf("Header: %s\nType: %s\n",u.dec.c_str(),u.type.c_str());
		if(strcmp(u.type.c_str(),"void") == 0)
			fprintf(fp,"%s \n{\n\t\n}\n",u.dec.c_str());
		else
			fprintf(fp,"%s \n{\n\t%s tmp;\n\t\n\treturn tmp;\n}\n",u.dec.c_str(),u.type.c_str());
	}
}

void cobble() {
	
	fp = fopen(f.c_str(),"w");
	fclose(fp);
	
	fp = fopen(f.c_str(),"ab+");
	for(int h = 0; h < (int)libs.size(); h++)
		wh(libs[h]);
	fprintf(fp,"\n");
	for(int t = 0; t < (int)fncs.size(); t++) {
		wf(fncs[t]);
		fprintf(fp,"\n");
	}
	fclose(fp);
}
