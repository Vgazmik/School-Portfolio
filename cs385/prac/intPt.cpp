#include <iostream>

void changeV(int * t, int p) {
	*t = p;
}

int main() {
	int val = 0;
	int * a;
	a = &val;
	std::cout<<val<<std::endl;
	changeV(a, 5);
	std::cout<<val<<std::endl;
}
