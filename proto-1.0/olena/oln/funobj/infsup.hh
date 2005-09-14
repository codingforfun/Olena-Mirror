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

#ifndef OLENA_CORE_FUNOBJ_INFSUP_HH
# define OLENA_CORE_FUNOBJ_INFSUP_HH

# include <oln/funobj/arith.hh>
//# include <oln/funobj/logic.hh>


namespace oln
{

  namespace f_
  {

    // Default for inf/sup is min/max.

    template <typename T>
    struct inf_ : public min_<T>
    {
    };

    template <typename T>
    struct sup_ : public max_<T>
    {
    };

  } // end of namespace oln::f_


  typedef f_::binary1_meta<f_::inf_> f_inf_type;
  static f_inf_type f_inf;

  typedef f_::binary1_meta<f_::sup_> f_sup_type;
  static f_sup_type f_sup;


} // end of namespace oln


#endif // ! OLENA_CORE_FUNOBJ_INFSUP_HH
