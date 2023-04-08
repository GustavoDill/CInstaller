#include "czip_file.h"

const char* czip_file::name() const { return file_name; }

const char* czip_file::path() const { return path_name; }

int czip_file::compress_file() {
	if (raw_data == nullptr) return Z_BUF_ERROR;
	if (compressed_data != nullptr) delete[] compressed_data;

	compressed_data = new byte[header.raw_data_size];
	header.compressed_data_size = header.raw_data_size;
	return compress(compressed_data, &this->header.compressed_data_size, raw_data, header.raw_data_size);
}

int czip_file::uncompress_file() {
	if (compressed_data == nullptr) return Z_BUF_ERROR;
	if (raw_data != nullptr) delete[] raw_data;

	raw_data = new byte[header.raw_data_size];
	return uncompress(raw_data, &header.raw_data_size, compressed_data, header.compressed_data_size);
}

void czip_file::set_name(const std::string& name) {
	file_name = new char[name.size() + 1];
	memcpy(file_name, name.data(), name.size() + 1);
	header.name_length = name.size();
}

void czip_file::set_path(const std::string& path) {
	path_name = new char[path.size() + 1];
	memcpy(path_name, path.data(), path.size() + 1);
	header.path_length = path.size();
}

void czip_file::free() {
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

czip_file::~czip_file() {
	free();
}


void read_compressed_file(czip_file* _file, std::ifstream* _in) {
	_in->read((char*)&_file->header, sizeof(_file->header));// Read headers
	// if has name
	if (_file->header.name_length > 0) {
		_file->file_name = new char[_file->header.name_length + 1]; // Allocate space for name
		_file->file_name[_file->header.name_length] = 0; // Set ending byte for c_str
		_in->read(_file->file_name, _file->header.name_length); // Read file name
	}
	if (_file->header.path_length > 0) {
		_file->path_name = new char[_file->header.path_length + 1]; // Allocate space for path
		_file->path_name[_file->header.path_length] = 0; // Set ending byte for c_str
		_in->read(_file->path_name, _file->header.path_length); // Read file path
	}
	_file->compressed_data = new byte[_file->header.compressed_data_size]; // Allocate memory specified in the header
	_in->read((char*)_file->compressed_data, _file->header.compressed_data_size); // Read compressed data
}

void read_raw_file(czip_file* _file, std::ifstream* _in) {
	uLong p = _in->tellg();
	_in->seekg(0, std::ios::end);
	_file->header.raw_data_size = _in->tellg();
	_in->seekg(p);
	_file->raw_data = new byte[_file->header.raw_data_size];
	_in->read((char*)(_file->raw_data), _file->header.raw_data_size);
}

void write_compressed_file(czip_file* _file, std::ofstream* _out) {
	size_t size_of_header = sizeof _file->header;
	int data_pos = _out->tellp();
	_out->write((char*)&(_file->header), size_of_header); // Write header
	/*_out->close();
	return;*/
	if (_file->header.name_length > 0) // If has name
		_out->write((char*)_file->file_name, _file->header.name_length); // Write name
	if (_file->header.path_length > 0)
		_out->write((char*)_file->path_name, _file->header.path_length);
	_out->write((char*)_file->compressed_data, _file->header.compressed_data_size); // Write compressed data
}

void write_raw_file(czip_file* _file, std::ofstream* _out) {
	_out->write((char*)(_file->raw_data), _file->header.raw_data_size);
}