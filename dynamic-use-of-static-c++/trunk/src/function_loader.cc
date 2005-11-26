#ifndef DYN_FUNCTION_LOADER_CC
# define DYN_FUNCTION_LOADER_CC

# include "function_loader.hh"
# include "ruby_stream.hh"

namespace dyn {

  ruby::environment ruby_environment;

  struct function_loader_t
  {

    function_loader_t()
    {
      ruby << "$: << \"" << DYNDIR << "\"" << ruby::eval;
      ruby << "require 'function_loader'" << ruby::eval;
    }

    void
    include(const std::string& file)
    {
      ruby << "FunctionLoader.include \"" << file << "\"" << ruby::eval;
    }

    void
    post_include(const std::string& file)
    {
      ruby << "FunctionLoader.post_include \"" << file << "\"" << ruby::eval;
    }

    void
    include_dir(const std::string& dir)
    {
      ruby << "FunctionLoader.include_dir \"" << dir << "\"" << ruby::eval;
    }

    void
    cflags(const std::string& elt)
    {
      ruby << "FunctionLoader.cflags \"" << elt << "\"" << ruby::eval;
    }

    void
    ldflags(const std::string& elt)
    {
      ruby << "FunctionLoader.ldflags \"" << elt << "\"" << ruby::eval;
    }

    void*
    load(const std::string& kind,
         const std::string& name,
         const std::string& arguments_types,
         const std::string& header_path,
         const std::string& options)
    {
      ruby << "FunctionLoader.call " << kind << ", \""
           << name << "\", [\"" <<  arguments_types << "\"], \""
           << header_path << "\"" << ", \"" << options << "\"" << ruby::eval;
      return RDLPTR(ruby.last_value())->ptr;
    }

    ruby::stream ruby;
  };

  function_loader_t function_loader;

  void
  include(const std::string& file)
  {
    function_loader.include(file);
  }

  void
  post_include(const std::string& file)
  {
    function_loader.post_include(file);
  }

  void
  include_dir(const std::string& dir)
  {
    function_loader.include_dir(dir);
  }

  void cflags(const std::string& elt)
  {
    function_loader.cflags(elt);
  }

  void ldflags(const std::string& elt)
  {
    function_loader.ldflags(elt);
  }

  void*
  load_function(const std::string& kind,
                const std::string& name,
                const std::string& arguments_types,
                const std::string& header_path,
                const std::string& options)
  {
    return function_loader.load(kind, name, arguments_types, header_path, options);
  }

} // end of namespace dyn

#endif
