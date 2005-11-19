#ifndef DYN_FUNCTION_LOADER_HH
#define DYN_FUNCTION_LOADER_HH

// Protection against already loaded config.h
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef HAVE_DLFCN_H

#include <string>
#include <sstream>
#include <vector>
#include <ruby.h>
#include "dl.h"
#include "ruby_stream.hh"
#include "config.hh"
#include "data.hh"
#include "name_of.hh"

// Protection against futur config.h
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef HAVE_DLFCN_H

namespace dyn {

  struct function_loader_t
  {

    function_loader_t()
    {
      ruby_init();
      ruby_script("function_loader.hh");
      ruby_init_loadpath();
      ruby << "$: << \"" << SOURCE_DIR << "\"" << ruby::eval;
      ruby << "require 'function_loader'" << ruby::eval;
    }

    ~function_loader_t()
    {
      std::cout << "<~functionloader>" << std::endl;
      ruby_finalize();
      std::cout << "</~functionloader>" << std::endl;
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
    misc_compile_line(const std::string& elt)
    {
      ruby << "FunctionLoader.misc_compile_line \"" << elt << "\"" << ruby::eval;
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

  void misc_compile_line(const std::string& elt)
  {
    function_loader.misc_compile_line(elt);
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
