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

enum OPS{PRT = 0, ADD = 1, SUB = 2, MUL = 3, EQ = 4, LP = 5};

std::string f = "outline.c";
std::string o = "proto";
std::string x = "./proto";

struct lib{
	std::string name;
	bool        cst;
};

struct arg{
	std::string type;
	std::string name;
	std::string value; //set for variables, left alone for fn args
	bool isptr;
	bool isref;
};

struct op{
	OPS type; // OP type
	std::string left;  // LHS
	std::string right; // RHS
	std::string str;   // if type is PRT, this holds the string to be printed
	std::vector<arg> strvars; // if type is PRT, this holds the order of printed vars
};

struct fn{
	std::string dec;
	std::string name;
	int         len;
	std::string type;
	std::vector<arg> args;
	std::vector<op> ops;
};

//std::string cname = "conf";    //config file name
std::string skelf;
std::vector<lib> libs;         //headers to include
std::vector<fn> fncs;          //function declarations
fn M;
bool m_spec;

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

void aproc(arg &raw) {
	
	char *t;
	char *a;
	char *b;

	//printf("%s %s\n",raw.type.c_str(),raw.name.c_str());
	
	a = (char*)raw.type.c_str();
	b = (char*)raw.name.c_str();
	
	if(strstr(a,"*") != NULL)
	{
		raw.isptr = true;
		t = strtok(a,"*");

	}
	else if(strstr(a,"&") != NULL)
	{
		raw.isref = true;
		t = strtok(a,"&");
	}
	else
		t = strtok(a,"%");
	
	if(strcmp(t,"i") == 0)
		raw.type = "int";
	else if(strcmp(t,"f") == 0)
		raw.type = "float";
	else if(strcmp(t,"d") == 0)
		raw.type = "double";
	else if(strcmp(t,"c") == 0)
		raw.type = "char";
	else if(strcmp(t,"t") == 0 || strcmp(t,"nil") == 0)
		raw.type = "void";
	else
		raw.type = "int";
	
}

void type() { // standard, friendly main
	
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
		//ftype(&u);
		//printf("Header: %s\nType: %s\n",u.dec.c_str(),u.type.c_str());
		if(strcmp(u.type.c_str(),"void") == 0)
		{
			//fprintf(fp,"%s \n{\n\t\n}\n",u.dec.c_str());
			fprintf(fp,"%s \n{\n",u.dec.c_str());
			
			if((int)u.ops.size() > 0)
			{
				for(int g = 0; g < (int)u.ops.size(); g++)
				{
					if(u.ops[g].type == PRT)
					{
						fprintf(fp,"\tprintf(\"%s\");\n",u.ops[g].str.c_str());
					}
				}
			}

			fprintf(fp,"\n}\n");
			
		}
		else
		{	
			fprintf(fp,"%s \n{\n\t%s tmp;\n\t\n\treturn tmp;\n}\n",u.dec.c_str(),u.type.c_str());
		}	
	}
}

void cobble() {
	
	fp = fopen(f.c_str(),"w");
	fclose(fp);
	
	fp = fopen(f.c_str(),"ab+");
	if((int)libs.size() > 0) {
		for(int h = 0; h < (int)libs.size(); h++)
			wh(libs[h]);
		fprintf(fp,"\n");
	}
	if((int)fncs.size() > 0) {
		for(int t = 0; t < (int)fncs.size(); t++) {
			wf(fncs[t]);
			fprintf(fp,"\n");
		}
	}
	if(!m_spec || (int)fncs.size() == 0)
		type();
	else if(m_spec) {
		wf(M);
		fprintf(fp,"\n");
	}
	fclose(fp);
}
