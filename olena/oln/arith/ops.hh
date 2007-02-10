// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_ARITH_OPS_HH
# define OLENA_ARITH_OPS_HH

# include <oln/basics.hh>
# include <oln/convert/basics.hh>
# include <oln/arith/internal/opdecls.hh>
# include <oln/math/macros.hh>

# include <ntg/basics.hh>

# include <functional>

namespace oln {
  /*!
  ** \brief Arithmetic implementation.
  */
  namespace arith {

    oln_arith_declare_binrecval_functors_(plus,
					  val1 + val2,
					  val + cst_);
    // need a cast for the case int_u - int_u
    oln_arith_declare_binrecval_functors_(minus,
					  val1 - val2,
					  val - cst_);
    oln_arith_declare_binrecval_functors_(times,
					  val1 * val2,
					  val * cst_);
    oln_arith_declare_binrecval_functors_(div,
					  val1 / val2,
					  val / cst_);
    oln_arith_declare_binrecval_functors_(min,
					  ntg::min(val1, val2),
					  ntg::min(val, cst_));
    oln_arith_declare_binrecval_functors_(max,
					  ntg::max(val1, val2),
					  ntg::max(val, cst_));

    oln_arith_declare_all_binop_procs_(plus)
    oln_arith_declare_all_binop_procs_(minus)
    oln_arith_declare_all_binop_procs_(times)
    oln_arith_declare_all_binop_procs_(div)
    oln_arith_declare_all_binop_procs_(min)
    oln_arith_declare_all_binop_procs_(max)

  } // end of arith

} // end of oln


#endif // ! OLENA_ARITH_OPS_HH
