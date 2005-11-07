#include <string>
#include <iostream>
#include <sstream>
#include <ruby.h>
#include "dl.h"
#include "ruby_stream.hh"
#include "config.hh"

/*#define dyn_include(file) \
\#include file \
function_loader.include(file);
*/


struct function_loader_t
{
  typedef void (*fun_t)();

  function_loader_t()
  {
    std::cout << "Initializing ruby..." << std::endl;
    ruby_init();
    ruby_script("function_loader.hh");
    ruby_init_loadpath();
    std::cout << "Loading function_loader.rb..." << std::endl;
    ruby << "require \""
         << SOURCE_DIR
         << "/function_loader.rb"
         << "\"" << ruby::eval;
  }

  fun_t
  load(const std::string& header_path, const std::string& fun_name)
  {
    std::cout << "Loading the function " << fun_name << " in " << header_path << std::endl;
    ruby << "FunctionLoader.from_mlc_name_of \""
         << header_path << "\", \"" << fun_name << "\"" << ruby::eval;
    return (fun_t)(RDLPTR(ruby.last_value())->ptr);
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

  ruby::stream ruby;
};

function_loader_t function_loader;
