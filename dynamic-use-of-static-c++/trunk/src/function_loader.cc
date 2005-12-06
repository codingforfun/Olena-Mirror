#ifndef DYN_FUNCTION_LOADER_CC
# define DYN_FUNCTION_LOADER_CC

# include <ltdl.h>
# include <map>

// FIXME use and improve the logger to avoid use of std::cerr
# include <iostream>

# include "data.hh"
# include "function_loader.hh"
# include "ruby_stream.hh"


template <typename InputIterator, typename T, typename OStream>
OStream& join(const InputIterator& begin, const InputIterator& end, const T& elt, OStream& ostr)
{
  InputIterator it = begin;

  if (it != end)
    ostr << *it;

  for (++it; it != end; ++it)
    ostr << elt << *it;

  return ostr;
}


namespace dyn {

  ruby::environment ruby_environment;

  struct ltstr
  {
    bool operator()(const char* s1, const char* s2) const
    {
      return strcmp(s1, s2) < 0;
    }
  };

  std::list<std::string> includes_;

  template <typename Fun>
  void
  foreach_path_in_paths(const std::string& header_paths, Fun& fun)
  {
    std::list<std::string>::const_iterator it;
    unsigned last = 0, pos;
    while (42)
    {
      pos = header_paths.find(":", last);
      std::string sub = header_paths.substr(last, pos - last);
      if (sub == "*")
      {
        for (it = includes_.begin(); it != includes_.end(); ++it)
          fun(*it);
      }
      else if (sub != "")
        fun(sub);
      if (pos >= std::string::npos) break;
      last = pos + 1;
    }
  }

  template <typename OStream>
  struct gen_cxx_path
  {
    gen_cxx_path(OStream& ostr_) : ostr(ostr_) {}
    void operator() (const std::string& path)
    {
      ostr << "#include ";
      if (path.find("<") == std::string::npos || path.find("\"") == std::string::npos)
        if (path.find(".hh") == path.length() - 3)
          ostr << "\"" << path << "\"";
        else
          ostr << "<" << path << ">";
      else
        ostr << path;
      ostr << '\n';
    }
    OStream& ostr;
  };

  template <typename OStream>
  struct gen_path
  {
    gen_path(OStream& ostr_) : first(true), ostr(ostr_) {}
    void operator() (const std::string& path)
    {
      if (first)
      {
        first = false;
        ostr << path;
      }
      else
        ostr << ':' << path;
    }
    bool first;
    OStream& ostr;
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

