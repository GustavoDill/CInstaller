#pragma once
#include <zlib/zlib.h>
#include <zlib/zlib_lib.h>
#include <fstream>
#include <filesystem>
typedef unsigned char byte;

#define COMP_SAFE_STRUCT 1
#define ___ENABLE_GET_Z_ERR 1
#define ___ENABLE_PRINT_BUFFER 0

#pragma pack(1)
struct compressed_file_header {
	uLong compressed_data_size = 0;
	uLong raw_data_size = 0;
	byte name_length = 0;
};

struct compressed_file {
	byte* raw_data = nullptr;
	byte* compressed_data = nullptr;
	char* file_name = nullptr;
	compressed_file_header header;


	int compress_file() {
		if (raw_data == nullptr) return Z_BUF_ERROR;
		if (compressed_data != nullptr) delete[] compressed_data;

		compressed_data = new byte[header.raw_data_size];
		header.compressed_data_size = header.raw_data_size;
		return compress(compressed_data, &this->header.compressed_data_size, raw_data, header.raw_data_size);
	}
	int uncompress_file() {
		if (compressed_data == nullptr) return Z_BUF_ERROR;
		if (raw_data != nullptr) delete[] raw_data;

		raw_data = new byte[header.raw_data_size];
		return uncompress(raw_data, &header.raw_data_size, compressed_data, header.compressed_data_size);
	}

	void set_name(const std::string& name) {
		file_name = new char[name.size()+1];
		memcpy(file_name, name.data(), name.size() + 1);
		header.name_length = name.size();
	}

	inline void free() {
		delete[] raw_data;
		delete[] compressed_data;
		delete[] file_name;
		raw_data = nullptr;
		compressed_data = nullptr;
		file_name = nullptr;

		memset(&header, 0, sizeof(header)); // Set everything to 0

	}
#if COMP_SAFE_STRUCT
	inline ~compressed_file() {
		free();
	}
#endif
};

void read_compressed_file(compressed_file* _file, std::ifstream* _in) {
	_in->read((char*)&_file->header, sizeof(_file->header.compressed_data_size) * 2);// Read headers
	_file->compressed_data = new byte[_file->header.compressed_data_size]; // Allocate memory specified in the header
	_in->read((char*)_file->compressed_data, _file->header.compressed_data_size); // Read compressed data
}

void read_raw_file(compressed_file* _file, std::ifstream* _in) {
	uLong p = _in->tellg();
	_in->seekg(0, std::ios::end);
	_file->header.raw_data_size = _in->tellg();
	_in->seekg(p);
	_file->raw_data = new byte[_file->header.raw_data_size];
	_in->read((char*)(_file->raw_data), _file->header.raw_data_size);
}

void write_compressed_file(compressed_file* _file, std::ofstream* _out) {
	_out->write((char*)&(_file->header), sizeof(_file->header)); // Write header
	if (_file->header.name_length > 0) // If has name
		_out->write((char*)_file->file_name, _file->header.name_length); // Write name
	_out->write((char*)_file->compressed_data, _file->header.compressed_data_size); // Write compressed data
}

void write_raw_file(compressed_file* _file, std::ofstream* _out) {
	_out->write((char*)(_file->raw_data), _file->header.raw_data_size);
}

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
#if ___ENABLE_GET_Z_ERR
const char* get_z_err(int err) {
	switch (err) {
		case Z_OK: return "Z_OK";
		case Z_STREAM_END: return "Z_STREAM_END";
		case Z_NEED_DICT: return "Z_NEED_DICT";
		case Z_ERRNO: return "Z_ERRNO";
		case Z_STREAM_ERROR: return "Z_STREAM_ERROR";
		case Z_VERSION_ERROR: return "Z_VERSION_ERROR";
		case Z_BUF_ERROR:return "Z_BUF_ERROR\n";
		case Z_MEM_ERROR:return"Z_MEM_ERROR\n";
		case Z_DATA_ERROR: return "Z_DATA_ERROR\n";
		default: return "Unknown error";
	}
}
#endif
