#include <iostream>
#include <import_lib.h>
#include "args.h"
#include "help.h"
#include "binds.h"

IMPORT_LIB("../CZipLib/lib", "CZipLib");


#define PRINT_HELP_AND_EXIT() { help(); exit(-1); }

// Binds an action
#define BIND(name, callback)if (args[1] == name) { exit(callback()); }

Args args;

CREATE_ARG_VARIABLES;

int main(int argc, char** argv) {

	ADD_PROGRAM_ARG("c", "<action> Compresses the target [-f/-d -o]");
	ADD_PROGRAM_ARG("e", "<action> Extracts the target [-f -o]");
	ADD_PROGRAM_ARG("l", "<action> Lists the contents of a czip file [-f]");
	ADD_PROGRAM_ARG("ei", "<action> Extract one file from the specified index [-f -i -o]");
	ADD_PROGRAM_ARG("-o", "Specifies the output");
	ADD_PROGRAM_ARG("-i", "Specifies the index of the file");
	ADD_PROGRAM_ARG("-f", "Specifies the target is a file, the next parameter should be the path to the file");
	ADD_PROGRAM_ARG("-d", "Specifies the target is a folder, the next parameter should be the path to the folder");

	if (argc == 1) PRINT_HELP_AND_EXIT();
	PARSE_ARGS(argc, argv);

	BIND("c", f_compress);
	BIND("e", f_extract);
	BIND("l", f_list);

}