#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>

enum cook{RAR = 0, MED = 1, WEL = 2};
enum bread{WHL = 0, WHT = 1, RYE = 2};

class foodItem {
	protected:
		char name[20];
		double price;
		int calories;
	public:
		char* getName() {return name;}
		double getPrice() {return price;}
		int getCals() {return calories;}
		void baseDesc(){std::cout<<"\nName: "<<name<<"\nPrice: $"<<price<<"\nCalories: "<<calories<<std::endl;}
		virtual void describe(){std::cout<<"\nMmm! Delicious Food Item!\n"<<std::endl;}
		virtual void copy(foodItem *cme) {std::cout<<"\nWheeeeeeeeeeeeeeeeeeeee!\n";}
		foodItem();
		foodItem(char *n, double p, int c);
};

foodItem::foodItem(char *n, double p, int c) {
	strcpy(name,n);
	price = p;
	calories = c;
}

class salmon : public foodItem {
	protected:
		int cook_temp;   //names a value from enum cook
		char side[80];     //c-string describing side dish
	public:
		salmon();
		salmon(char *n, double p, int c, int ct, char* s);
		virtual void describe(){
			std::cout <<"\nSALMON\n"; 
			baseDesc(); 
			std::cout << "\nSide Dish: " << side; 
			std::cout << "\nCooked: ";
					
			switch(cook_temp) {
				case 0: std::cout << "Rare" << std::endl;
					break;
				case 1: std::cout << "Medium" << std::endl;
					break;
				case 2: std::cout << "Well-Done" << std::endl;
					break;
				default: std::cout << "???" << std::endl;
					break;	
			}
		}
		virtual void copy(salmon *cme) {
			memset(this->name,' ',20);
			strcpy(this->name,cme->name);
			this->price = cme->price;
			this->calories = cme->calories;
			//
			this->cook_temp = cme->cook_temp;
			memset(this->side,' ',80);
			strcpy(this->side,cme->side);
		}	
};

salmon::salmon(char *n, double p, int c, int ct, char* s) : foodItem(n, p, c){
	cook_temp = ct;
	strcpy(side,s);
}

class twich : public foodItem {
	protected:
		int bread_t;             //type of bread
		std::vector<char*> cond; //infinite condiments
	public:
		twich();
		twich(char *n, double p, int c, int bt);

		virtual void describe() {
			std::cout <<"\nTURKEY SANDWICH\n"; 
			baseDesc(); 
			std::cout << "\nBread Type: ";
				
			switch(bread_t) {
				case 0: std::cout << "Whole" << std::endl;
					break;
				case 1: std::cout << "Wheat" << std::endl;
					break;
				case 2: std::cout << "Rye" << std::endl;
					break;
				default: std::cout << "???" << std::endl;
					break;	
			}
			std::cout << "\nCondiments\n----------------\n ";
			for(int i = 0; i < (int)cond.size(); i++) {
				if((i > 0) && (i%3 == 0))
					std::cout<<"\n";
				std::cout<<cond[i];
				if(i < (int)cond.size() - 1)
					std::cout<<", ";
			}
			std::cout<<std::endl;
		}
		virtual void copy(twich *cme) {
			memset(this->name,' ',20);
			strcpy(this->name,cme->name);
			this->price = cme->price;
			this->calories = cme->calories;
			//
			this->bread_t = cme->bread_t;
			this->cond.clear();
			for(int j = 0; j < (int)cme->cond.size(); j++)
				this->cond.push_back(cme->cond[j]);
		}
		void addC(std::string cnd);
};

twich::twich(char *n, double p, int c, int bt) : foodItem(n,p,c){
	bread_t = bt;
	std::string con = "";
	std::cout << "\nAnd what would you like with that?\n";
	
	std::cout<<"\nAdd a condiment (type \"done\" to stop): ";
	std::getline(std::cin,con);
	std::cout<<"\n";
	
	while(strcmp(con.c_str(),"done") != 0) {
		this->addC(con);
		std::cout<<"\nAdd a condiment (type \"done\" to stop): ";
		std::getline(std::cin,con);
		std::cout<<"\n";
	}
}

void twich::addC(std::string cnd) {
	char *con = new char[20];
	strcpy(con,cnd.c_str());
	this->cond.push_back(con);
}

class eggc : public foodItem {
	protected:
		int oz;        //ounces in serving
		char side[80]; //c-string describing side dish
	public:
		eggc();
		eggc(char *n, double p, int c, int z, char* s);
		virtual void describe() {
			std::cout<<"\nEGGPLANT CASSEROLE\n";
			baseDesc();
			std::cout <<"\nServing Size: " 
				  << oz 
				  << " ounces\nSide Dish: " 
				  << side
				  << std::endl;
		}
		virtual void copy(eggc *cme) {
			memset(this->name,' ',20);
			strcpy(this->name,cme->name);
			this->price = cme->price;
			this->calories = cme->calories;
			//
			this->oz = cme->oz;
			memset(this->side,' ',80);
			strcpy(this->side,cme->side);
		}
};

eggc::eggc(char *n, double p, int c, int z, char *s) : foodItem(n,p,c) {
	oz = z;
	strcpy(side,s);
}

struct orderList {
	std::vector<foodItem*> tab;
	int seatNumber;
	bool inUse;
};

void examine(orderList l) {
	if(l.seatNumber < 1 || l.seatNumber > 10) {
		std::cout<<"\nError, Invalid Seat Selection\n";
		return;
	}
	if((int)l.tab.size() == 0) {
		std::cout<<"\nSeat " 
			 << l.seatNumber 
			 << " has an empty tab.\n";
		return;
	}
	for(int i = 0; i < (int)l.tab.size(); i++) {
		l.tab[i]->describe();
	}
}

void swap(orderList *lists, int a, int b) {
	if(!((a >= 0 && a <= 10) && (b >= 0 && b <= 10))) {
		std::cout<<"\nError, Invalid Seat Selection\n";
		return;
	}
	else {
		orderList tmp = lists[a];
		orderList tm2 = lists[b];
		lists[a].tab.clear();
		lists[b].tab.clear();
		for(int x = 0; x < (int)tm2.tab.size(); x++)
			lists[a].tab.push_back(tm2.tab[x]);
		for(int y = 0; y < (int)tmp.tab.size(); y++)
			lists[b].tab.push_back(tmp.tab[y]);
	}
}

void copy(orderList *lists, int a, int b) {
	if(!((a >= 0 && a <= 10) && (b >= 0 && b <= 10))) {
		std::cout<<"\nError, Invalid Seat Selection\n";
		return;
	}
	else {
		lists[b].tab.clear();
		for(int x = 0; x < (int)lists[a].tab.size(); x++) 
			lists[b].tab.push_back(lists[a].tab[x]);
		lists[b].inUse = true;
	}
}
