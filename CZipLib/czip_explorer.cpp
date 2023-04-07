#include "czip_explorer.h"

void czip_explorer::process_entries() {
	std::vector<czip_entry*> new_entries;
	std::vector<czip_entry> dir_entries;
	//int max_level = 0;
	for (int c_level = 0; c_level <= m_maxLevel; c_level++) {
		for (auto& entry : m_entries) {
			if (entry.level == c_level) {
				new_entries.push_back(&entry);
			}
		}
	}
}
bool czip_explorer::extract(int index, const char* outputPath) {
	if (index >= m_entries.size()) return false;

	czip_entry& entry = m_entries[index];
	m_czip.current_file = index;
	m_czip._in.seekg(entry.data_pos);
	czip_file* file = m_czip.read_next();
	if (file->uncompress_file() != Z_OK)
		return false;

	std::ofstream output(outputPath, std::ios::out | std::ios::binary);
	write_raw_file(file, &output);
	return true;
}


void czip_explorer::get_entries() {
	m_entries.reserve(m_czip.total_files());
	m_czip._in.seekg(sizeof czip_header);

	for (int i = 0; i < m_czip.total_files(); i++) {

		m_entries.emplace_back(); // create new entry
		czip_entry& entry = m_entries[m_entries.size() - 1]; // get reference to it
		entry.data_pos = m_czip._in.tellg();
		m_czip._in.read((char*)&entry.header, sizeof czip_file_header); // read its header
		uLong cPos = static_cast<uLong>(m_czip._in.tellg());

		entry.type = CZIP_File;


		if (entry.header.name_length != 0) {
			entry.file = new char[entry.header.name_length + 1];
			entry.file[entry.header.name_length] = 0;
			m_czip._in.read(entry.file, entry.header.name_length);
		}
		if (entry.header.path_length != 0) {
			entry.path = new char[entry.header.path_length + 1];
			entry.path[entry.header.path_length] = 0;
			m_czip._in.read(entry.path, entry.header.path_length);
		}


		entry.level = entry.header.path_length == 0 ? 0 : charcount('\\', entry.path, entry.header.path_length) + 1;

		if (m_maxLevel < entry.level)
			m_maxLevel = entry.level;

		m_czip._in.seekg(cPos
						 + entry.header.compressed_data_size
						 + entry.header.name_length
						 + entry.header.path_length, m_czip._in.beg); // tell file to skip over content of the file

	}

}

int charcount(const char c, const char* str, size_t str_size) {
	int count = 0;
	for (size_t i = 0; i < str_size; i++) {
		if (str[i] == c) ++count;
	}
	return count;
}
