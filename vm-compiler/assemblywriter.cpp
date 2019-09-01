#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "assemblywriter.hpp"
#include "AssemblyProgram.hpp"
#include "VMInstr.hpp"

using namespace std;
	
vector<string> write_assembly(vector<VMInstr> codes){
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
	for(int i=0;i<codes.size();i++){
		VMInstr code = codes.at(i);
		printf("	translating '%s'  '%s'  '%s'\n",code.cmd.c_str(),code.arg1.c_str(),code.arg2.c_str());
		string instr=code.cmd;
		
		if(code.is("return")){
			prog.ret();
		}else if(code.is("add")){
			//take topmost 2 items from stack, and add them
			prog.pop("ebx");
			prog.pop("eax");
			prog.add("eax","ebx");
			prog.push("eax");
		}else if(code.is("sub")){
			//take topmost 2 items from stack, and add them
			prog.pop("ebx");
			prog.pop("eax");
			prog.sub("eax","ebx");
			prog.push("eax");
		}else if(code.is("push")){
			//consider the const case
			if(
				code.arg1.compare("0")==0
				|| code.arg1.compare("1")==0
			){
				
				prog.mov("eax",atoi(code.arg1.c_str()),"");
				prog.push("eax");
			}else{
				printf("error 434 '%s' \n",code.arg1.c_str());
				exit(1);
			}
		}else if(code.is("pop")){
			prog.pop(code.arg1);
		}else{
			printf("unconsidered case in assemblywriter.cpp: %s\n",code.cmd.c_str());
			exit(1);
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


