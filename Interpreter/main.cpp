#include <list>
#include <iterator>
#include <vector>
#include <fstream>

#include "gtree.hpp"

#define GPATH "/home/milo/Schreibtisch/Code/Projects/Interpreter/dragon/Interpreter/grammar"

void printex(string message);
void syntax_error(string e, int line);
void complete(string word);

int main(int argc, char** argv) {

	if(argc < 2)
		printex("Missing arguments...");

	string options = argv[1];

	if(options == "--complete") {

		if(argc < 3)
			printex("Missing keyword...");

		complete(argv[2]);
		
		return EXIT_SUCCESS;		
	}

	GTree gtree;

	if(gtree.init(GPATH) == INIT_FAILURE)
		printex("Grammar Tree Setup Failure");
	
	list <string> source; list <string> :: iterator it;

	ifstream File(argv[1]);

	if(File.good() == false)
		printex("Invalid Source Path");

	string line = "";

	while(getline(File, line))
		source.push_back(line);

	int line_counter = 1;

	for(it = source.begin(); it != source.end(); ++it) {
	
		if(gtree.validate(*it) == false) {

			string e = gtree.complete(*it);
			syntax_error(e, line_counter);
		}			

		line_counter += 1;
	}

	cout << "Success !" << endl;
	
	return EXIT_SUCCESS;
}

void syntax_error(string syn_ex, int line) {

	cout << "On line " << line << ": " << syn_ex << endl;

	exit(EXIT_FAILURE);
}

void printex(string message) {

	cout << message << endl;

	exit(EXIT_FAILURE);	
}

void complete(string word) {

	GTree gtree;

	if(gtree.init(GPATH) == INIT_FAILURE)
		printex("Grammar Tree Setup Failure");

	vector <string> out;
	stringstream str_stream(gtree.complete(word));

	string sub;

	while(getline(str_stream, sub, ' '))
		out.push_back(sub);

	string suggest = out.at(out.size() - 1);

	if(suggest.find("...") != string::npos) {
	
		cout << "No such keyword" << endl;
		return;
	}
	
	cout << suggest << endl;
}

