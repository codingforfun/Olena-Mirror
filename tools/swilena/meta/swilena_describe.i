%include std_string.i
%{
#include <sstream>
%}

%define EXTEND_DESCRIBE2(T1, T2)
%extend
{
  std::string describe() const
    {
      std::ostringstream os;
      os << # T1 "," # T2 "@" << self << " = " << (*self);
      return os.str();
    }

#if defined(SWIGPYTHON)
  std::string __str__() {
    std::ostringstream os;
    os << (*self);
    return os.str();
  }
#endif

#if defined(SWIGRUBY)
  std::string to_s() {
    std::ostringstream os;
    os << (*self);
    return os.str();
  }
#endif
}
%enddef
%define EXTEND_DESCRIBE(T)
%extend
{
  std::string describe() const
    {
      std::ostringstream os;
      os << # T "@" << self << " = " << (*self);
      return os.str();
    }

#if defined(SWIGPYTHON)
  std::string __str__() {
    std::ostringstream os;
    os << (*self);
    return os.str();
  }
#endif

#if defined(SWIGRUBY)
  std::string to_s() {
    std::ostringstream os;
    os << (*self);
    return os.str();
  }
#endif
}
%enddef
