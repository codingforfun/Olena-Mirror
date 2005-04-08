// Copyright (C) 2005  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef NTG_DECLS_HH
# define NTG_DECLS_HH

# include <ntg/vect/cplx_representation.hh>


namespace ntg
{

  // abstract classes
  template <typename E> class value;
  template <typename E> class enum_value;
  template <typename E> class real_value;
  template <typename E> class float_value;
  template <typename E> class int_value;
  template <typename E> class uint_value;
  template <typename E> class sint_value;
  template <typename E> class vect_value;
  template <unsigned N, class T, class E> class vec;
  template <typename E> class color_value;

  // concrete classes
  class bin;
  template <unsigned nbits, class behavior> class int_u;
  template <unsigned nbits, class behavior> class int_s;
  template <class T, class interval> class cycle;
  template <class T, class interval, class behavior> class range;
  template <cplx_representation R, class T> class cplx;
  template <unsigned ncomps, unsigned qbits,
	    template <unsigned> class color_system> struct color;

} // end of namespace ntg


#endif // ! NTG_DECLS_HH

