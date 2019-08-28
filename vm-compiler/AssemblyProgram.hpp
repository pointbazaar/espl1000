#ifndef ASSEMBLYPROGRAM_HPP
#define ASSEMBLYPROGRAM_HPP

#include <vector>
#include <string>

class AssemblyProgram{
	
private:
	std::vector<std::string> prog; //the assembly program
	void cmd_with_comment(std::string cmd,std::string comment);
	bool indented=false;
	
public:
	std::vector<std::string> get_prog();
	void mov(std::string reg,int value,std::string comment);
	void label(std::string value,std::string comment);
	void section(std::string section,std::string comment);
	void global(std::string value,std::string comment);
	
	//write any command
	void any(std::string value,std::string comment); 
	
	
};

#endif
