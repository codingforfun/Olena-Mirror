#include "mangle.hh"

#include <sstream>
#include <cassert>

namespace dyn {

  // call routines
  // ---------------------------------------------

  // data-like one

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
