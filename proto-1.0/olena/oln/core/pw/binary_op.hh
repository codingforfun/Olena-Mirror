// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_PW_BINARY_OP_HH
# define OLENA_CORE_PW_BINARY_OP_HH

# include <mlc/is_a.hh>
# include <oln/core/pw/abstract/binary_function.hh>
# include <oln/funobj/abstract/binary.hh>


namespace oln {


  // fwd decl
  namespace pw {
    template <typename F, typename L, typename R> struct binary_op;
  }

  // super type
  template <typename F, typename L, typename R>
  struct set_super_type < pw::binary_op<F,L,R> > { typedef pw::abstract::binary_function< L, R, pw::binary_op<F,L,R> > ret; };

  // props
  template <typename F, typename L, typename R>
  struct set_props < category::pw, pw::binary_op<F,L,R> >
  {
    typedef
    typename f_::binary_meta_result< F,
				     oln_pw_type_of(L, value),
				     oln_pw_type_of(R, value) >::ret
    value_type;
  };



  namespace pw {


    template <typename F, typename L, typename R>
    struct binary_op : public abstract::binary_function< L, R, binary_op<F,L,R> >
    {
      typedef binary_op<F,L,R> self_type;
      typedef abstract::binary_function<L,R,self_type> super_type;

      F fun;

      binary_op(const abstract::function<L>& left,
		const abstract::function<R>& right) :
	super_type(left, right),
	fun()
      {
	mlc::or_< mlc_is_a(F, f_::binary_meta),
	          mlc_is_a(F, f_::binary1_meta) >::ensure();
      }

      typedef oln_pw_type_of(self_type, point) point_type;
      typedef oln_pw_type_of(self_type, value) value_type;

      const value_type impl_get(const point_type& p) const
      {
	const value_type tmp = this->fun(this->left(p), this->right(p));
	return tmp;
      }

    };


  } // end of namespace oln::pw

} // end of namespace oln



#endif // ! OLENA_CORE_PW_BINARY_OP_HH
