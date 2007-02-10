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

/*------------------------.
| oln::w_window structure |
`------------------------*/

%define structelt_methods_w(T, W, Dim)
     structelt_methods(T, Dim)

     W w(unsigned) const;
     
     T& add(const oln::dpoint ## Dim ## d&, const W& );

%enddef

%define decl_w_win(Dim)

%include swilena_describe.i
%include swilena_core.i

%import swilena_structelt ## Dim ## d.i

#if Dim == 1
%{
#include <oln/core/w_window1d.hh>
%}
#elif Dim == 2
%{
#include <oln/core/w_window2d.hh>
%}
#elif Dim == 3
%{
#include <oln/core/w_window3d.hh>
%}
#else
#error Unsupported dimension.
#endif

namespace oln
{
  template<typename T>
  struct w_window ## Dim ## d
  {
    structelt_methods_w(oln::w_window ## Dim ## d<T>, T, Dim);

    %extend
    {
      static oln::w_window ## Dim ## d<T> of_window(T w, const oln::window ## Dim ## d& win)
	{ return oln::mk_w_win_from_win(w, win); }
    }
  };
}
%enddef

%define make_w_win(name, Dim, T)
%template(name) oln::w_window ## Dim ## d<T >;
%enddef
