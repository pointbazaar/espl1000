#ifndef ASSEMBLYWRITER_HPP
#define ASSEMBLYWRITER_HPP

#include <string>
#include <vector>
#include "VMInstr.hpp"

std::vector<std::string> write_assembly(std::vector<VMInstr> vm_instr);

#endif
