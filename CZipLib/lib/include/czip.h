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

std::string __zip_get_rel_dir(const std::string& root, const std::string& current);


#pragma pack(1)
struct czip_header {
	int32_t total_files = 0;
	int64_t total_raw_size = 0;
};

__declspec(dllexport)
class czip {
	MODE m_mode;
	czip_header m_header;
	int32_t current_file;

public:
	czip_file current;
	std::ifstream _in;
	std::ofstream _out;
	czip(MODE mode = MODE_COMPRESS);
	czip(const char* path, MODE mode = MODE_COMPRESS);
	void write_header();
	void read_header();
	virtual bool add_dir(const std::string& root, const std::string& relpath = "");
	virtual int extract_dir(const std::string& output_path);
	void add_file(const char* filepath, const std::string& data_name, const std::string& data_path);
	czip_file* get_file(const char* path, const char* name);
	czip_file* operator[](int file_index);
	czip_file* read_next();
	constexpr int total_files() const;
	czip_file* read_first();
	void open(const char* path);
	void close();
	void set_mode(MODE mode);
	~czip();

	friend class czip_explorer;
};