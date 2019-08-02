/* Interpreter's GTree Class Header */

#ifndef GTREE_HPP
#define GTREE_HPP

#include <iostream>
#include <iomanip>
#include <string>

#define IN_SIZE 52
#define INIT_SUCCESS 0
#define INIT_FAILURE -1
#define NONE -1

using namespace std;

struct Node {

	bool valid;
	string keyword;
	struct Node* link[IN_SIZE];

	struct Node* add() {

		struct Node* node = (struct Node*) malloc(sizeof(struct Node)); 

		for(int i = 0; i < IN_SIZE; i++)
			node->link[i] = NULL;

		return node;
	}	
};

class GTree {

public:
	GTree();
	
	int init(string path);
	bool validate(string token);
	string complete(string token);
	
private:
	struct Node* root;	
	
};

#endif
