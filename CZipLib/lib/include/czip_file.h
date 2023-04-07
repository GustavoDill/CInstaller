#pragma once
#include <zlib/zlib.h>
#include <zlib/zlib_lib.h>
#include <fstream>
#include <filesystem>
typedef unsigned char byte;

//#define ___ENABLE_GET_Z_ERR 1
#define ___ENABLE_PRINT_BUFFER 0

#pragma pack(1)
struct czip_file_header {
	uLong compressed_data_size = 0;
	uLong raw_data_size = 0;
	byte name_length = 0;
	byte path_length = 0;
};

class czip_file {

	char* file_name = nullptr;
	char* path_name = nullptr;

public:
	czip_file_header header;
	byte* raw_data = nullptr;
	byte* compressed_data = nullptr;
	inline constexpr char* name() const { return file_name; }
	inline constexpr char* path() const { return path_name; }

	int compress_file();
	int uncompress_file();

	void set_name(const std::string& name);

	void set_path(const std::string& path);

	inline void free() {
		delete[] raw_data;
		delete[] compressed_data;
		delete[] file_name;
		delete[] path_name;
		raw_data = nullptr;
		compressed_data = nullptr;
		file_name = nullptr;
		path_name = nullptr;

		memset(&header, 0, sizeof(header)); // Set everything to 0

	}
	inline ~czip_file() {
		free();
	}
	friend void read_compressed_file(czip_file*, std::ifstream*);
	friend void write_compressed_file(czip_file*, std::ofstream*);
	friend void read_raw_file(czip_file*, std::ifstream*);
	friend void write_raw_file(czip_file*, std::ofstream*);
};

void read_compressed_file(czip_file* _file, std::ifstream* _in);
void read_raw_file(czip_file* _file, std::ifstream* _in);
void write_compressed_file(czip_file* _file, std::ofstream* _out);
void write_raw_file(czip_file* _file, std::ofstream* _out);
const char* get_z_err(int err);


#if ___ENABLE_PRINT_BUFFER
#include <iostream>
#include <sstream>
void print_buffer(byte* buff, uLong size) {
	std::stringstream ss;
	for (int i = 0; i < size; i++) {
		ss << std::uppercase << std::hex << (int)buff[i] << " ";
	}
	std::cout << ss.str() << std::endl;
}


#endif


