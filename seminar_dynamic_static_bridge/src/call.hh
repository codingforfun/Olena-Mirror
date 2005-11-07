#include "mangle.hh"

#include <sstream>
#include <fstream>
#include <cassert>

namespace dyn {

  // loader

  struct
  {
    static std::string& repository()
    {
      static std::string name = "/tmp/";
      return name;
    }

    void* operator()(const std::string& proc, unsigned argc)
    {
      map_t::iterator i = map_.find(proc);
      if (i == map_.end())
	{
	  std::cout << "compiling \"" << proc << "\"... " << std::endl;
 	  generate(proc, argc);
	  compile(proc);
          // std::cout << "not yet impled!" << std::endl;
	  // FIXME: then load in map!!!
	  return 0;
	}
      return i->second;
    }

    void generate(const std::string& proc, unsigned argc)
    {
      std::string filename = repository() + proc + ".cc";
      std::ofstream file(filename.c_str());
      file << "#include \"dyn.hh\"" << std::endl;
      file << "#include \"lib.hh\"" << std::endl; // FIXME: not generic
      file << "namespace generated" << std::endl << "{" << std::endl;
      file << "  void " << proc << "(";
      for (unsigned i = 1; i <= argc; ++i)
	file << "const dyn::data& arg" << i << (i != argc ? ", " : ")");
      file << std::endl << "  {" << std::endl;
      for (unsigned i = 1; i <= argc; ++i) {
	std::string type = demangle_arg(proc, i);
	file << "    const dyn::data_proxy< " << type << " >* arg" << i
	     << "_ = dynamic_cast<const dyn::data_proxy< " << type
	     << " >*>(arg" << i << ".proxy_);" << std::endl;
      }
      file << "    arg" << argc << "_->assign( " << demangle_proc(proc) << "(";
      for (unsigned i = 1; i < argc; ++i)
	file << "*(arg" << i << "_->p_obj_)" << (i != argc-1 ? ", " : ") );");
      file << std::endl;
      file << "  }" << std::endl;
      file << "}" << std::endl;
      file.close();
    }

    void compile(const std::string& proc)
    {
      const std::string filename = repository() + proc + ".cc";
      const std::string gxx = "g++";
      const std::string cmd = gxx + " -I . -shared " + filename;
      std::cout << cmd << std::endl;
      int compile_status = system(cmd.c_str());
      if (compile_status)
      {
        std::cout << "compile ko" << std::endl;
        assert(compile_status == 0);
      }
      else
        std::cout << "compile ok" << std::endl;
      // FIXME: not yet impled
    }

    typedef std::map<std::string, void*> map_t;
    map_t map_;
  }
    proc_loader;



  // call routines
  // ---------------------------------------------

  // data-like one

  void call(const std::string& name,
	    const data& arg, const data& ret)
  {
    std::ostringstream name_;
    name_ << mangle_name(name)
	  << arg.mangle()
	  << ret.mangle();
    typedef void (*proc_type_)(const data&, const data&);
    void* p_ = proc_loader(name_.str(), 2);
    if (p_ == 0) return;
    proc_type_ proc_ = proc_type_(p_);
    (*proc_)(arg, ret); // ...
  }

  // generic one

  template <class T, class Tr>
  void call_gen(const std::string& name,
		const T& arg, const Tr& ret)
  {
    data d = arg;
    data r = ret;
    call(name, d, r);
    // std::ostringstream name_;
    // name_ << mangle_name(name)
          // << mangle_arg(arg)
          // << mangle_arg(ret);
    // typedef void (*proc_type_)(const data&, const data&);
    // void* p_ = proc_loader(name_.str(), 2);
    // if (p_ == 0) return;
    // proc_type_ proc_ = proc_type_(p_);
    // (*proc_)(arg, ret); // ...
  }

  // string-like one

  void call_str(const std::string& name,
		const std::string& arg, const std::string& ret)
  {
    data arg_ = dyn::dat[arg];
    if (arg_.proxy_ == 0) {
      std::cerr << "object \"" << arg << "\" not found: call aborted!" << std::cerr;
      return;
    }
    data ret_ = dyn::dat[ret];
    if (ret_.proxy_ == 0) {
      std::cerr << "object \"" << ret << "\" not found: call aborted!" << std::cerr;
      return;
    }
    call(name, arg_, ret_);
  }

  void call(const char* name,
	    const char* arg, const char* ret)
    // facade that disambiguate with call_gen
  {
    return call_str(name, arg, ret);
  }


  // fake code (just for testing purpose)!

  data call_12()
  {
    int* i = new int;
    *i = 12;
    data tmp(*i);
    return tmp;
  }


  // when 2 args:

  template <class T1, class T2, class Tr>
  void call_gen(const std::string& name,
		const T1& arg1, const T2& arg2, const Tr& ret)
  {
    std::ostringstream name_;
    name_ << mangle_name(name)
	  << mangle_arg(arg1)
	  << mangle_arg(arg2)
	  << mangle_arg(ret);
    typedef void (*proc_type_)(const data&, const data&, const data&);
    void* p_ = proc_loader(name_.str(), 3);
    if (p_ == 0) return;
    proc_type_ proc_ = proc_type_(p_);
    (*proc_)(arg1, arg2, ret); // ...
  }



std::ostream& operator<<(std::ostream& ostr, const dyn::data& d)
{
  d.print(ostr);
  return ostr;
}


} // end of dyn
