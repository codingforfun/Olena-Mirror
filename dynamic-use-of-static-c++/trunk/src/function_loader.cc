#ifndef DYN_FUNCTION_LOADER_CC
# define DYN_FUNCTION_LOADER_CC


// FIXME
# include <iostream>

# include <ltdl.h>
# include <map>

# include "data.hh"
# include "function_loader.hh"
# include "ruby_stream.hh"

namespace dyn {

  ruby::environment ruby_environment;

  struct ltstr
  {
    bool operator()(const char* s1, const char* s2) const
    {
      return strcmp(s1, s2) < 0;
    }
  };

  struct function_loader_t
  {

    function_loader_t()
    {
      lt_dlinit();
      ruby << "$: << \"" << DYNDIR << "\"" << ruby::eval;
      ruby << "require 'function_loader'" << ruby::eval;
      ruby << "require 'md5'" << ruby::eval;
    }

    ~function_loader_t()
    {
      lt_dlexit();
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
         const arguments_types_t& arguments_types,
         const std::string& header_path,
         const std::string& options)
    {
      std::ostringstream ostr;
      ostr << name << '(';
      arguments_types_t::const_iterator it(arguments_types.begin());
      if (it != arguments_types.end())
      {
        ostr << *it;
        for (++it; it != arguments_types.end(); ++it)
          ostr << ',' << *it;
      }
      ostr << ')';
      if (header_path != "") ostr << ", path: " << header_path;
      if (options != "") ostr << ", options: " << options;
      std::string prototype = ostr.str();

      std::cerr << prototype << std::endl;
      ruby << "Digest::MD5.new(\"" << prototype.c_str() << "\").to_s" << ruby::eval;
      const char* identifier = STR2CSTR(ruby.last_value());

      cache_type::iterator ptr_it = cache.find(identifier);

      if ( ptr_it != cache.end() )
      {
        std::cerr << "\e[36mJIT: \e[32mHIT: \e[0m " << prototype << std::endl;
        return ptr_it->second;
      }

      std::cerr << "\e[36mJIT: \e[31mMISS: compile: \e[0m " << prototype << std::endl;

      ruby << "FunctionLoader.call \"" << identifier << "\", " << kind << ", \""
           << name << "\", [";
      it = arguments_types.begin();
      if (it != arguments_types.end())
      {
        ruby << '"' << *it << '"';
        for (++it; it != arguments_types.end(); ++it)
          ruby << ',' << '"' << *it << '"';
      }
      ruby << "], \"" << header_path << "\"" << ", \"" << options << "\"" << ruby::eval;

      const char* error;
      std::string lib_path = std::string("repository/") + identifier
                             + "/libdyn_" + identifier + ".la";
      std::string symb = std::string("dyn_") + identifier;

      lt_dlhandle lib = lt_dlopen(lib_path.c_str());
      if ((error = lt_dlerror())) std::cerr << error << std::endl;
      void* ptr = lt_dlsym(lib, symb.c_str());
      cache[identifier] = ptr;
      if ((error = lt_dlerror())) std::cerr << error << std::endl;
      return ptr;
    }

    typedef std::map<const char*, void*, ltstr> cache_type;
    cache_type cache;
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
                const arguments_types_t& arguments_types,
                const std::string& header_path,
                const std::string& options)
  {
    return function_loader.load(kind, name, arguments_types, header_path, options);
  }

} // end of namespace dyn

#endif
