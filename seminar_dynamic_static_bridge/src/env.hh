#include "data.hh"
// #include <cassert>
// #include <cmath>

#include <iostream>
// #include <string>
// #include <sstream>
// #include <fstream>

// #include <vector>
#include <map>

// #include "mlc.hh"

namespace dyn {

  // dat

/*  typedef std::map<std::string, data> dat_t;
  dat_t dat;
*/
  // misc

/*  bool data::set_name(const std::string& name)
  {
    dat_t::iterator d = dat.find(name);
    if (d != dat.end()) {
      std::cerr << "object name \"" << name << "\" already exist" << std::endl;
      return false;
    }
    dat[name] = *this;
    return true;
  }
*/
/*  template <class T>
  void mk_data(const T& obj, const std::string& name)
  {
    data* d = new data(obj);
    bool ok = d->set_name(name);
    if (not ok) {
      delete d;
      std::cerr << "call to dyn::mk_data aborted!" << std::endl;
    }
  }
*/
/*  void rm_data(const std::string& name)
  {
    dat_t::iterator d = dat.find(name);
    if (d == dat.end()) {
      std::cerr << "object with name \"" << name
		<< "\" does not exist;"
		<< " call to dyn::rm_data aborted!" << std::endl;
      return;
    }
    dat.erase(name);
  }
*/
}

