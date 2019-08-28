#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "AssemblyProgram.hpp"

using namespace std;
	
void AssemblyProgram::section(string section, string comment){
	this->cmd_with_comment("section "+section,comment);
}

void AssemblyProgram::mov(string reg,int value,string comment){
	this->cmd_with_comment("mov "+reg+","+to_string(value),comment);
}

void AssemblyProgram::label(string value,string comment){
	this->cmd_with_comment(value+":",comment);
}

vector<string> AssemblyProgram::get_prog(){
	return this->prog;
}

void AssemblyProgram::cmd_with_comment(std::string cmd,std::string comment){
	this->prog.push_back(cmd+"\t\t;"+comment);
}

void AssemblyProgram::global(std::string value,std::string comment){
	this->cmd_with_comment("global "+value,comment);
}

void AssemblyProgram::any(std::string value,std::string comment){
	this->cmd_with_comment(value,comment);
}
