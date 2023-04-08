#include "czip.h"

void czip::set_mode(MODE mode) { m_mode = mode; }
bool czip::add_dir(const std::string& root, const std::string& relpath) {
	if (m_mode != MODE_COMPRESS) return false;
	// Yes this whole method is inefficient

	if (std::filesystem::exists(root) == false) return false;
	std::filesystem::directory_iterator dir;

	if (relpath.size() != 0)
		dir = std::filesystem::directory_iterator(root + "\\" + relpath);
	else
		dir = std::filesystem::directory_iterator(root);
	for (auto& entry : dir) {

		if (entry.is_regular_file()) {
			std::string __path = entry.path().string();
			add_file(__path.c_str(), entry.path().filename().string(), relpath);
		}
		else {
			add_dir(root, __zip_get_rel_dir(root, entry.path().string()));
		}

	}
	return true;
}

czip::czip(MODE mode) : m_mode (mode) { }

void czip::open(const char* path) {
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

int czip::extract_dir(const std::string& output_path) {
	if (std::filesystem::exists(output_path) == false)
		std::filesystem::create_directories(output_path);
	std::ofstream out_file;
	std::filesystem::path _Poutput_path(output_path);
	while (current_file != m_header.total_files) {
		if (int res = current.uncompress_file() != Z_OK) // Decompress
			return res; // Do some sort of error handling later :)

		if (current.path() == nullptr) // Not have path
		{
			out_file.open(_Poutput_path / current.name(), std::ios::out | std::ios::binary); // Open output
		}
		else {
			std::filesystem::path p = _Poutput_path / current.path();
			if (std::filesystem::exists(p) == false)
				std::filesystem::create_directories(p);
			out_file.open(p / current.name(), std::ios::out | std::ios::binary);

		}
		write_raw_file(&current, &out_file); // Write decompressed data
		out_file.close(); // Close output
		read_next(); // Get next file
	}

	return Z_OK;
}

czip_file* czip::read_next() {
#if _DEBUG
	if (m_mode != MODE_DECOMPRESS) return nullptr; // In release this should not happen...
#endif
	if (current_file >= m_header.total_files)
		return nullptr;
	current_file++;

	current.free();
	read_compressed_file(&current, &_in);

	return &current;
}
void czip::add_file(const char* filepath, const std::string& data_name, const std::string& data_path) {
#if _DEBUG
	if (m_mode != MODE_COMPRESS) return; // In release this should not happen...
#endif

	m_header.total_files++;
	current.free(); // Free any resources left

	_in.open(filepath, std::ios::binary | std::ios::in); // Open file
	read_raw_file(&current, &_in); // Read data
	m_header.total_raw_size += current.header.raw_data_size;
	if (data_name.size() > 0)
		current.set_name(data_name);
	if (data_path.size() > 0)
		current.set_path(data_path);
	current.compress_file(); // Compress

	write_compressed_file(&current, &_out); // Append output

	_in.close(); // Close file
}

czip_file* czip::get_file(const char* path, const char* name) {
	int16_t p_len = strlen(path);
	int16_t n_len = strlen(name);

	if (p_len == current.header.path_length &&
		n_len == current.header.name_length) // If path and name length ==
		if (strcmp(name, current.name())) // if name ==
			if (strcmp(path, current.path())) // If path ==
				return &current; // Return current

	_in.seekg(0);
	do {
		current.free();
		read_compressed_file(&current, &_in);
		if (n_len == current.header.name_length &&
			p_len == current.header.path_length)
			if (strcmp(name, current.name()))
				if (strcmp(path, current.path()))
					return &current;

	} while (current_file < total_files());
	return nullptr;
}
czip_file* czip::operator[](int file_index) {
#if _DEBUG
	if (m_mode != MODE_DECOMPRESS) return nullptr; // In release this should not happen...
#endif
	if (file_index >= total_files()) return nullptr;
	if (current_file == file_index) return &current;

	_in.seekg(0);
	current_file = 0;
	czip_file_header __tmp_header;
	do {
		if (current_file == file_index) {
			read_compressed_file(&current, &_in); // If its this file then read the data and return
			return &current;
		}
		_in.read((char*)&__tmp_header, sizeof __tmp_header); // read header
		uLong c_pos = _in.tellg(); // Get current position
		uLong newPos = c_pos + __tmp_header.name_length + __tmp_header.path_length + __tmp_header.compressed_data_size; // Add file size to c_pos
		_in.seekg(newPos, _in.beg); // Goto new position (skipped file)
		current_file++;
	} while (current_file < file_index);

	return &current;
}

czip::~czip() {
	write_header();
	close();
}

void czip::close() {
	switch (m_mode) {
		case MODE_COMPRESS: _out.close(); break;
		case MODE_DECOMPRESS: _in.close(); break;
	}
}

czip_file* czip::read_first() {
	_in.seekg(sizeof m_header); // Skip header
	current_file = -1;
	return read_next(); // Read first file
}

constexpr int czip::total_files() const { return m_header.total_files; }

czip::czip(const char* path, MODE mode) : m_mode(mode) {
	open(path);
}

void czip::write_header() {
	_out.seekp(0);
	_out.write((char*)(&m_header), sizeof m_header);
}

void czip::read_header() {
	_in.read((char*)&m_header, sizeof m_header);
}

std::string __zip_get_rel_dir(const std::string& root, const std::string& current) {
	return std::filesystem::path(current).lexically_relative(root).string();
}
