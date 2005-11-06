#include "mlc.hh"


// generic equipment for dynamic library access


namespace dyn
{

  const std::string mangle_name(const std::string& cpp_name)
  {
    std::string name_;
    for (unsigned i = 0; i < cpp_name.size(); ++i)
      {
	if (cpp_name[i] == ':' and cpp_name[i+1] == ':') {
	  name_ += "_N_";
	  ++i;
	}
	else if (cpp_name[i] == '<') name_ += "_L_";
	else if (cpp_name[i] == ',') name_ += "_C_";
	else if (cpp_name[i] == '>') name_ += "_R_";
	else name_ += cpp_name[i];
      }
    return name_;
  }

  template <class T>
  const std::string mangle_arg(const T& arg)
  {
    return std::string("_A_") + mangle_name(mlc_name<T>::of());
  }

  const std::string demangle(const std::string& name)
  {
    std::string cpp_name_;
    unsigned i;
    for (i = 0; i < name.size() - 2; ++i)
      {
	if (name[i] == '_' and name[i+2] == '_') {
	    if (name[i+1] == 'N') cpp_name_ += "::";
	    else if (name[i+1] == 'L') cpp_name_ += " < ";
	    else if (name[i+1] == 'C') cpp_name_ += ',';
	    else if (name[i+1] == 'R') cpp_name_ += " > ";
	    i += 2;
	  }
	else cpp_name_ += name[i];
      }
    if (i == name.size() - 2) {
      cpp_name_ += name[i];
      cpp_name_ += name[i+1];
    }
    return cpp_name_;
  }

  const std::string demangle_proc(const std::string& name)
  {
    if (name.size() <= 3)
      return name;
    unsigned i_last;
    for (unsigned i = 0; i < name.size() - 3; ++i)
      if (name[i] == '_' and name[i+1] == 'A' and name[i+2] == '_') {
	i_last = i;
	break;
      }
    std::string cpp_name_;
    std::copy(name.begin(), name.begin() + i_last, 
	      back_inserter(cpp_name_));
    return demangle(cpp_name_);
  }

  const std::string demangle_arg(const std::string& name, unsigned n)
  {
    unsigned count = 0, i_first, i_last = name.size();
    for (unsigned i = 0; i < name.size() - 3; ++i)
      {
	if (name[i] == '_' and name[i+1] == 'A' and name[i+2] == '_')
	  {
	    ++count;
	    if (count == n) i_first = i + 3;
	    if (count == n+1) { i_last = i; break; }
	  }
      }
    std::string cpp_name_;
    std::copy(name.begin() + i_first, name.begin() + i_last, 
	      back_inserter(cpp_name_));
    return demangle(cpp_name_);
  }

}
