// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_MINMAX_HH
# define OLENA_MORPHO_MINMAX_HH

# include <oln/arith/ops.hh>
# include <oln/arith/logic.hh>

namespace oln {
  namespace morpho {

    /*
      arith::min and arith::max should not work on type bin because
      there is no order on this type.

      However, morphological operatators definied on binary image
      are usually generalized to other types by using 'min'
      instead of 'and', and 'max' instead of 'or'.

      To ease the writing of generic algorithm we define
      morpho::min, and morpho::max. The sole difference with
      the arith:: version of these operators is that these work
      with bin.
    */

    template<class T>
    class f_min : public arith::f_min<T, T> {};
    template<>
    class f_min<bin> : public arith::f_logic_and {};

    template<class T>
    class f_max : public arith::f_max<T, T> {};
    template<>
    class f_max<bin> : public arith::f_logic_or {};

    _OLN_ARITH_DECLARE_SEMIGENERIC_BINOP_PROCS(min);
    _OLN_ARITH_DECLARE_SEMIGENERIC_BINOP_PROCS(max);

  } // morpho
} // oln

#endif /* OLENA_MORPHO_MINMAX_HH */
