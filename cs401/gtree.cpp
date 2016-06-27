#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <vector>

using namespace std;

struct node;

struct goby {
	node *rt; //the root the two connect to (gate)
	int ridx; //index of rightmost member of a pairing
	goby(node *r, int i) : rt(r), ridx(i) {}
};

struct node {
	int cost;
	node *left;
	node *right;
	int con; // 0 for singular, 1 for connected
	goby *g; //eventually, every node gets a go-by, goby for short
	node(int c) : cost(c) {}
	node(int c, node *l, node *r) : cost(c), left(l), right(r) {}
};

int main (int argc, char *argv[]) {
	
	string line;
	int *delays;
	int first = 0, count = 0;
	node **gates;
	int gd = atoi(argv[2]);
	vector<goby> verts;
	vector<goby> verts2;
	
	if (argc < 2 || argc > 3) { //no arguments / too many arguments
		cout << "\nUsage (<'s, >'s to be omitted): "
		     << "\n\t$ gtree <gate library> <gate delay> \n"
		     << endl;
		return -1;		
	}
	
	cout << "scanning lib file...\n" << endl;
	
	ifstream fs (argv[1]);
	
	if (fs.is_open()) {
		while (getline (fs, line)) {
			if(first == 0) {
				delays = new int[atoi(line.c_str())];
				first++;
			}
			else {
				delays[count] = atoi(line.c_str());
				//cout << "delays[" << count << "] = " 
				//     << delays[count] << endl;
				count++;
			}
			cout << line << '\n';
		
		}
		cout << endl;
		fs.close();

	}
	
	else { 
		cout << "Error: Unable to open file \"" 
		     << argv[1] 
		     << "\"\n" 
		     << endl;
		return -1;
	}
	
	gates = new node*[count];
	int j;

	for (first = 0; first < count; first++) {
		j = delays[first];
		//cout << j << '\n';
		node *ng = new node(j,NULL,NULL);
		gates[first] = ng;
	}

	//cout << endl;
	
	node *root = new node(0,NULL,NULL);
	
	//All the tools for node making are set, all data is stored, now solve
	
	int higher = 0;
	
	for (j = 0; j < count; j++) {
		if(j-1 >= 0) {
			if( delays[j-1] == delays[j]-1 || 
		    	    delays[j-1] == delays[j]+1 || 
		    	    delays[j-1] == delays[j]) {
		
				if(gates[j-1]->con == 0 && gates[j]->con == 0) {
					//connect
					first = (delays[j-1] > delays[j]) ? delays[j-1]+gd:delays[j]+gd;
					node *meld = new node(first,gates[j-1],gates[j]);
					goby *ng = new goby(meld, j);
					
					higher++;
					gates[j-1]->con++;
					gates[j]->con++;
					gates[j-1]->g = ng;
					gates[j]->g = ng;
				}
		
			}
		
		}
		else if(j+1 < count) {
			if( delays[j+1] == delays[j]+1 || 
			    delays[j+1] == delays[j]-1 || 
			    delays[j+1] == delays[j]) {
			
				if(gates[j+1]->con == 0 && gates[j]->con == 0) {
					//connect
					first = (delays[j+1] > delays[j]) ? delays[j+1]+gd:delays[j]+gd;
					node *meld = new node(first,gates[j],gates[j+1]);
					goby *ng = new goby(meld,j+1);
					
					higher++;
					gates[j+1]->con++;
					gates[j]->con++;
					gates[j]->g = ng;
					gates[j+1]->g = ng;
				}
			
			}
	
		}
	
	}//end for
	//now re-evaluate
	int rid = 0;
	for(first = 0; first < count; first++) {
		if(first == rid) // index accounted for by a goby
			continue;
		if(gates[first]->g != NULL) {
			rid = gates[first]->g->ridx;
			verts.push_back(*(gates[first]->g));
		}
		else {
			goby *gbt = new goby(gates[first],first);
			verts.push_back(*gbt);
		}
	}
	//now, the vector verts has all inputs and outputs after the first iteration, valuable info
	
	int tmp = 0;
	int tmp2 = 0;
	int w = 1;
	higher = 0;
	int cst = 0;
	
	while (root->left == NULL || root->right == NULL) {
	//The idea is this:
	//
	//start off with a list of go-by's (stand ins for connections or yet to be connected leaves)
	//and an offset, w here, starts at 1 but rises to accomodate larger differences between adjacent 
	//goby roots should none be within the +/- w range of current root's accumulated delay time.
	//In essence, a greedy algorithm searching for minimum delay neighbors.
		for (vector<goby>::size_type it = 0; it != verts.size(); it++) {
		//Within the for, the counter 'higher' will be incremented for each new connection made
		//until it attains the value of n-2 (hence the while and increasing w), the nuber of inputs - 2. 
		//Unconnected go-by's get stuck onto verts 2, along with newly connected go-by's, clustering them 
		//in order of connection; with a rising w on the condition of no connections made in an iteration, 
		//all the go-by's will eventually be connected when there are n-2 such go-by's, as at that point 
		//the root will be given the left and rightmost goby roots as its left and right members, and 
		//the root will be returned. After a for iteration, when verts2 has been populated, verts is 
		//cleared, verts2 is copied to verts, and verts2 is cleared in preparation for the next while 
		//loop iteration.
			
			if(it != 0) {
				tmp = verts[it-1].rt->cost;
				tmp2 = verts[it].rt->cost;
				
				if (tmp == tmp2 || tmp == tmp2-w || tmp == tmp2+w) {
					if(verts[it-1].rt->con == 0 && verts[it].rt->con == 0) {
						//connect
						cst = (tmp2 > tmp) ? tmp2+gd : tmp+gd;
						node *u;
						if (verts[it].ridx > verts[it-1].ridx) { // should always be true
							u = new node(cst,verts[it-1].rt,verts[it].rt);
							rid = it;
						}
						else {
							u = new node(cst,verts[it].rt,verts[it-1].rt);
							rid = it;
						}

						goby *f = new goby(u,rid);
						higher++;
						verts2.push_back(*f);
					
					}
				}
		
			}
			if(it != verts.size()-1) {
			//Symmetry here with regards to the right neighbor
			
			}	
			
		}//end for
		//evaluate highest, change w as needed;
		//if highest == n-2 (meaning n-2 pairings), iterate through latest go-by's and distinguish the left and rightmost to set
		//root members, set root, break
		
	}
	
	//report tree
	
	return 0;
	
}
