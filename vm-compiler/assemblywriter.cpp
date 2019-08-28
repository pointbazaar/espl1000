#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "assemblywriter.hpp"

using namespace std;

vector<string> write_assembly(vector<string> vm_instr){
	//the vm instructions are assumed to be clean 
	//which means no comments and no empty lines
	
	vector<string> res;
	
	//TODO
	
	res.push_back("section .text ;must be declared for linker (ld)");
	res.push_back("global		_start");
	
	res.push_back("_start: 	;tell linker entry point");
	
	res.push_back("mov eax,1	;system call number (sys_exit)");
	res.push_back("mov ebx,0     ;return code 0 indicates success");
	res.push_back("int 0x80	;call kernel");
	
	return res;
}
