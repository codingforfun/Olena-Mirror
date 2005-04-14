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
# include <oln/core/pw/abstract/unary_function.hh>
# include <oln/core/pw/abstract/binary_function.hh>
# include <oln/core/pw/macros.hh>


namespace oln {

  /*--------.
  | Unary.  |
  `--------*/

  // fwd decl
  namespace pw {
    template <typename F, typename T> struct apply1;
  }

  // super type
  template <typename F, typename T>
  struct set_super_type < pw::apply1<F, T> >
  {
    typedef pw::abstract::unary_function< T, pw::apply1<F, T> > ret;
  };

  // props
  template <typename F, typename T>
  struct set_props < category::pw, pw::apply1<F, T> >
  {
    typedef oln_pw_type_of(T, point) point_type;
    typedef typename F::result_type value_type;
    typedef oln_pw_type_of(T, size) size_type;
  };


  namespace pw {

    template <typename F, typename T>
    struct apply1 : public abstract::unary_function < T, apply1<F, T> >
    {
      typedef apply1<F, T> self_type;

      typedef oln_pw_type_of(self_type, point) point_type;
      typedef oln_pw_type_of(self_type, value) value_type;
      typedef oln_pw_type_of(self_type, size)  size_type;

      typedef abstract::unary_function<T, self_type> super_type;

      F f_;

      apply1(const mlc::abstract::unary_function<F>& f,
	    const abstract::function<T>& input) :
	super_type(input),
	f_(f.exact())
      {
      }

      const value_type impl_get(const point_type& p) const
      {
	return f_(this->input(p));
      }
    };


  } // end of namespace oln::pw


  /*---------.
  | Binary.  |
  `---------*/

  // fwd decl
  namespace pw {
    template <typename F, typename T1, typename T2> struct apply2;
  }

  // super type
  template <typename F, typename T1, typename T2>
  struct set_super_type < pw::apply2<F, T1, T2> >
  {
    typedef pw::abstract::binary_function<T1, T2, pw::apply2<F, T1, T2> > ret;
  };

  // props
  template <typename F, typename T1, typename T2>
  struct set_props < category::pw, pw::apply2<F, T1, T2> >
  {
    typedef typename F::result_type value_type;
  };

  namespace pw {

    template <typename F, typename T1, typename T2>
    struct apply2 :
      public abstract::binary_function <T1, T2, apply2<F, T1, T2> >
    {
      typedef apply2<F, T1, T2> self_type;

      typedef oln_pw_type_of(self_type, point) point_type;
      typedef oln_pw_type_of(self_type, value) value_type;
      typedef oln_pw_type_of(self_type, size)  size_type;

      typedef
	abstract::binary_function <T1, T2, apply2<F, T1, T2> > super_type;

      F f_;

      apply2(const mlc::abstract::binary_function<F>& f,
	     const abstract::function<T1>& left,
	     const abstract::function<T2>& right) :
	super_type(left, right),
	f_(f.exact())
      {
      }

      const value_type impl_get(const point_type& p) const
      {
	return f_(this->left(p), this->right(p));
      }
    };

  } // end of namespace oln::pw

} // end of namespace oln


/// apply function on pwf

template <typename F, typename T>
oln::pw::apply1<F, T>
p_apply (const mlc::abstract::unary_function<F>& f,
	 const oln::pw::abstract::function<T>& x)
{
  oln::pw::apply1<F, T> tmp(f, x);
  return tmp;
}

template <typename F, typename T1, typename T2>
oln::pw::apply2<F, T1, T2>
p_apply (const mlc::abstract::binary_function<F>& f,
	 const oln::pw::abstract::function<T1>& x,
	 const oln::pw::abstract::function<T2>& y)
{
  oln::pw::apply2<F, T1, T2> tmp(f, x, y);
  return tmp;
}


#endif // ! OLENA_CORE_PW_APPLY_HH
