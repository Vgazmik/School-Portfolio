#include "ctl.h"

int main(int argc, char** argv)
{
	if(argc < 2) {
		printf("Please specify the skel file as follows (omit <,>): \n\t$ skel <skel_file>");
		return 1;

	}
	
	skelf = argv[1];

	printf("\nPreparing to parse file \"%s\"\n",skelf.c_str());
	
	int  read = 0;
	//fn   cf;
	char line[80];
	
	pid_t a = fork();
	
	if(a < 0) 
	{
		printf("Error: fork()\n");
		return 1;
	}
	else if (a == 0) 
	{
		read = 0;	
		FILE *hdr = fopen(skelf.c_str(),"r");
		
		while( fgets(line,80,hdr) )	
		{
			lib cl;
			if(strstr(line, "#:") != NULL)
			{
				cl.cst = false;
				
				char *p;
	
				p = strtok(line,"#:");
				
				cl.name = std::string(trim(p));
				
				libs.push_back(cl);
			}
			else if(strstr(line, "!:") != NULL) 
			{
				cl.cst = true;
				
				char *p;
	
				p = strtok(line,"!:");
				
				cl.name = std::string(trim(p));
				
				libs.push_back(cl);
			}
		}
		
		fclose(hdr); // libs parsed
		
		printf("\n<Detected Libs>\n");
		for(int i = 0; i < libs.size(); i++)
		{
			printf("\t%s\n",libs[i].name.c_str());
		}
		
		read = 0;
		FILE *fnc = fopen(skelf.c_str(),"r");
		
		m_spec = false;
		//fn cf;
		
		bool halt = false;

		while( fgets(line,80,fnc) )	
		{
			fn cf;
			
			if(strstr(line,"):")) 
			{
				if(strstr(line,"):;"))
					halt = true;
				
				// process
					
				char *p;
				char *s;
					
				p = strtok(trim(line),"@");
				cf.dec = std::string(trim(p));
				//printf("line minus at:\n\n\t%s\n",p);
				//
				//p = full declaration minus '@', for future reference
				
				p = strtok(p, "(");
				s = trim(p);
				p = strtok(NULL, "(");
				p = strtok(p, ")");
				
				s = strtok(s,",");
				
				cf.name = trim(s);
				
				s = strtok(NULL,",");
				
				cf.type = trim(s);
				
				arg ad;
				ad.type = cf.type;
				ad.name = "whatever";
				aproc(ad);
				
				cf.type = ad.type;
				
				//printf("\nfn name: %s\nfn type: %s\n",cf.name.c_str(),cf.type.c_str());
				
				//printf("\nfn args: \n\t%s\n",p);
				//
				//p == fn arguments separated by commas
				
				p = trim(p);
				p = strtok(p,",");
				
				while(p != NULL) {
					char *t;
					arg a;
					//printf("\nPassed Argument: %s\n",p);
					t = p;
					p = strtok(NULL,",");
					t = strtok(t," ");
					a.type = trim(t);
					t = strtok(NULL," ");
					a.name = trim(t);
					
					//process argument
					
					aproc(a);
					
					//printf("Parsed Arg: %s %s\n",a.type.c_str(),a.name.c_str());
					
					cf.args.push_back(a);
				}
				if(halt) {
						cf.dec = cf.type;
						cf.dec += " ";
						cf.dec += cf.name;
						cf.dec += "(";
						for(int y = 0; y < (int)cf.args.size(); y++)
						{	
							//printf("%s arg %i: %s %s\n",cf.name.c_str(),y,cf.args[y].type.c_str(),cf.args[y].name.c_str());
							cf.dec += cf.args[y].type;
							cf.dec += " ";
							cf.dec += cf.args[y].name;
							
							if(y != (int)cf.args.size() - 1)
								cf.dec += ", ";
						}
						cf.dec += ")";
						fncs.push_back(cf);
						halt = false; 
						continue;
				}
				else {
					while(fgets(line,80,fnc)) 
					{
						if(strstr(line,"@!") != NULL) //end function declaration
						{
							cf.dec = cf.type;
							cf.dec += " ";
							cf.dec += cf.name;
							cf.dec += "(";
							for(int y = 0; y < (int)cf.args.size(); y++)
							{	
								//printf("%s arg %i: %s %s\n",cf.name.c_str(),y,cf.args[y].type.c_str(),cf.args[y].name.c_str());
								cf.dec += cf.args[y].type;
								cf.dec += " ";
								cf.dec += cf.args[y].name;
							
								if(y != (int)cf.args.size() - 1)
									cf.dec += ", ";
							}
							cf.dec += ")";
						
							/*if(strcmp(cf.name.c_str(),"main") == 0) 
							{	
								M = cf;
								m_spec = true;
							}		
							else
							{*/
								fncs.push_back(cf);
							//}
							break;
						}
						else 
						{
							//process in-function ops
							/*if(strstr(line,"p:"))
							{
								op r;
								r.type = PRT;
								char *prt;
								prt = strtok(line,"p:");
								prt = strtok(prt,".");
								prt = strtok(trim(prt)," ");
							
								while(prt != NULL)
								{
									char *c;
									c = prt;
									prt = strtok(NULL," ");
									if(strstr(prt,"'") != NULL) 
									{
										char *w;
										w = strtok(w,"'");
										for(int n = 0; n < (int)cf.args.size(); n++)
										{
											if(strcmp(cf.args[n].name.c_str(),w) == 0) {
												r.strvars.push_back(cf.args[n]);
												r.str += " ";
												r.str += cf.args[n].type;
												break;
											}		
										}
									
									}
									else
									{
										r.str += " ";
										r.str += c;
									}
								}
							
								printf("Added print operation with string %s to function %s\n",r.str.c_str(),cf.name.c_str());
							
								cf.ops.push_back(r);
							}*/
						}
					}
				}
			}	
		}
		
		fclose(fnc);
		printf("\n<Detected Function Declarations>\n");
		for(int j = 0; j < fncs.size(); j++) 
		{
			printf("\t%s\n",fncs[j].dec.c_str());
		}
		/*************************<\CONFIG>************************/
		printf("\n#Libs: %d\n#Fncs: %d\n\n",(int)libs.size(),(int)fncs.size());
		
		printf("Typing...\n");
		cobble();
	} // End Scan
	else 
	{
		waitpid(a,NULL,0);
		
		pid_t b = fork();
		
		if(b < 0)
		{
			printf("Error: fork()\n");
			return 1;
		}
		else if(b == 0) 
		{ 
			printf("Compiling...\n");
			compile();
		}
		else 
		{
			waitpid(b,NULL,0);
			printf("Executing...\n");
			run();
		}
	}
	
	return 0;
}
