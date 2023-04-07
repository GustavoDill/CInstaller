#pragma once

#include <vector>
#include <string>


#define PARSE_ARGS(argc, argv) args = Args(argc, argv)
#define GET_ARGS(argc, argv) PARSE_ARGS(argc, argv)

class Args {
	using string = std::string;
	using string_vector = std::vector<string>;
	char** m_argv;
	string_vector args;

public:
	int argc;
	Args();
	Args(int argc, char** argv);
	bool contains(const char* name);
	string& get(const char* name) noexcept;
	string& operator[](int index);
	void arg_not_found(const char* arg_name, int exitCode);
	void require(const char* name);

	operator size_t() const;
};

extern Args args;