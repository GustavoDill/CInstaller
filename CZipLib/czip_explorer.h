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
	~czip_entry() {
		delete[] path;
		delete[] file;
	}
};

int charcount(const char c, const char* str, size_t str_size);

class czip_explorer {
	czip& m_czip;
	std::vector<czip_entry> m_entries;

	inline int read_level(const czip_entry& entry) {
		int c = charcount('\\', entry.path, entry.header.path_length) + 1;
		return c;
	}
	int m_maxLevel = 0;

	void get_entries();
	void process_entries();
public:
	const int entries() const { return m_entries.size(); }
	inline const czip_entry& operator[] (int index) const { return m_entries[index]; }
	inline czip_explorer(czip& _czip) : m_czip(_czip) { get_entries(); }

	
	bool extract(int index, const char* outputPath);

	czip_explorer(const czip_explorer& other) = delete;
	czip_explorer(czip_explorer&& other) = delete;
	czip_explorer() = delete;
};
