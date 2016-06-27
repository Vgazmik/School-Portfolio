#include "ctl.h"

int main(int argc, char** argv)
{
	int  read = 0;
	fn   cf;
	char line[80];
	
	pid_t a = fork();
	
	if(a < 0) 
	{
		printf("Error: fork()\n");
		return 1;
	}
	else if (a == 0) {
		
		printf("\n<CONFIG>\n");
		
		FILE *config = fopen(cname.c_str(),"r");
	
		while( fgets(line,80,config) )
		{
			switch(read) 
			{
				case 0:  break;
				case 1:  hname = std::string(trim(line));
					 read = 0;
				 	 break;
				case 2:  fname = std::string(trim(line));
					 read = 0;
					 break;
				default: break;
		
			}
		
			if( strstr(line,"LIB_FILE") != NULL ) {
				read = 1;
				continue;
			}
			else if( strstr(line,"FN_FILE") != NULL ) {
				read = 2;
				continue;
			}
		}
	
		fclose(config);
		
		printf("\n<Detected Files>\n\tLIB_FILE: %s\n\t FN_FILE: %s\n",hname.c_str(),fname.c_str());
	
		read = 0;	
		FILE *hdr = fopen(hname.c_str(),"r");
		
		while( fgets(line,80,hdr) )	
		{
			lib cl;
			if(strstr(line, "STD") != NULL)
			{
				read = 1;
				continue;
			}
			else if(strstr(line, "CST") != NULL) 
			{
				read = 2;
				continue;
			}
		
			if(strcmp(trim(line),"none") != 0)
			{
				cl.name = std::string(trim(line));
			
				if(read == 1) 
				{
					cl.cst = false;
				}
				else if(read == 2) 
				{
					cl.cst = true;
				}
		
				libs.push_back(cl);
			}
		}		
	
		fclose(hdr);
		
		printf("<Detected Libs>\n");
		for(int i = 0; i < libs.size(); i++)
		{
			printf("\t%s\n",libs[i].name.c_str());
		}
	
		read = 0;
		FILE *fnc = fopen(fname.c_str(),"r");
		
		while( fgets(line,80,fnc) )	
		{
			fn cf;
		
			if(strstr(trim(line),"FNCS") != NULL) 
			{
				read = 1;
				continue;
			}
		
			if(read == 1)
			{
				cf.dec = std::string(trim(line));
				cf.len = strlen(cf.dec.c_str());
				fncs.push_back(cf);
				continue;
			}
		}	
			
		fclose(fnc);
		
		printf("<Detected Function Declarations>\n");
		for(int j = 0; j < fncs.size(); j++) 
		{
			printf("\t%s\n",fncs[j].dec.c_str());
		}
		/*************************<\CONFIG>************************/
		//printf("#Libs: %d\n#Fncs: %d\n",(int)libs.size(),(int)fncs.size());
		printf("\n</CONFIG>\n\n");
		
		printf("Typing...\n");
		cobble();
	}
	else 
	{
		waitpid(a,NULL,0);
		
		pid_t b = fork();
		
		if(b < 0)
		{
			printf("Error: fork()\n");
			return 1;
		}
		else if(b == 0) { 
			printf("Compiling...\n");
			compile();
		}
		else {
			waitpid(b,NULL,0);
			printf("Executing...\n");
			run();
		}
	}
	
	return 0;
}
