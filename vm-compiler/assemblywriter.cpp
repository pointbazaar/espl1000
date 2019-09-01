#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "assemblywriter.hpp"
#include "AssemblyProgram.hpp"

using namespace std;
	
vector<string> write_assembly(vector<string> vm_instr){
	//the vm instructions are assumed to be clean 
	//which means no comments and no empty lines
	
	//there should be some method to verify that the vm instructions
	//form a correct dracovm program
	
	//vector<string> res;
	
	AssemblyProgram prog;
	
	prog.section(".text","must be declared for linker (ld)");
	prog.global("_start","");
	prog.label("_start","tell linker entry point");
	
	//TODO: translate the codes here
	for(int i=0;i<vm_instr.size();i++){
		string instr = vm_instr.at(i);
		
		if(instr.find("return")!=string::npos){
			
		}
	}
	
	//TEMP
	prog.any("mymem 10 0","");
	prog.any("mov eax [mymem]","");
	
	prog.mov("eax",1,"system call number (sys_exit)");
	prog.mov("ebx",0,"return code 0 indicates success");
	prog.any("int 0x80","call kernel");
		
	return prog.get_prog();
}

/*
	res.push_back("section .text ;must be declared for linker (ld)");
	res.push_back("global		_start");
	
	res.push_back("_start: 	;tell linker entry point");
	
	res.push_back("mov eax,1	;system call number (sys_exit)");
	res.push_back("mov ebx,0     ;return code 0 indicates success");
	res.push_back("int 0x80	;call kernel");
*/


