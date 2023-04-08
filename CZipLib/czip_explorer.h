#pragma once

#include "czip.h"
#include <fstream>

enum czip_entry_type : bool {
	CZIP_Directory,
	CZIP_File
};

struct czip_entry {
	czip_entry_type type = (czip_entry_type)CZIP_Directory;
	uLong data_pos = -1;
	czip_file_header header;
	int level = 0;
	char* path = nullptr;
	char* file = nullptr;
	~czip_entry();
};

int charcount(const char c, const char* str, size_t str_size);

__declspec(dllexport)
class czip_explorer {
	czip* m_czip;
	std::vector<czip_entry> m_entries;

	int read_level(const czip_entry& entry);
	int m_maxLevel = 0;

	void get_entries();
	void process_entries();
public:
	const int entries() const;
	const czip_entry& operator[] (int index) const;
	czip_explorer();
	czip_explorer(czip* _czip);
	bool extract(int index, const char* outputPath);
	czip_explorer(const czip_explorer& other) = delete;
	czip_explorer(czip_explorer&& other) = delete;
	void set_ref(czip* _czip);
	void update();
};
