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

#ifndef OLENA_CORE_PW_MINUS_HH
# define OLENA_CORE_PW_MINUS_HH

# include <oln/core/pw/abstract/binary_function.hh>
# include <oln/core/pw/literal.hh>
# include <ntg/all.hh>
# include <oln/core/pw/macros.hh>


namespace oln {

  // fwd decl
  namespace pw {
    template <typename L, typename R> struct minus;
  }

  // super type
  template <typename L, typename R>
  struct set_super_type < pw::minus<L, R> > { typedef pw::abstract::binary_function<L, R, pw::minus<L, R> > ret; };

  // props
  template <typename L, typename R>
  struct set_props < category::pw, pw::minus<L, R> >
  {
    typedef ntg_return_type(minus,
			    oln_pw_type_of(L, value),
			    oln_pw_type_of(R, value)) value_type;
  };


  namespace pw {

    template <typename L, typename R>
    struct minus : public abstract::binary_function < L, R, minus<L, R> >
    {
      typedef minus<L, R> self_type;

      typedef oln_pw_type_of(self_type, point) point_type;
      typedef oln_pw_type_of(self_type, value) value_type;
      typedef oln_pw_type_of(self_type, size)  size_type;

      typedef abstract::binary_function<L, R, self_type > super_type;

      minus(const abstract::function<L>& left,
	    const abstract::function<R>& right) :
	super_type(left, right)
      {
      }

      const value_type impl_get(const point_type& p) const
      {
	return this->left(p) - this->right(p);
      }

    };


    // impl of abstract::function<E>::operator-()

    namespace abstract {

      template <typename E>
      minus<literal<oln_pw_type_of(E, value)>, E>
      function<E>::operator-() const
      {
	typedef literal<oln_pw_type_of(E, value)> lit_type;
	static const lit_type lhs = 0;
	minus< lit_type, E> tmp(lhs, this->exact());
	return tmp;
      }

    } // end of namespace oln::pw::abstract


  } // end of namespace oln::pw

} // end of namespace oln


/// Operator - on pwf

template <typename L, typename R>
oln::pw::minus<L, R> operator - (const oln::pw::abstract::function<L>& lhs,
				 const oln::pw::abstract::function<R>& rhs)
{
  precondition(lhs.size() == rhs.size());
  oln::pw::minus<L, R> tmp(lhs, rhs);
  return tmp;
}

oln_pw_operator(minus, -, int)
oln_pw_operator(minus, -, float)
oln_pw_operator(minus, -, double)


#endif // ! OLENA_CORE_PW_MINUS_HH
