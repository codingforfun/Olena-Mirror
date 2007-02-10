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

/*--------------------------------------.
| oln::point and oln::dpoint structures |
`--------------------------------------*/

// Both structures share nearly the same interface; 
// the following macro factorizes the declaration.

%define point_methods(T, Dim)

     // default constructor
     T();

     // access to dimension values
     coord nth(unsigned) const;
     %extend {
       void nth(unsigned d, coord val)
	 { self->nth(d) = val; }
     }

     // point arithmetics
     T operator+(const oln::dpoint ## Dim ## d&) const;
     T& operator+=(const oln::dpoint ## Dim ## d&);

     T operator-(const oln::dpoint ## Dim ## d&) const;
     T& operator-=(const oln::dpoint ## Dim ## d&);

     T operator-() const;

     // some scripting languages do not support operator
     // overloading. Provide a substitution.
     %extend {
       T& add(const oln::dpoint ## Dim ## d& d)
	 { return (*self) += d; }
       T& sub(const oln::dpoint ## Dim ## d& d)
	 { return (*self) -= d; }
       T neg() const
	 { return -(*self); }
     }

// Specialized constructors.
#if Dim == 1
     T(coord);
#elif Dim == 2
     T(coord, coord);
#elif Dim == 3
     T(coord, coord, coord);
#endif


// Specialized dimension accessors.
#if Dim >= 1
     coord col() const;
     %extend {
       void col(coord x) 
	 { self->col() = x; }
     }
#endif

#if Dim >= 2
     coord row() const;
     %extend {
       void row(coord x)
	 { self->row() = x; }
     }
#endif

#if Dim >= 3
     coord slice() const;
     %extend {
       void slice(coord x)
	 { self->slice() = x; }
     }
#endif

EXTEND_DESCRIBE(T)

%enddef


// The generic declarator for a given dimension.

%define decl_point(Dim)

%include swilena_describe.i
%include swilena_core.i

#if Dim == 1
%{
#include <oln/core/dpoint1d.hh>
#include <oln/core/point1d.hh>
%}
#elif Dim == 2
%{
#include <oln/core/dpoint2d.hh>
#include <oln/core/point2d.hh>
%}
#elif Dim == 3
%{
#include <oln/core/dpoint3d.hh>
#include <oln/core/point3d.hh>
%}
#else
#error Unsupported dimension.
#endif

namespace oln
{
  struct dpoint ## Dim ## d;

  struct point ## Dim ## d
   {
     point_methods(point ## Dim ## d, Dim)
     
     dpoint ## Dim ## d operator-(const point ## Dim ## d&) const;
  };

  struct dpoint ## Dim ## d
  {
    dpoint ## Dim ## d(const point ## Dim ## d);

    point_methods(dpoint ## Dim ## d, Dim)     
  };
}

%enddef
