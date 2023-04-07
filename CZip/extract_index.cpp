#include "args.h"
#include <fstream>
#include <iostream>
#include "../CZipLib/czip_explorer.h"

#define string std::string


int f_extract_index() {
	args.require("-f");
	args.require("-i");
	args.require("-o");

	string& file = args.get("-f");
	string& out = args.get("-o");
	string& s_index = args.get("-i");
	if (s_index.empty() || !isdigit(s_index[0])) {
		std::cout << "Invalid value for '-i'." << std::endl;
		return EXIT_FAILURE;
	}
	int index = atoi(s_index.c_str());

	if (std::filesystem::exists(file) == false) {
		std::cout << "File does not exist!" << std::endl;
		return EXIT_FAILURE;
	}

	czip czip(file.c_str(), MODE_DECOMPRESS);
	czip_explorer exp(czip);

	std::cout << "Extracting...";
	std::cout << (exp.extract(index, out.c_str()) ? "\tOK" : "\tFAIL");
	std::cout << std::endl;

}