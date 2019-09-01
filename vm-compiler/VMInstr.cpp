#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "assemblywriter.hpp"
#include "AssemblyProgram.hpp"
#include "VMInstr.hpp"

using namespace std;
vector<string> split(vector <string> res, string s) {
	int pos = s.find(" ");
	if(pos == string::npos) {
		res.push_back(s);
	} else {
		res.push_back( s.substr(0, pos) );
		s = s.substr(pos+1, s.size());
		split(res, s).swap(res);
	}
	return res;
}


vector<VMInstr> VMInstr::make_vm_instrs(vector<string> vmcodes){
		vector<VMInstr> res;
		for(int i=0;i<vmcodes.size();i++){
				string s = vmcodes.at(i);
				VMInstr instr;
				
				printf("parsing: '%s'\n",s.c_str());
				
				//split it. as of now we have 
				//a maximum of 3 seperate words
				
				vector<string> parts;
				split(parts,s).swap(parts);
				for(int j=0;j<parts.size();j++){
					string ss=parts.at(j);
					switch(j){
						case 0:
							instr.cmd=ss;
							break;
						case 1:
							instr.arg1=ss;
							break;
						case 2:
							instr.arg2=ss;
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
