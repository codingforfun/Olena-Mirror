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

#ifndef OLENA_OPS_CMP_HH
# define OLENA_OPS_CMP_HH

# include <oln/ops/macro.hh>
# include <oln/core/pw/cmp.hh>



namespace oln {

  oln_decl_image_binary_op(eq);
  oln_decl_image_binary_op(neq);
  oln_decl_image_binary_op(less);
  oln_decl_image_binary_op(leq);
  oln_decl_image_binary_op(greater);
  oln_decl_image_binary_op(geq);

} // end of namespace oln


#endif // ! OLENA_OPS_CMP_HH
