// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_ARITH_OPS_HH
# define OLENA_ARITH_OPS_HH

# include <oln/basics.hh>
# include <oln/math/macros.hh>
# include <oln/arith/internal/opdecls.hh>
# include <ntg/global_ops_traits.hh>
# include <ntg/typetraits_builtins.hh>
# include <oln/convert/basics.hh>
# include <functional>

namespace oln {
  namespace arith {

    _OLN_ARITH_DECLARE_BINRECVAL_FUNCTORS(plus,
					  val1.self() + val2.self(),
					  val.self() + _cst);
    // need a cast for the case int_u - int_u
    _OLN_ARITH_DECLARE_BINRECVAL_FUNCTORS(minus,
					  val1.self() - val2.self(),
					  val.self() - _cst);
    _OLN_ARITH_DECLARE_BINRECVAL_FUNCTORS(times,
					  val1.self() * val2.self(),
					  val.self() * _cst);
    _OLN_ARITH_DECLARE_BINRECVAL_FUNCTORS(div,
					  val1.self() / val2.self(),
					  val.self() / _cst);
    _OLN_ARITH_DECLARE_BINRECVAL_FUNCTORS(min,
					  min(val1.self(), val2.self()),
					  min(val.self(), _cst));
    _OLN_ARITH_DECLARE_BINRECVAL_FUNCTORS(max,
					  max(val1.self(), val2.self()),
					  max(val.self(), _cst));

    _OLN_ARITH_DECLARE_ALL_BINOP_PROCS(plus);
    _OLN_ARITH_DECLARE_ALL_BINOP_PROCS(minus);
    _OLN_ARITH_DECLARE_ALL_BINOP_PROCS(times);
    _OLN_ARITH_DECLARE_ALL_BINOP_PROCS(div);
    _OLN_ARITH_DECLARE_ALL_BINOP_PROCS(min);
    _OLN_ARITH_DECLARE_ALL_BINOP_PROCS(max);

  } // end of arith

} // end of oln


#endif // ! OLENA_ARITH_OPS_HH
