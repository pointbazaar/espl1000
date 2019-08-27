#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "removecomments.hpp"

using namespace std;

int main(int argc, char** argv);


int main(int argc, char** argv)
{
	//argv[0] is name of the program
    cout << "DracoVM Compiler" << endl;
    
    //open the .dracovm file supplied as argument
    
    if(argc<2){
		cout << "usage: dracovmc file.dracovm" << endl; 
	}else{
		char* filearg = argv[1];
		//TODO: read the file
		ifstream f;
		f.open(filearg,ios::in);
		
		if(!f.good()){
			cout << "could not read file: " << filearg << endl;
		}else{
		
			string line;
			vector<string> lines;
			
			while(getline(f,line)){
				lines.push_back(line);
				//cout << line << endl;
			}
			
			vector<string> clean_lines = remove_comments(lines);
			
			//cout << clean_lines.size() << " clean lines found " <<endl;
			cout << "--------------" << endl;
			for(int i=0;i<clean_lines.size();i++){
				cout << clean_lines.at(i) << endl;
			}
		}
		f.close();
	}
    
    return 0;
}
