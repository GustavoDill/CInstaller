#pragma once
#include <iostream>

#define ADD_PROGRAM_ARG(name, description) arg_names.push_back(name); arg_desc.push_back(description)
#define PRINT_ARG(arg, desc) std::cout << '\t' << arg  << "\t:\t" << desc << std::endl;

extern std::vector<std::string> arg_names;
extern std::vector<std::string> arg_desc;

#define CREATE_ARG_VARIABLES Args args; std::vector<std::string> arg_names; std::vector<std::string> arg_desc

#define sc std::cout
void help() {
	sc << "Usage: czip.exe <action> [options]\n" << std::endl;

	for (int i = 0; i < arg_names.size(); i++) {
		PRINT_ARG(arg_names[i], arg_desc[i]);
	}

	sc << "\nExample usage:\n[Compress]\tczip.exe c -d \"C:\\mydirectory\" -o \"C:\\mydirectory.czip\"";
	sc << "\n[Extract]\tczip.exe e -f \"C:\\mydirectory.czip\" -o \"C:\\mydirectory\"";
	sc << std::endl;
}