#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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
		
		string line;
		
		while(getline(f,line)){
			cout << line << endl;
		}
		
		f.close();
	}
    
    return 0;
}
