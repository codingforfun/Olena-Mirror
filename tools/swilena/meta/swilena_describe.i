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
}
%enddef

