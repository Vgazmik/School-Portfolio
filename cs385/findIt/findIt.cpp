#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#define MAX_ARGS 5
using namespace std;

enum opt{SM = 0,AT = 1,LG = 2,ST = 3,TD = 4,SP = 5};
bool directives[6] = {false,false,false,false,false,false};

void caseDetect(char *test, int *a) {
	
	if (strcmp(test,"-summary") == 0 || strcmp(test, "-sum") == 0) 
		*a = SM;
	else if (strcmp(test,"-access") == 0 || strcmp(test, "-acc") == 0) 
		*a = AT;
	else if (strcmp(test,"-largest") == 0 || strcmp(test, "-lg") == 0)
		*a = LG;
	else if (strcmp(test,"-smallest") == 0 || strcmp(test, "-sm") == 0)
		*a = ST;
	else if (strcmp(test,"-treedir") == 0 || strcmp(test, "-td") == 0)
		*a = TD;
	else if (strcmp(test,"-sparse") == 0 || strcmp(test, "-sp") == 0)
		*a = SP;
	else {
		cout << "Usage: ./findIt [option1] [option2] "
		     << "[option3] [option4]\n"<<endl;
		cout << "[option*] options: "
		     <<"\n\t-access or -acc"
		     <<"\n\t-largest or -lg"
		     <<"\n\t-smallest or -sm"
		     <<"\n\t-treedir or -td"
		     <<"\n\t-sparse or -sp\n"
		     << endl;
		exit(1);
	}
}

void listAll(DIR *pd) {
	/*Directory related vars*/
	DIR *next = NULL;
	struct dirent *pent = NULL;
	struct stat s; //for file info
	int k = 0;
	int dirs = 0;
	char **rdir;
	
	if(pd == NULL) {
		cout << "Error: pdir\n" << endl;
		exit(1);
	}
	
	while(pent = readdir(pd)) {
		
		if(pent == NULL) {
			cout << "Error: pent\n" << endl;
			exit(1);
		}
		
		if(k == 0) {
			cout << "In "
			     << getcwd(NULL, 0)
			     << "\n-------------------------\n"
			     << endl;
		}
		
		cout << pent->d_name
		     << endl;
		lstat(pent->d_name, &s);
		if(S_ISDIR(s.st_mode)) {
			cout << "\tDirectory" << endl;
			
			if(strcmp(pent->d_name,".") != 0 
			&& strcmp(pent->d_name,"..") != 0) {
				dirs++;
			}
		}
		else
			cout << "\tFile" << endl;
		k++;
	}
	
	if(dirs > 0) {
		rdir = new char*[dirs];
	
		cout << "\n";
		k = 0;
	
		while(pent = readdir(pd)) {
		
			lstat(pent->d_name, &s);
		
			if(S_ISDIR(s.st_mode))
				if(strcmp(pent->d_name,".") != 0 
				&& strcmp(pent->d_name,"..") != 0) {
					rdir[k] == pent->d_name;
					k++;
				}
		}
	
		for(int i = 0; i < dirs; i++) {
			next = opendir(rdir[i]);
			listAll(next);
			closedir(next);
		}
	
		delete[] rdir;
	}
	
	closedir(pd);
}

int main(int argc, char** argv) {
	//Introduction
	cout<<"*******************************************"<<endl;
	cout<<"Project: Find My Sh*t!\nAuthor: Shane Lopez"<<endl;
	cout<<"*******************************************\n"<<endl;
	//Check validity of command before bothering with variables
	if(argc < 1 || argc > MAX_ARGS) {
		cout << "\nUsage: ./findIt [option1] [option2] "
		     << "[option3] [option4]\n"<<endl;
		exit(1);
	}
	//Define variables
	/*Option parsing vars*/
	int tick = 1;
	int opts[argc-1];
	int *pt;
	int tmp;

	/*Directory related vars*/
	DIR *pdir = NULL;
	struct dirent *pent = NULL;
	
	//Parse through options
	while (tick < argc) {
		//tmp = opts[tick-1];
		pt = &opts[tick-1];
		caseDetect(argv[tick], pt);
		tick++;
	}
	
	if(argc > 1) {
		cout<<"options: \n"<<endl;
		for(int i = 0; i < argc-1; i++) {
			/*cout << "opts[" 
			     << i 
			     << "]: " 
			     << opts[i] 
			     << "\n" 
			     << endl;*/
			directives[(opts[i])] = true;
		}
		
		if(directives[0])
			cout << "-sum\n" << endl;
		if(directives[1])
			cout << "-acc\n" << endl;	
		if(directives[2])
			cout << "-lg\n" << endl;
		if(directives[3])
			cout << "-sm\n" << endl;
		if(directives[4])
			cout << "-td\n" << endl;
		if(directives[5])
			cout << "-sp\n" << endl;
	}
	///////////////////////////////DO WORK//////////////////////////////
	/*pdir = opendir(".");//current dir
	int k = 0;
	if(pdir == NULL) {
		cout << "Error: pdir\n" << endl;
		exit(1);
	}
	
	while(pent = readdir(pdir)) {
		
		if(pent == NULL) {
			cout << "Error: pent\n" << endl;
			exit(1);
		}
		
		if(k == 0) {
			cout << "In "
			     << getcwd(NULL, 0)
			     << "\n-------------------------\n"
			     << endl;
		}
		
		cout << pent->d_name
		     //<< ", "
		     //<< pent->d_type
		     << endl;
		k++;
	}
	
	cout << "\n";
	closedir(pdir);*/
	listAll(opendir("."));
	exit(0);
}
