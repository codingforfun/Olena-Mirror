// -*- C++ -*-

// Copyright (C) 2006  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.


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
