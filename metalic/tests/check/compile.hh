#ifndef OLENA__TESTS__CHECK__COMPILE__HH
# define OLENA__TESTS__CHECK__COMPILE__HH

# include <string>

std::string compile_cmd(const std::string& src,
		        const std::string& dst = "",
			const std::string& extra_flags = "");
const char* cxxflags_optimize();
const char* cxxflags_strict();
const char* cxxflags_strict_errors();


#endif
