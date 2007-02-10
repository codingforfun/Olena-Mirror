// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and
// Development Laboratory
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

#ifndef OLENA_ARITH_LOGIC_HH
# define OLENA_ARITH_LOGIC_HH

# include <oln/basics.hh>
# include <ntg/bin.hh>
# include <oln/convert/basics.hh>
# include <oln/arith/internal/opdecls.hh>
# include <functional>

namespace oln {

  namespace arith {

    /// Functor AND operators.
    oln_arith_declare_binfixedtype_functors_(logic_and, ntg::bin,
					     val1 && val2,
					     val && cst_);

    /// Functor OR operators.
    oln_arith_declare_binfixedtype_functors_(logic_or, ntg::bin,
					     val1 || val2,
					     val || cst_);
    /// Functor AND NOT operators.
    oln_arith_declare_binfixedtype_functors_(logic_and_not, ntg::bin,
					     val1 && ! val2,
					     val && ! cst_);

    /// Functor NOT operator.
    oln_arith_declare_unfixedtype_functor_(logic_not, ntg::bin, ! val);

    /// AND NOT operators.
    oln_arith_declare_all_nongenericbinop_procs_(logic_and)

    /// OR operators.
    oln_arith_declare_all_nongenericbinop_procs_(logic_or)

    /// AND NOT operators.
    oln_arith_declare_all_nongenericbinop_procs_(logic_and_not)

    /// NOT operator.
    oln_arith_declare_nongenericunop_procs_(logic_not)

  } // end of arith

} // end of oln


#endif // ! OLENA_ARITH_LOGIC_HH
