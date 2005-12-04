#ifndef DYN_FUNCTION_LOADER_HH
# define DYN_FUNCTION_LOADER_HH

# include <string>
# include <list>

namespace dyn {

  void
  include(const std::string& file);

  void
  post_include(const std::string& file);

  void
  include_dir(const std::string& dir);

  void cflags(const std::string& elt);

  void ldflags(const std::string& elt);

  typedef std::list<std::string> arguments_types_t;

  void*
  load_function(const std::string& kind,
                const std::string& name,
                const arguments_types_t& arguments_types,
                const std::string& header_path,
                const std::string& options);

} // end of namespace dyn

#endif
