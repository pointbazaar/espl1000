#include <fstream>
#include <queue>
#include "gtree.hpp"

/* Function Prototypes */

int get_id(char c);
struct Node* jump_to(struct Node* entry, string token);

/* Function Prototypes */

GTree::GTree() {

	this->root = (struct Node*) malloc(sizeof(struct Node));

	if(this->root == NULL) {
		// Allocation Exception...should not happen
	}
}

int GTree::init(string path) {

	ifstream File(path);

	string line = "";

	if(File.good() == false)
		return INIT_FAILURE;

	while(getline(File, line)) {

		struct Node* parent = this->root;
	
		for(int i = 0; i < line.length(); i++) {

			int link_id = get_id(line[i]);  

			if(link_id == NONE)
				return INIT_FAILURE;

			if(parent->link[link_id] == NULL)
				parent->link[link_id] = parent->add();
				
			parent = parent->link[link_id];						
		}

		parent->valid = true;
		parent->keyword = line;
	}

	return INIT_SUCCESS;
}

bool GTree::validate(string token) {

	struct Node* parent = this->root;

	for(int i = 0; i < token.length(); i++) {

		int link_id = get_id(token[i]);

		if(link_id == NONE)
			return false;

		if(parent->link[link_id] == NULL) {

			return false;
		}
	
		parent = parent->link[link_id];
	}

	if(parent->valid == false)
		return false;

	return true;
}

string GTree::complete(string token) {

	struct Node* parent = jump_to(this->root, token);

	queue <struct Node*> level;

	string res_ex = "Could not resolve: " + token + "...";

	if(parent == NULL)
		return res_ex;

	if(parent->valid == true)
		return "No completion needed";

	for(int i = 0; i < IN_SIZE; i++) {

		if(parent->link[i] != NULL)
			level.push(parent->link[i]);
	}										

	while(level.empty() == false) {

		struct Node* current = level.front();

		if(current->valid == true) {
		
			return res_ex + "Did you mean: " + current->keyword;
			
		} else {

			for(int i = 0; i < IN_SIZE; i++) {

				if(current->link[i] != NULL)
					level.push(current->link[i]);
			}
		}

		level.pop();
	}
	
}

struct Node* jump_to(struct Node* entry, string token) {

	for(int i = 0; i < token.length(); i++) {

		int link_id = get_id(token[i]);

		if(link_id == NONE)
			return NULL;

		if(entry->link[link_id] == NULL)
			return NULL;

		entry = entry->link[link_id];
	}

	return entry;
}

int get_id(char c) {

	int asc = (int) c;

	if(asc > 96 && asc < 123) {
	
		return asc - 97;
				
	} else if(asc > 64 && asc < 91) {

		return asc - 65;			
	}

	return NONE;
}
