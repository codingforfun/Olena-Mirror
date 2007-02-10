// -*- C++ -*-

// Copyright (C) 2003, 2006  EPITA Research and Development Laboratory
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


%include swilena_config.i

/*---------------------------.
| oln::image_size structures |
`---------------------------*/

%define imagesize_methods(T, Dim)
     coord nth(unsigned) const;
     
     bool operator==(const T&) const;
     %extend
     {
       bool eq(const T& other) const
	 { return (*self) == other; }
       unsigned dim() const
        { return self->dim; }
       std::string describe() const
	 {
	   std::ostringstream os;
	   os << #T "@" << self << " = (";
	   for (unsigned i = 0; i < self->dim; ++i)
	     {
	       if (i)
		 os << ',';
	       os << self->nth(i);
	     }
	   os << ')';
	   return os.str();
	 }
     }
#if Dim == 1
T(coord, coord);
#elif Dim == 2
T(coord, coord, coord);
#elif Dim == 3
T(coord, coord, coord, coord);
#endif

#if Dim >= 1
coord ncols() const;
#endif
#if Dim >= 2
coord nrows() const;
#endif
#if Dim >= 3
coord nslices() const;
#endif

%enddef

%define decl_imagesize(Dim)

%include swilena_core.i
#if Dim == 1
%{
#include <oln/core/image1d_size.hh>
%}
#elif Dim == 2
%{
#include <oln/core/image2d_size.hh>
%}  
#elif Dim == 3
%{
#include <oln/core/image3d_size.hh>
%}
#endif

namespace oln
{
  struct image ## Dim ## d_size
  {
    imagesize_methods(image ## Dim ## d_size, Dim)
  };
}

%enddef
