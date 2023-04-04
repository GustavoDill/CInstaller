#include <windows.h>

#include <iostream>
#include "compressed_file.h"

#define TEST_FILE "C:\\Users\\Gustavo\\Desktop\\animes.txt"
#define TEST_OUT_FILE "C:\\Users\\Gustavo\\Desktop\\animes.compressed.txt"
#define TEST_UN_FILE "C:\\Users\\Gustavo\\Desktop\\animes.uncompressed.txt"

int main() {

#if 0
	size_t size = 0;
	compressed_file compressed_file;
	std::ifstream _in_file(TEST_FILE, std::ios::in | std::ios::binary);
	std::ofstream _out_file(TEST_OUT_FILE, std::ios::out | std::ios::binary);


	read_raw_file(&compressed_file, &_in_file);
	compressed_file.set_name("ANIMES.txt");
	std::cout << get_z_err(compressed_file.compress_file()) << std::endl; // Compress
	write_compressed_file(&compressed_file, &_out_file); // Write output

	compressed_file.free(); // Free data  
#endif

	std::ifstream _in(TEST_OUT_FILE, std::ios::in | std::ios::binary);
	std::ofstream _out(TEST_UN_FILE, std::ios::out | std::ios::binary);
	compressed_file cf;
	read_compressed_file(&cf, &_in); _in.close();

	cf.uncompress_file();
	write_raw_file(&cf, &_out); _out.close();

}