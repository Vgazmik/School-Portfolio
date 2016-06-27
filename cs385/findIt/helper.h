#include <errno.h>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#define MAX_ARGS 7

void caseDetect(char *test, int *a);

struct tab { //keep 'tabs' on files
	std::string fname;  // file name
	int         fsz;    // file size
	time_t      at;     // last access time
	uid_t       uid;    // file user ID
	gid_t       gid;    // file group ID
	bool        dir;    // ?directory?
	std::string loc;    // path to file
};

struct dir { //remember dirs
	std::string      dname;   // directory name
	int              nf;      // #files in directory
	int              nd;      // #dirs in directory
	std::vector<dir> kids;    // subdirs
};

enum opt{SUM = 0,ACC = 1,LG = 2,SM = 3,TD = 4,SZ = 5}; //option codes
bool directives[6] = {false,false,false,false,false,false}; //bool tracker

static bool options;           // ?options invoked?
static std::string target;     // global target name
static std::vector<tab> found; // global tab vector
static tab XL;                 // largest file
static tab XS;                 // smallest file
static dir root;               // starting directory
//static dir rt;

void populate(int argc, char** argv) {
	/*Option parsing vars*/
	int tick = 1;
	int opts[argc-1];
	int *pt;
	int tmp;
	
	//Parse through options
	while (tick < argc) {
		pt = &opts[tick-1];
		caseDetect(argv[tick], pt);
		tick++;
	}
	
	if(argc > 1) {
		std::cout << "options specified: \n" 
			  << std::endl;
		for(int i = 0; i < argc-1; i++) {
			directives[(opts[i])] = true;
		}
		
		if(directives[0]) {
			std::cout << "-sum\n" << std::endl;
			options = true;
		}
		if(directives[1]) {
			std::cout << "-acc\n" << std::endl;
			options = true;
		}
		if(directives[2]) {
			std::cout << "-lg\n" << std::endl;
		}
		if(directives[3]) {
			std::cout << "-sm\n" << std::endl;
		}
		if(directives[4]) {
			std::cout << "-td\n" << std::endl;
		}
		if(directives[5]) {
			std::cout << "-sz\n" << std::endl;
			options = true;
		}
	}
	
	return;
}

void caseDetect(char *test, int *a) {
	
	if (strcmp(test,"-summary") == 0 || strcmp(test, "-sum") == 0) 
		*a = SUM;
	else if (strcmp(test,"-access") == 0 || strcmp(test, "-acc") == 0) 
		*a = ACC;
	else if (strcmp(test,"-largest") == 0 || strcmp(test, "-lg") == 0)
		*a = LG;
	else if (strcmp(test,"-smallest") == 0 || strcmp(test, "-sm") == 0)
		*a = SM;
	else if (strcmp(test,"-treedir") == 0 || strcmp(test, "-td") == 0)
		*a = TD;
	else if (strcmp(test,"-size") == 0 || strcmp(test, "-sz") == 0)
		*a = SZ;
	else {
		std::cout << "Usage: ./findIt [option1] [option2] "
		     	  << "[option3] [option4]\n"<<std::endl;
		std::cout << "[option*] options: "
		     	  <<"\n\t-summary or -sum"
		     	  <<"\n\t-access or -acc"
		     	  <<"\n\t-largest or -lg"
		     	  <<"\n\t-smallest or -sm"
		     	  <<"\n\t-treedir or -td"
		     	  <<"\n\t-size or -sz\n"
		     	  << std::endl;
		exit(1);
	}
}

void query() {
	
	std::cout << "What are you looking for? ";
	getline(std::cin,target);
	std::cout << std::endl 
		  << "You entered: " 
		  << target 
		  << std::endl;
	if(strcmp(target.c_str(),"") == 0 || target.length() < 2) {
		std::cout<< "\tSorry, too general of a target. Try again (min 2 characters)\n"
			 << std::endl;
		query();
		return;
	}
}

