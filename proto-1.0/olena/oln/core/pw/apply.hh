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

#ifndef OLENA_CORE_PW_APPLY_HH
# define OLENA_CORE_PW_APPLY_HH

# include <mlc/fun.hh>
# include <oln/core/pw/abstract/function.hh>
# include <oln/core/pw/macros.hh>


namespace oln {

  /*--------.
  | Unary.  |
  `--------*/

  // fwd decl
  namespace pw {
    template <typename F, typename T>
    struct apply;
  }

  // super type
  template <typename F, typename T>
  struct set_super_type < pw::apply<F, T> >
  {
    typedef pw::abstract::function< pw::apply<F, T> > ret;
  };

  // props
  template <typename F, typename T>
  struct set_props < category::pw, pw::apply<F, T> >
  {
    typedef oln_pw_type_of(T, point) point_type;
    typedef typename F::result_type value_type;
    typedef oln_pw_type_of(T, size) size_type;
  };


  namespace pw {

    template <typename F, typename T>
    struct apply : public abstract::function < apply<F, T> >
    {
      typedef apply<F, T> self_type;

      typedef oln_pw_type_of(self_type, point) point_type;
      typedef oln_pw_type_of(self_type, value) value_type;
      typedef oln_pw_type_of(self_type, size)  size_type;

      typedef abstract::function<self_type> super_type;

      F f_;
      T input_;

      apply(const mlc::abstract::unary_function<F>& f,
	    const abstract::function<T>& input) :
	super_type(),
	f_(f.exact()),
	input_(input.exact())
      {
      }

      const size_type& impl_size() const
      {
	return input_.size();
      }

      const value_type impl_get(const point_type& p) const
      {
	return f_(input_(p));
      }

      bool impl_hold(const point_type& p) const
      {
	return input_.hold(p);
      }

      bool impl_hold_large(const point_type& p) const
      {
	return input_.hold_large(p);
      }
    };


  } // end of namespace oln::pw

} // end of namespace oln


/// apply function on pwf

template <typename F, typename T>
oln::pw::apply<F, T> p_apply (const mlc::abstract::unary_function<F>& f,
			      const oln::pw::abstract::function<T>& x)
{
  oln::pw::apply<F, T> tmp(f, x);
  return tmp;
}


#endif // ! OLENA_CORE_PW_APPLY_HH
