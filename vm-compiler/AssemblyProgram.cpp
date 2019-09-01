#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "AssemblyProgram.hpp"

using namespace std;

//the assembly program class
//should have a method to verify it?
	
void AssemblyProgram::section(string section, string comment){
	this->cmd_with_comment("section "+section,comment);
}

void AssemblyProgram::mov(string reg,int value,string comment){
	this->cmd_with_comment("mov "+reg+","+to_string(value),comment);
}

void AssemblyProgram::label(string value,string comment){
	//lines after a label should be indented,
	//the label itself should not be indented
	this->indented=false;
	this->cmd_with_comment(value+":",comment);
	this->indented=true;
}

vector<string> AssemblyProgram::get_prog(){
	return this->prog;
}

void AssemblyProgram::cmd_with_comment(std::string cmd,std::string comment){
	//string prefix="";
	//string res="";
	//sprintf(&res,"%20s ;%20s",cmd,comment);
	ostringstream ss;
	
	if(this->indented){
		//this->prog.push_back("\t"+cmd+"\t;"+comment);
		ss << "\t" << left << setfill(' ') << setw(10) << cmd << "\t" << left << setfill(' ') << setw(25) << (";"+comment);
	}else{
		//this->prog.push_back(cmd+"\t\t;"+comment);
		ss << "" << left << setfill(' ') << setw(10) << cmd << "\t\t" << left << setfill(' ') << setw(25) << (";"+comment);
	}
	
	this->prog.push_back(ss.str());
}

void AssemblyProgram::global(std::string value,std::string comment){
	this->cmd_with_comment("global "+value,comment);
}

void AssemblyProgram::any(std::string value,std::string comment){
	this->cmd_with_comment(value,comment);
}

void AssemblyProgram::ret(){
	this->cmd_with_comment("ret","");
}
