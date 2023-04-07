#include "args.h"
Args::Args() : argc(0), m_argv(0) { }

Args::Args(int argc, char** argv) : argc(argc), m_argv(argv) {
	args.reserve(argc);
	for (int i = 0; i < argc; i++)
		args.push_back(argv[i]);
}

bool Args::contains(const char* name) {
	for (size_t i = 0; i < args.size(); i++) {
		if (args[i] == name) {
			return true;
		}
	}
	return false;
}

std::string& Args::get(const char* name) noexcept {
	for (size_t i = 0; i < args.size(); i++) {
		if (args[i] == name) {
			return args[i + 1];
		}
	}
	return args[-1];
}

std::string& Args::operator[](int index) { return args[index]; }

Args::operator size_t() const { return args.size(); }

void Args::arg_not_found(const char* arg_name, int exitCode) {
	printf("Argument %s was not found", arg_name);
	exit(exitCode);
}

void Args::require(const char* name) {
	if (!contains(name)) arg_not_found(name, EXIT_FAILURE);
}
