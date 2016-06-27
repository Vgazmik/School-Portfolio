#include "helpers.h"

orderList bills[10];

int main(int argc, char **argv) {
	
	for(int i = 0; i < 10; i++) {
		bills[i].seatNumber = i+1;
	}
	
	
	while(true) {
		std::string res = "";
		int seat;
		//orderList current;

		std::cout<<"\nPlease Enter a Command: ";
	
		std::getline(std::cin,res);
		std::cout<<"\nExcellent!"<<std::endl;

		if(strcmp(res.c_str(),"a") == 0) {
			//add to list procedure
			//foodItem *fi;
			std::string a = "";
			int choice;
			int c;

			std::cout<<"\nChoose a Seat: ";
			std::getline(std::cin,a);
			seat = atoi(a.c_str());
			//current = bills[seat-1];
			std::cout<<"\n\tMENU\n\n1) Salmon Fillet\t2) Turkey Sandwich\t3) Eggplant Casserole\n" 
				 << std::endl;
			std::cout<<"What would you like? ";
			std::getline(std::cin,a);
			std::cout<<"\n";
			choice = atoi(a.c_str());
		
			switch(choice) {
				case 1:		std::cout<<"\nHow would you like that cooked? 1) Rare  2) Medium  3) Well\n";
						std::getline(std::cin,a);
						std::cout<<"\n";
						c = atoi(a.c_str()) - 1;
						std::cout<<"\n\tAdding a Salmon Fillet!\n";
						bills[seat-1].tab.push_back(new salmon("Delicious Salmon",
											7.25,
											200,
											c,
											"Succulent Dreameater shrimp marinated in our special blend of basil and sadness."));
						if(bills[seat-1].inUse == false)
							bills[seat-1].inUse = true;
						break;
				case 2:		std::cout<<"What kind of bread would you like? 1) Whole  2) Wheat  3) Rye\n";
						std::getline(std::cin,a);
						std::cout<<"\n";
						c = atoi(a.c_str()) - 1;
						std::cout<<"\n\tAdding a Turkey Sandwich!\n";
						bills[seat-1].tab.push_back(new twich("Alluring Turkey Sandwich",
										       5.67,
										       350,
										       c));
						if(bills[seat-1].inUse == false)
							bills[seat-1].inUse = true;
						break;
				case 3:		std::cout<<"\n\tAdding an Eggplant Casserole!\n";
						bills[seat-1].tab.push_back(new eggc("Tasty Eggplant Casserole",
										      6.75,
										      400,
										      7,
										      "A hearty helping of our original grease chips."));
						if(bills[seat-1].inUse == false)
							bills[seat-1].inUse = true;
						break;
				default:	std::cout<<"\n\tWoah, back up partner! Let's take it from the top...\n";
						continue;
			}
		
		}
		else if(strcmp(res.c_str(),"n") == 0) {
			//new list procedure
			std::string r = "";
			int t;
			std::cout << "\nWhich seat would you like to assign a new tab? ";
			std::getline(std::cin,r);
			std::cout<<"\n";
			t = atoi(r.c_str()) - 1; // true index
			
			if(bills[t].inUse == false) {
				bills[t].inUse = true;
			}
			else {
				std::cout << "\nError! That seat already has a bill. Please delete it first.\n";
				continue;
			}
			
		}
		else if(strcmp(res.c_str(),"c") == 0) {
			//copy list procedure
			std::string r = "";
			int s,t;
			std::cout << "\nCopying tab!\n\nEnter the seat number of the tab you wish to copy: ";
			std::getline(std::cin,r); // true index
			std::cout<<"\n";
			t = atoi(r.c_str()) - 1;
			std::cout<<"\nEnter the seat number you'd like to assign the copied tab: ";
			std::getline(std::cin,r);
			std::cout<<"\n";
			s = atoi(r.c_str()) - 1; // true index
			copy(bills,t,s);
		}
		else if(strcmp(res.c_str(),"d") == 0) {
			//delete list procedure
			std::string r = "";
			int t;
			std::cout << "\nWhich seat's tab  would you like to delete? ";
			std::getline(std::cin,r);
			std::cout<<"\n";
			t = atoi(r.c_str()) - 1; // true index

			bills[t].tab.clear();
			bills[t].inUse = false;
		}
		else if(strcmp(res.c_str(),"l") == 0) {
			//list-patron procedure
			std::string r = "";
			int t;
			std::cout << "\nWhich seat would you like to examine? ";
			std::getline(std::cin,r);
			std::cout<<"\n";
			t = atoi(r.c_str());
			examine(bills[t-1]);
		}
		else if(strcmp(res.c_str(),"s") == 0) {
			//swap lists procedure
			std::string r = "";
			int s,t;
			std::cout << "\nSwapping tabs!\n\nEnter the seat number of the first tab: ";
			std::getline(std::cin,r); // true index
			std::cout<<"\n";
			t = atoi(r.c_str()) - 1;
			std::cout<<"\nEnter the seat number of the second tab: ";
			std::getline(std::cin,r);
			std::cout<<"\n";
			s = atoi(r.c_str()) - 1; // true index
			swap(bills,s,t);
		}
		else if(strcmp(res.c_str(),"q") == 0) {
			//quit
			break;
		}
		else {
			continue;
		}				
	}			
	
	for(int y = 0; y < 10; y++) {
		bills[y].tab.clear();
		bills[y].inUse = false;
	}
	
	return 0;
}
