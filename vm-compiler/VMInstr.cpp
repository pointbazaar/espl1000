#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "assemblywriter.hpp"
#include "AssemblyProgram.hpp"
#include "VMInstr.hpp"

using namespace std;

vector<VMInstr> VMInstr::make_vm_instrs(vector<string> vmcodes){
		vector<VMInstr> res;
		for(int i=0;i<vmcodes.size();i++){
				string s = vmcodes.at(i);
				VMInstr instr;
				//split it. as of now we have 
				//a maximum of 3 seperate words
				int x=s.find(" ");
				if(x!=string::npos){
					instr.cmd=s.substr(0,x);
				}
				string s2=s.substr(x,s.length());
				int x2=s2.find(" ");
				if(x2!=string::npos){
					instr.arg1=s2.substr(0,x2);
					instr.arg2=s2.substr(x2,s2.length());
				}else{
					instr.arg2=s2;
				}
				res.push_back(instr);
		}
		return res;
}

int VMInstr::is(std::string instr){
	return this->cmd.find(instr)!=string::npos;
}
