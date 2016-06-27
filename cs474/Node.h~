class Node {
	public:
		int value;
		Node* left;
		Node* right;
		Node();
		Node(int v) {value = v;};
		Node(const Node &base);
		void addNode(int v, Node* t);
		void removeNode(int v, Node* t);
		bool find(int v, Node* t);
		void uni(const Node &a, const Node &b);
		void inter(const Node &a, const Node &b);
		void traverse(const Node* t);
};

Node::Node() {
	value = -1;
	left = NULL;
	right = NULL;
}

Node::Node(const Node &base) {
	value = base.value;
	*left = *base.left;
	*right = *base.right;
}

void Node::addNode(int v, Node* t) {
	if(t == NULL) {
		*t = Node(v);
	}
	else {
		if(v < t->value)
			addNode(v, t->left);
		else if(v > t->value)
			addNode(v, t->right);
		else {
			cout << "Duplicate Value Found!\nDiscarding...\n" 
			     << endl;
		}
	}	
}

bool Node::find(int v, Node* t) {
	if(t == NULL)
		return false;
	else {
		if(v < t->value)
			find(v, t->left);
		else if(v > t->value)
			find(v, t->right);
		else
			return true;
	}
}
	









