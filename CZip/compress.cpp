#include "args.h"
#include <iostream>
#include "../CZipLib/czip.h"

using string = std::string;
int f_compress() {

	if (!((args.contains("-f")) || (args.contains("-d")))) {
		std::cout << "Missing arguments -f or -d" << std::endl;
		exit(EXIT_FAILURE);
	}
	args.require("-o");

	bool isFile = args.contains("-f");

	string& out = args.get("-o");
	string& input = isFile ? args.get("-f") : args.get("-d");
	if (std::filesystem::exists(input) == false) {
		std::cout << (isFile ? "File" : "Directory") << " does not exist!" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Compressing..." << std::endl;
	string filename = std::filesystem::path(input).filename().string();
	czip zip(out.c_str(), MODE_COMPRESS);
	if (isFile)
		zip.add_file(input.c_str(), filename, "");
	else
		zip.add_dir(input.c_str());
	std::cout << "Done." << std::endl;

	return 0;
 }