#pragma once
#include "czip_file.h"
#include <fstream>
#include <filesystem>


#if defined _MSVC_LANG
#if _MSVC_LANG < 201703L
#error "'czip' library requires C++17 or newer"
#endif
#elif __cplusplus < 201703L
#error "'czip' library requires C++17 or newer"
#endif

#define MODE_COMPRESS 0
#define MODE_DECOMPRESS 1
typedef int MODE;

inline std::string __zip_get_rel_dir(const std::string& root, const std::string& current) {
	return std::filesystem::path(current).lexically_relative(root).string();
}


#pragma pack(1)
struct czip_header {
	int32_t total_files = 0;
	int64_t total_raw_size = 0;
};

class czip {
	MODE m_mode;
	czip_header m_header;

	int32_t current_file;

public:
	czip_file current;
	std::ifstream _in;
	std::ofstream _out;

	inline czip(const char* path, MODE mode = MODE_COMPRESS) : m_mode(mode) {
		open(path);
	}

	inline void write_header() {
		_out.seekp(0);
		_out.write((char*)(&m_header), sizeof m_header);
	}

	inline void read_header() {
		_in.read((char*)&m_header, sizeof m_header);
	}
	// Pls no '\\' at the end 


	virtual bool add_dir(const std::string& root, const std::string& relpath = "");
	virtual int extract_dir(const std::string& output_path);
	void add_file(const char* filepath, const std::string& data_name, const std::string& data_path);
	czip_file* get_file(const char* path, const char* name);
	czip_file* operator[](int file_index);
	czip_file* read_next();

	inline constexpr int total_files() const {
		return m_header.total_files;
	}

	inline czip_file* read_first() {
		_in.seekg(sizeof m_header); // Skip header
		current_file = -1;
		return read_next(); // Read first file
	}




	inline void open(const char* path) {
		switch (m_mode) {
			case MODE_COMPRESS:
				_out.open(path, std::ios::binary | std::ios::out);
				write_header();// Reserve space for the header of course

				break;

			case MODE_DECOMPRESS:
				_in.open(path, std::ios::binary | std::ios::out);
				int data_pos = _in.tellg();
				read_header();
				int c_pos = _in.tellg();
				current_file = -1; // Read first
				read_next();
				break;
		}
	}

	inline void close() {
		switch (m_mode) {
			case MODE_COMPRESS: _out.close(); break;
			case MODE_DECOMPRESS: _in.close(); break;
		}
	}
	inline ~czip() {
		write_header();
		close();
	}

	friend class czip_explorer;
};