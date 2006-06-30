// -*- C++ -*-

%include swilena_config.i
%include swilena_basics.i

%include swilena_describe.i
%include swilena_core.i

%define decl_conversions(Dim)

%include swilena_basics ## Dim ## d.i

%{
#include <oln/convert/basics.hh>
%}

%enddef
