#include <iostream>

using namespace std;

int main(int argc, char **argv) {
	
	Node *listA;
	Node *listB;
	
	listA->addNode(10);
	listB->addNode(15);

	traverse(listA);
	traverse(listB);
	
	return 0;
}