void listAll(std::string go) {
	/*Directory related vars*/
	static int top = 0;	         // for recursion
	static int top2 = 0;	         // for recursion
	static int top3 = 0;	         // for recursion
	static int top4 = 0;	         // for recursion
	DIR *pd;                         // name of dir to open
	struct dirent *pent = NULL; 	 // for dir info
	struct stat s;              	 // for file info
	int matches = 0;                 // # target matches
	int k = 0;                  	 // counter
	int dirs = 0;               	 // # of subdirs
	char path[128];             	 // for book keeping
	tab curT;                   	 // current tab
	tab lg,sm;                       // tabs of largest and smallest
	std::vector<tab> tabs;           // collection of tabs
	std::vector<std::string> rdir;   // for recursion
	std::string p;                   // for storing top level path
	std::string c;		         // for storing full path to file
	static dir tmp;                  // for storing directory structure
	static dir cur;
	
	if((pd = opendir(go.c_str())) == NULL) { // try to open dir
		std::cout << "Error: pdir\n" << std::endl;
		return;
	}
	
	while(pent = readdir(pd)) { // loop through items in dir
		if(pent == NULL) {
			std::cout << "Error: pent\n" << std::endl;
			exit(1);
		}

		if(k == 0) {
			if(top == 0) {
				getcwd(path, 128);
				std::cout << "\nIn "
			     	          << path
			    		  << "\n-----------------------------------"
			     		  << std::endl;
				p = std::string(path);
				cur.dname = p; //TD
				top = 1;
			}
			else {
				std::cout << "\nIn "
			     	 	  << go
			     		  << "\n-----------------------------------"
			     		  << std::endl;
				p = go;
				cur.dname = p; //TD
			}
		}
		
		if(top2 == 0) {
			if(lstat(pent->d_name, &s) != 0) {
				std::cout << "Error: lstat - " 
				<< errno 
				<< std::endl;
				return;
			}
			c = p;
			c = c.append("/");
			curT.loc = c;
			top2 = 1;
		}
		else {
			std::string h = std::string(pent->d_name);
			c = p;
			c = c.append("/");
			curT.loc = c;
			c = c.append(h);
			if(lstat(c.c_str(), &s) != 0) {
				std::cout << "Error: lstat - " 
					  << errno 
					  << std::endl;
				return;
			}
		}
		
		if(S_ISDIR(s.st_mode)) {
			if(strcmp(pent->d_name,".") != 0 
			&& strcmp(pent->d_name,"..") != 0) {
				rdir.push_back(pent->d_name);
				dirs++;
				curT.dir = true;
			}
		}
		else 
			curT.dir = false;
		
		
		if(strcmp(pent->d_name,".") != 0 
		&& strcmp(pent->d_name,"..") != 0) {

			curT.fname = std::string(pent->d_name);
			curT.fsz = s.st_size;
			curT.at = s.st_atime;
			curT.uid = s.st_uid;
			curT.gid = s.st_gid;
			tabs.push_back(curT);
			
			if(strstr(curT.fname.c_str(),target.c_str()) != NULL) {
				found.push_back(curT);
				matches++;
			}
		}

		k++;
	}

	tmp.nd = dirs;                 // TD
	if((k-2) > 0)                  // TD
		tmp.nf = k - 2 - dirs; // TD
	else                           // TD
		tmp.nf = 0;            // TD
	
	if(top4 == 0) {                // TD
		cur.nd = tmp.nd;       // TD
		cur.nf = tmp.nf;       // TD
		root = cur;            // TD
	}
	else 
	        cur = tmp;
	
	if(k > 2) {// > 0 files -> Determine largest, smallest
		
		int l,s;
		for(int g = 0; g < tabs.size(); g++) {
			if(g  == 0) {
				if(top3 == 0) {
					XL = XS = tabs[g];
					top3++;
				}
				lg = sm = tabs[g];
			}
			else
				if(tabs[g].fsz > lg.fsz)
					lg = tabs[g];
				else if(tabs[g].fsz < sm.fsz)
					sm = tabs[g];
				
				if(tabs[g].fsz > XL.fsz)
					XL = tabs[g];
				else if(tabs[g].fsz < XS.fsz)
					XS = tabs[g];
		}
		
		// Deal With Specified Options
		
		if(options) {
			std::cout << "\n\t=====DATA=====" 
				  << std::endl;
			if(directives[SUM])
					std::cout << "\t~~" << matches  << " Targets Found~~" 
						  << std::endl;
			if(directives[ACC]) {
		
				for(int b = 0; b < tabs.size(); b++) {
					
					std::cout << "\n\t" 
						  << tabs[b].fname;
					if(tabs[b].dir)
						std::cout << "/" 
							  << std::endl;
					else
						std::cout<<std::endl;
					if(directives[SUM])
						std::cout << "\t|\n\t*--Path to file: " 
							  << tabs[b].loc 
							  << std::endl;
					std::cout << "\t|\n\t*--Last accessed: " 
						  << ctime(&tabs[b].at);
						  //<< std::endl;
					if(directives[SZ])
						std::cout << "\t|\n\t*--Size: "
							  << tabs[b].fsz
							  << std::endl;
					else
						std::cout << std::endl;
				}
			}
			else if(directives[SUM]) {
			
				for(int d = 0; d < tabs.size(); d++) {
					std::cout << "\n\t" 
						  << tabs[d].fname;
					if(tabs[d].dir)
						std::cout << "/" 
							  << std::endl;
					else
						std::cout << std::endl;

					std::cout << "\t|\n\t*--Path to file: " 
						  << tabs[d].loc;
						  //<< std::endl;
					if(directives[SZ])
						std::cout << "\t|\n\t*--Size: "
							  << tabs[d].fsz
							  << std::endl;
					else
						std::cout << std::endl;
				}
			}
			else if(directives[SZ]) {
				for(int v = 0; v < tabs.size(); v++) {
					std::cout << "\n\t" 
						  << tabs[v].fname;
					if(tabs[v].dir)
						std::cout << "/" 
							  << std::endl;
					else
						std::cout << std::endl;

					std::cout << "\t|\n\t*--Size: "
						  << tabs[v].fsz
						  << std::endl;
				}
			}

			if(directives[LG] && directives[SUM]) {
				std::cout << "\n    * Largest File: " 
					  << lg.fname 
			  		  << "\n      Size: " 
					  << lg.fsz
					  << std::endl;
			}

			if(directives[SM] && directives[SUM]) {
				std::cout << "\n    * Smallest File: " 
					  << sm.fname 
					  << "\n      Size: " 
					  << sm.fsz
					  << std::endl;
			}
		}
		else {
			std::cout << "\t~~" 
				  << matches  
				  << " Targets Found~~" 
				  << std::endl;
		}
		
	}
	if(dirs > 0) {
		std::string cat = p;
		for(int m = 0; m < rdir.size(); m++) {
			cat = cat.append("/");
			cat = cat.append(rdir[m]);
			if(directives[SUM])
				std::cout << "\n\n*~ Recursively Listing Contents of " 
			   	          << rdir[m]  
					  << "/\n" 
					  << std::endl;
			//cout << "\n\t\t" << cat  << endl;
			tmp.dname = rdir[m];
			top4++;
			listAll(cat);
			top4--;
			cur.kids.push_back(tmp);
			cat = p;
		}
		if(top4 == 0) {
			root = cur;
			cur = root;
		}
	}
	else {
		if(k == 2)
			std::cout << "\n\t\t~No Files~"
				  << std::endl;
			std::cout << "\n\t\t~No Subdirectories~" 
				  << std::endl;
		cur.kids.push_back(tmp);
	}
	closedir(pd);
	return;
}

