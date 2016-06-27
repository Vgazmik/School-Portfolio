#include <iostream>
struct lnode {
	int data;
	lnode *next;
};

int main() {
	lnode *head;
	lnode *tmp;
	lnode *tmp2;
	
	head = NULL;

	for(int i = 0; i < 10; i++) {
		tmp = new lnode;
		tmp->data = i;
		tmp->next = NULL;
		
		if(head == NULL) {
			head = tmp;
		}
		else {
			tmp2 = head;
			while(tmp2->next != NULL)
				tmp2 = tmp2->next;
			tmp2->next = tmp;
		}
	} 
	lnode *t;
	t = head;
	while(t != NULL) {
		std::cout<<t->data<<std::endl;
		t = t->next;
	}
}
