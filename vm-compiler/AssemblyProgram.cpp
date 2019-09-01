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
	this->indented=false;
	this->cmd_with_comment("section "+section,comment);
	this->indented=true;
}

void AssemblyProgram::mov(string reg,int value,string comment){
	this->cmd_with_comment("mov "+reg+","+to_string(value),comment);
}
void AssemblyProgram::mov(string reg,int value){
	this->mov(reg,value,"");
}
void AssemblyProgram::mov(string reg,string value){
	this->cmd_with_comment("mov "+reg+","+value,"");
}

void AssemblyProgram::jmp(string label){
	string s="jmp ";
	s.append(label);
	this->cmd_with_comment(s,"");
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
	//we should use our own handling of this,
	//popping the program counter of the caller?
}

void AssemblyProgram::push(std::string reg){
	string s="push ";
	s.append(reg);
	this->cmd_with_comment(s,"");
}

void AssemblyProgram::pop(std::string reg){
		string s="pop ";
		s.append(reg);
		this->cmd_with_comment(s,"");
}

void AssemblyProgram::add(std::string reg1,std::string reg2){
	string s="add ";
	s.append(reg1);
	s.append(",");
	s.append(reg2);
	this->cmd_with_comment(s,"");
}

void AssemblyProgram::sub(std::string reg1,std::string reg2){
	string s="sub ";
	s.append(reg1);
	s.append(",");
	s.append(reg2);
	this->cmd_with_comment(s,"");
}
