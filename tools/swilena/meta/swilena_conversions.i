// -*- C++ -*-

%include swilena_describe.i
%include swilena_core.i

%define decl_conversions(Dim)

%{
#include <oln/convert/basics.hh>
%}

// FIXME: not very elegant, but %{ %} blocks are not preprocessed.

#if Dim == 1
%{
#include <oln/basics1d.hh>
%}
#elif Dim == 2
%{
#include <oln/basics2d.hh>
%}  
#elif Dim == 3
%{
#include <oln/basics3d.hh>
%}
#endif

%enddef
