// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_FOLD_HH
# define OLENA_CORE_FOLD_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/iter.hh>
# include <oln/core/macros.hh>
# include <mlc/typeadj.hh>
# include <oln/core/macros.hh>

namespace oln {

  // Compute f(...f(f(val,i_0),i_1)...,i_n), where i_0...i_n
  // are the value associated to each abstract::image point.
  template<class AdaptableBinaryFun, class I> inline
  typename AdaptableBinaryFun::result_type
  fold(AdaptableBinaryFun f,
       // f could return a reference or a const.  Make sure VAL is assignable.
       typename mlc::typeadj<
         typename AdaptableBinaryFun::result_type>::mutable_val val,
       const abstract::image<I>& input)
  {
    // FIXME: ensure that first_argument_type == result_type.
    oln_iter_type(I) p(input);
    for_all(p)
      val = f(val, input[p]);
    return val;
  }


  // Compute f(...f(f(i_0,i_1),i_2)...,i_n).
  template<class AdaptableBinaryFun, class I> inline
  typename AdaptableBinaryFun::result_type
  fold(AdaptableBinaryFun f, const abstract::image<I>& input)
  {
    // FIXME: ensure that first_argument_type == result_type.
    oln_iter_type(I) p(input);
    p = begin;
    // f could return a reference or a const, so make sure VAL is assignable.
    typename mlc::typeadj<
      typename AdaptableBinaryFun::result_type>::mutable_val val
        = input[p];
    for_all_remaining(p)
      val = f(val, input[p]);
    return val;
  }

} // end of oln

#endif // ! OLENA_CORE_FOLD_HH