void treedir(dir start) {
	dir now;
	static int depth = 0;
	std::cout << "+ "
		  << start.dname 
		  << " ( " 
		  << start.nf 
		  << " files, " 
		  << start.nd 
		  << " dirs )" 
		  << std::endl;
	depth++;
	for(int i = 0; i < start.kids.size(); i++) {
		for(int j = 0; j < depth; j++) {
			std::cout << "\t";
		}
		now = start.kids[i];
		/*std::cout << "\n\t" 
			  << now.dname 
			  << std::endl;*/
		treedir(now);
		depth--;
	}
	if(depth == 0)
		std::cout << std::endl;
}

void listMatch() {
	std::cout<< "\n\n[Results]"
		 << std::endl;
	if(directives[TD]) {
		treedir(root);
	}
	if(directives[LG]) {
		std::cout << "\nLargest File Found: "
			  << XL.fname
			  << "\nSize: "
			  << XL.fsz
			  << "\nPath: "
			  << XL.loc
			  << std::endl;
		if(directives[ACC])
			std::cout << "Last Access: "
				  << ctime(&XL.at)
				  << std::endl;
	}
	if(directives[SM]) {
		std::cout << "\nSmallest File Found: "
			  << XS.fname
			  << "\nSize: "
			  << XS.fsz
			  << "\nPath: "
			  << XS.loc
			  << std::endl;
		if(directives[ACC])
			std::cout << "Last Access: "
				  << ctime(&XS.at)
				  << std::endl;
	}
	if(found.size() > 0) {
		double per;
		std::cout << "\n~# Listing Matches for target \"" 
			  << target 
			  <<"\" #~" 
			  << std::endl;
		for(int y = 0; y < found.size(); y++) {
			per = ((double)target.length() 
			    /  (double)found[y].fname.length()) 
			    * 100.0;
			std::cout << "\n<Match " << y+1 << "> " << std::endl;
			std::cout << "\tName          \"" 
				  << found[y].fname 
				  << "\"\n\tPath          "
				  << found[y].loc
				  << "\n\t%Match        "
				  << (int)per
				  << std::endl;
			if(directives[ACC])
				std::cout << "\tLast Access   "
				 	  << ctime(&found[y].at);
				 	  //<< std::endl;
			if(directives[SZ])
				std::cout << "\tSize          "
					  << found[y].fsz
					  << std::endl;
			else
				std::cout << std::endl;
		}
	}
	else {
		std::cout << "\n~# No Matches Found #~" 
			  << std::endl;
	}

	return;
}
