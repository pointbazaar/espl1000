#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "removecomments.hpp"
#include "assemblywriter.hpp"

//https://www.geeksforgeeks.org/compiler-design-tutorials/

//https://de.wikipedia.org/wiki/Include-Guard

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
		//read the file
		ifstream f;
		f.open(filearg,ios::in);
		
		if(!f.good()){
			cout << "could not read file: " << filearg << endl;
			f.close();
			return 1;
		}else{
		
			string line;
			vector<string> lines;
			
			while(getline(f,line)){
				lines.push_back(line);
				//cout << line << endl;
			}
			
			//remove comments
			vector<string> clean_lines = remove_comments(lines);
			
			//cout << clean_lines.size() << " clean lines found " <<endl;
			cout << "--------------" << endl;
			for(int i=0;i<clean_lines.size();i++){
				cout << clean_lines.at(i) << endl;
			}
			
			//TODO: verify that all the lines have the correct format
			//and that the instructions are valid
			
			//TODO: generate assembly
			ofstream fout;
			string arg0=filearg;
			arg0.append(".asm");
			
			string asm_file_name=arg0;
			
			fout.open(asm_file_name,ios::out);
			
			if(!fout.good()){
				cout << "could not open file for writing: " << filearg << endl;
				return 1;
				fout.close();
			}else{
				//write assembly to file
				
				//this is from a tutorial
				//http://asm.sourceforge.net/intro/hello.html
				
				vector<string> asm_codes = write_assembly(clean_lines);
				
				for(int i=0;i<asm_codes.size();i++){
					fout << asm_codes.at(i) << endl;
				}
				
			}
			fout.close();
			
			//TODO: open process to call nasm to compile to machine code
			
			//$ nasm -f elf hello.asm  # this will produce hello.o ELF object file
			//$ ld -s -o hello hello.o # this will produce hello executable
			
			string arg1="nasm -f elf ";
			arg1.append(asm_file_name);
			
			string arg2="ld -melf_i386 -s -o hello ";
			arg2.append(filearg);
			arg2.append(".o");
			
			system(arg1.c_str());
			system(arg2.c_str());
		}
		f.close();
	}
    
    return 0;
}
