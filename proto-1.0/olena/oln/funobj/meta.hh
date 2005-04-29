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

#ifndef OLENA_CORE_FUNOBJ_META_HH
# define OLENA_CORE_FUNOBJ_META_HH

# include <oln/funobj/abstract/unary.hh>
# include <oln/funobj/abstract/binary.hh>


namespace oln
{

  /// Plus.


  // fwd decl
  namespace f_ {
    template <typename L, typename R> struct plus;
  }

  // super type
  template <typename L, typename R>
  struct set_super_type < f_::plus<L,R> > { typedef f_::abstract::binary< f_::plus<L,R> > ret; };

  // props
  template <typename L, typename R>
  struct set_props < category::fun2, f_::plus<L,R> >
  {
    typedef ntg_return_type(plus, L, R) res_type;
    typedef L left_type;
    typedef R right_type;
  };  

  namespace f_
  {

    // class
    template <typename L, typename R>
    struct plus : public oln_super2_of_(f_::plus<L,R>)
    {
      typedef plus<L,R> self_type;

      const oln_fun2_type_of(self_type, res)
	impl_binop(const L& left, const R& right) const
	{
	  return left + right;
	}
    };

  } // end of namespace oln::f_


  static f_::binary_meta<f_::plus> f_plus;


} // end of namespace oln



#endif // ! OLENA_CORE_FUNOBJ_META_HH
