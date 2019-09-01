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
				
				printf("parsing: '%s'\n",s.c_str());
				
				//split it. as of now we have 
				//a maximum of 3 seperate words
				
				for(int i=0;i<3;i++){
					int x=s.find(" ");
					if(x!=string::npos){
						switch(i){
							case 0:
								instr.cmd=s.substr(0,x);
								break;
							case 1:
								instr.arg1=s.substr(0,x);
								break;
							case 2:
								instr.arg2=s.substr(0,x);
								break;
						}
						s=s.substr(x,s.length());
					}else{
						break;
					}
				}
				
				res.push_back(instr);
		}
		return res;
}

int VMInstr::is(std::string instr){
	return this->cmd.find(instr)!=string::npos;
}
