// -*- C++ -*-

%include exception.i

%{
#include <exception>
%}

// Olena uses classes that derive from std::exception. Declare it.
namespace std { class exception { public: const char *what() const; }; }

%define START_PROTECT
%exception {
  try {
    $action
      }
  catch(std::exception &e)
    {
      // FIXME: do we need to copy the string here ?
      SWIG_exception(SWIG_RuntimeError, const_cast<char*>(e.what()));
    }
  catch(...)
    {
      SWIG_exception(SWIG_UnknownError, __FUNCTION__);
    }
}
%enddef

%define END_PROTECT
%exception;
%enddef

START_PROTECT
