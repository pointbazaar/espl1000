/* CodeRender's main */

// C++ Headers
#include <vector>

// Own Headers
#include "renderer.hpp"
#include "pixmap.hpp"

/* Function Prototypes */

void printex(string message);
bool is_digit(string input);

/* Function Prototypes */

int main(int argc, char** argv) {

	//if(argc < 5)
	//	printex("Usage: -r <text to render> -l <line> [optional: --save]");

	vector <string> arg_ls;

	//for(int i = 1; i < 5; i++)
	//	arg_ls.push_back(argv[i]);

	//if(arg_ls[0] != "-r")
	//	printex("Unknown Option...Try -r instead.");

	//if(arg_ls[2] != "-l")
	//	printex("Unknown Option...Try -l instead.");

	//if(is_digit(arg_ls[3]) == false)
	//	printex("Invalid argument for Option -l ...Needed: Integer");

    //crend -l 4
    int line = atoi(argv[2]);

    string input;
    //read the input
    //cin >> input;
    getline(cin,input);

    //cout << input << endl;
    //exit(1);

	Renderer rend(7, 7);	

	Pixmap pixmap = rend.to_ppm(input, line);	// toDo: Convert argv[4] to Integer and pass to Render::rend

	pixmap.save("/home/alex/a.ppm");

	pixmap.show();
	
	return EXIT_SUCCESS;
}

void printex(string message) {

	cout << message << endl;
	exit(EXIT_FAILURE);
}

bool is_digit(string input) {

	for(int i = 0; i < input.length(); i++) {

		int ascii = (int) input[i];

		if(ascii < 48 || ascii > 57)
			return false;		
	}

	return true;
}