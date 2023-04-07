#include "args.h"
#include <iostream>
#include <filesystem>
#include "../CZipLib/czip.h"

//#define sc std::cout
//#define el std::endl

int f_extract() {
	using string = std::string;
	args.require("-f");
	args.require("-o");

	string& out = args.get("-o");
	string& input = args.get("-f");
	if (std::filesystem::exists(input) == false) {
		std::cout << "File does not exist!" << std::endl;
		return EXIT_FAILURE;
	}
	string filename = std::filesystem::path(input).filename().string();
	czip zip(out.c_str(), MODE_DECOMPRESS);
	std::cout << "Extracting " << zip.total_files() << " files..." << std::endl;
	if (zip.extract_dir(out) == false) {
		std::cout << "Failed." << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Done." << std::endl;
	return 0;
	//sc << "File: " << args.get("-f") << el;
}