    template <typename OStream>
    void
    gen_cxx(const std::string& identifier,
            const std::string& name,
            const std::list<std::string>& args,
            fun_kind kind,
            const std::string paths,
            const std::string options,
            OStream& ostr)
    {
      ruby::stream r;
      typedef std::string        str;
      typedef std::list<str>     str_list;
      str_list call_args;
      std::ostringstream body, call;
      str nl("\n        ");
      bool first_type_is_ptr = false;
      str_list::const_iterator it;

      ostr << "#include \"dyn-light.hh\"\n";

      gen_cxx_path<OStream> fun(ostr);
      foreach_path_in_paths(paths, fun);

      ostr << "extern \"C\" {\n"
           << "  namespace dyn {\n"
           << "    namespace generated {\n"
           << "      data\n"
           << "      dyn_" << identifier << "(";

      int i = 0;
      for (it = args.begin(); it != args.end(); ++it, ++i)
      {
        std::ostringstream oarg;
        oarg << "arg" << i;
        str arg(oarg.str());

        // remove references cause they are forbidden on lhs
        r << '"' << *it << "\".gsub(/&*$/, '')" << ruby::eval;
        str type(STR2CSTR(r.last_value()));
        r << "!! (\"" << type << "\" =~ /\\*\\s*(const)?\\s*>\\s*$/)" << ruby::eval;
        first_type_is_ptr = r.last_value() == Qtrue;
        if (it != args.begin()) ostr << ", ";
        ostr << "const data& " << arg;
        call_args.push_back(arg + "_reinterpret_cast_ptr->obj()");
        body << type << "* " << arg << "_reinterpret_cast_ptr = "
             << "reinterpret_cast<" << type << "* >(" << arg << ".proxy());"
             << nl << "assert(" << arg << "_reinterpret_cast_ptr);" << nl;
      }

      if ( options.find("method") != str::npos )
      {        
        call << call_args.front() << ((first_type_is_ptr)? "->" : ".");
        call_args.pop_front();
      }

      call << name << "(";
      join(call_args.begin(), call_args.end(), ", ", call);
      call << ")";

      str op(name);

      switch (kind)
      {
        case OP:
          if ( op.compare(0, 8, "operator") == 0 )
            op.erase(0, 8);
          call.str(std::string());
          switch (call_args.size())
          {
            case 1:
              call << op << "(" << *call_args.begin() << ")";
              break;
            case 2:
              it = call_args.begin();
              call << "(" << *it++ << ") ";
              call << op
                   << " (" << *it << ")";
              break;
            default: assert(0);
          }
          // no break here
        case FUN:
          body << "policy::receiver<select_dyn_policy((" << call.str() << "))> receiver;" << nl
               << "(receiver(), " << call.str() << ");" << nl
               << "data ret(receiver.proxy(), (proxy_tag*)0);" << nl
               << "return ret;\n";
          break;
        case CTOR:
          body << "typedef " << name << " T;" << nl
               << "T* ptr = new T(";
          join(call_args.begin(), call_args.end(), ", ", body);
          body << ");" << nl
               << "abstract_data* proxy = new data_proxy_by_ptr<T>(ptr);" << nl
               << "data ret(proxy, (proxy_tag*)0);" << nl
               << "return ret;\n";
          break;
        default:
          assert(!"Unknown kind");
      }

      ostr << ")\n"
           << "      {" << nl
           << body.str()
           << "      }\n"
           << "    }\n"
           << "  }\n"
           << "};\n";

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

#if 0

    void
    include_dir(const std::string& dir)
    {
      include_dirs_.push_back(dir);
    }

    void
    cflags(const std::string& elt)
    {
      cflags_.push_back(elt);
    }

    void
    ldflags(const std::string& elt)
    {
      ldflags_.push_back(elt);
    }

#endif

    void*
    load(fun_kind kind,
         const std::string& name,
         const arguments_types_t& arguments_types,
         const std::string& options,
         const std::string& paths)
    {
      std::ostringstream ostr;
      ostr << name << '(';
      arguments_types_t::const_iterator it(arguments_types.begin());
      if (it != arguments_types.end())
      {
        ostr << *it;
        for (++it; it != arguments_types.end(); ++it)
          ostr << ", " << *it;
      }
      ostr << ')';
      if (paths != "")
      {
        ostr << ", paths: ";
        gen_path<std::ostream> fun(ostr);
        foreach_path_in_paths(paths, fun);
      }
      if (options != "") ostr << ", options: " << options;
      std::string prototype = ostr.str();

      ruby << "Digest::MD5.new(%q{" << prototype.c_str() << "}).to_s" << ruby::eval;
      const char* identifier = STR2CSTR(ruby.last_value());

      cache_type::iterator ptr_it = cache.find(identifier);

      if ( ptr_it != cache.end() )
      {
        std::cerr << "\e[36mJIT: \e[32mHIT: \e[0m " << prototype << std::endl;
        return ptr_it->second;
      }

      std::cerr << "\e[36mJIT: \e[31mMISS: compile: \e[0m " << prototype << std::endl;

      ruby << "FunctionLoader.call %q{";
      gen_cxx(identifier, name, arguments_types, kind, paths, options, ruby);
      ruby << "}, \"" << identifier << "\", \""
           << name << "\"" << ruby::eval;

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

    protected:
    typedef std::map<const char*, void*, ltstr> cache_type;
    cache_type cache;
    std::list<std::string> cflags_, ldflags_;
    ruby::stream ruby;
  };

  function_loader_t function_loader;

  void
  include(const std::string& file)
  {
    includes_.push_back(file);
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
  load_function(fun_kind kind,
                const std::string& name,
                const arguments_types_t& arguments_types,
                const std::string& options,
                const std::string& header_path)
  {
    return function_loader.load(kind, name, arguments_types, options, header_path);
  }

} // end of namespace dyn

#endif
