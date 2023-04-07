#include "args.h"
#include "../CZipLib/czip_explorer.h"
#include <filesystem>
#include <iostream>

int f_list() {
	using string = std::string;

	args.require("-f");

	string& file = args.get("-f");
	if (std::filesystem::exists(file) == false) {
		std::cout << "File does not exist!" << std::endl;
		return EXIT_FAILURE;
	}

	czip czip(file.c_str(), MODE_DECOMPRESS);
	czip_explorer zipe(czip);
	
	for (int i = 0; i < zipe.entries(); i++) {
		std::cout << '[' << i << ']' << ' ';
		if (zipe[i].path != nullptr)
			std::cout << '\\' << zipe[i].path;
		std::cout << '\\' << zipe[i].file;
		int d = zipe[i].header.raw_data_size / 1024;
		std::cout << " [" << (d == 0 ? 1 : d) << " Kb]";
		std::cout << std::endl;
		
	}
	return 0;
}