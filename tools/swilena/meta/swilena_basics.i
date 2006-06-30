// -*- C++ -*-

// Not to be used directly.  Use swilena_basics1d, swilena_basics2d,
// swilena_basics3d instead.

// Declare headers of basic types (image, point, etc.) for a given
// dimension.
%define decl_basics(Dim)

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
#else
#error Unsupported dimension.
#endif

%{
/* Provide a (dummy) implementation for the const copy constructor, to
   avoid linking errors (undefined symbols) when loading the shared
   library of the C++ wrapper.

   Beware, doing so breaks the Olena's `const' mechanism for images,
   but we don't really care: scripting languages like Python or Ruby
   don't have the concept of constness.

   See https://www.lrde.epita.fr/cgi-bin/twiki/view/Olena/ConstImages
   for more information.  */
namespace oln
{
  // Shallow copy.  This ctor is the same as the non const version,
  // except the ugly `const_cast', used to avoid a duplication of all
  // the ctors of the image hierarchy.
  template<class T, class Exact>
  image ## Dim ## d<T, Exact>::
  image ## Dim ## d(const image ## Dim ## d<T, Exact>& rhs) :
    super_type(const_cast< image ## Dim ## d<T, Exact>& > (rhs))
  {
    mlc_init_static_hierarchy(Exact);
  }
}
%}

%enddef // decl_basics(Dim)
