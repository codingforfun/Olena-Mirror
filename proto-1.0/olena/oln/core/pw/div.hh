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

#ifndef OLENA_CORE_PW_DIV_HH
# define OLENA_CORE_PW_DIV_HH

# include <oln/core/pw/abstract/binary_function.hh>
# include <oln/core/pw/literal.hh>
# include <ntg/all.hh>


namespace oln {


  namespace pw { // means "point-wise"

    // fwd decl
    template <typename L, typename R> struct div;

    template <typename L, typename R>
    struct traits < div<L, R> >
    {
      typedef abstract::binary_function<L, R, div<L, R> > super_type;
      typedef typename traits<super_type>::point_type point_type;
      typedef typename traits<super_type>::size_type  size_type;
      typedef ntg_return_type(div,
			      oln_pw_value_type(L),
			      oln_pw_value_type(R)) value_type;
    };

    template <typename L, typename R>
    struct div : public abstract::binary_function < L, R, div<L, R> >
    {
      typedef div<L, R> self_type;

      typedef oln_pw_point_type(self_type) point_type;
      typedef oln_pw_value_type(self_type) value_type;
      typedef oln_pw_size_type(self_type)  size_type;

      typedef abstract::binary_function<L, R, self_type> super_type;

      div(const abstract::function<L>& left,
	  const abstract::function<R>& right) :
	super_type(left, right)
      {
      }

      const value_type impl_get(const point_type& p) const
      {
	precondition(this->right(p) != 0);
	return this->left(p) / this->right(p);
      }

    };


  } // end of namespace oln::pw

} // end of namespace oln



/// Operator / on pwf

template <typename L, typename R>
oln::pw::div<L, R> operator / (const oln::pw::abstract::function<L>& lhs,
			       const oln::pw::abstract::function<R>& rhs)
{
  precondition(lhs.size() == rhs.size());
  oln::pw::div<L, R> tmp(lhs, rhs);
  return tmp;
}


oln_pw_operator(div, /, int)
oln_pw_operator(div, /, float)
oln_pw_operator(div, /, double)


#endif // ! OLENA_CORE_PW_DIV_HH
