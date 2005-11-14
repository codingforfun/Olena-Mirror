#ifndef DYN_FUNCTION_LOADER_HH
#define DYN_FUNCTION_LOADER_HH

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <ruby.h>
#include "dl.h"
#include "ruby_stream.hh"
#include "config.hh"
#include "data.hh"
#include "mlc.hh"

namespace dyn {

  struct function_loader_t
  {

    function_loader_t()
    {
      ruby_init();
      ruby_script("function_loader.hh");
      ruby_init_loadpath();
      ruby << "require \""
           << SOURCE_DIR
           << "/function_loader.rb"
           << "\"" << ruby::eval;
    }

    void
    include(const std::string& file)
    {
      ruby << "FunctionLoader.include \"" << file << "\"" << ruby::eval;
    }

    void
    include_dir(const std::string& dir)
    {
      ruby << "FunctionLoader.include_dir \"" << dir << "\"" << ruby::eval;
    }

    void*
    load(const std::string& kind,
         const std::string& name,
         const std::string& arguments_types,
         const std::string& header_path)
    {
      ruby << "FunctionLoader.call " << kind << ", \""
           << name << "\", [\"" <<  arguments_types << "\"], \""
           << header_path << "\"" << ruby::eval;
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
  include_dir(const std::string& dir)
  {
    function_loader.include_dir(dir);
  }

  void*
  load_function(const std::string& kind,
                const std::string& name,
                const std::string& arguments_types,
                const std::string& header_path)
  {
    return function_loader.load(kind, name, arguments_types, header_path);
  }

} // end of namespace dyn

#endif
