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

#ifndef OLENA_OPS_ARITH_HH
# define OLENA_OPS_ARITH_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/pw/image.hh>
# include <oln/core/pw/value.hh>
# include <oln/core/pw/arith.hh>


namespace oln
{

  /// Operator-like "min" and "max" between 2 images.

  template <typename L, typename R>
  image_from_pwf< pw::binary_op< f_min_type,
				 pw::value<L>,
				 pw::value<R> > >
  min (const abstract::image<L>& lhs,
       const abstract::image<R>& rhs)
  {
    return image_for_all_p( oln::min(pw_value(lhs), pw_value(rhs)) );
  }

  template <typename L, typename R>
  image_from_pwf< pw::binary_op< f_max_type,
				 pw::value<L>,
				 pw::value<R> > >
  max (const abstract::image<L>& lhs,
       const abstract::image<R>& rhs)
  {
    return image_for_all_p( oln::max(pw_value(lhs), pw_value(rhs)) );
  }

  /// Operator-like "inf" and "sup" between 2 images.

  template <typename L, typename R>
  image_from_pwf< pw::binary_op< f_inf_type,
				 pw::value<L>,
				 pw::value<R> > >
  inf (const abstract::image<L>& lhs,
       const abstract::image<R>& rhs)
  {
    return image_for_all_p( oln::inf(pw_value(lhs), pw_value(rhs)) );
  }

  template <typename L, typename R>
  image_from_pwf< pw::binary_op< f_sup_type,
				 pw::value<L>,
				 pw::value<R> > >
  sup (const abstract::image<L>& lhs,
       const abstract::image<R>& rhs)
  {
    return image_for_all_p( oln::sup(pw_value(lhs), pw_value(rhs)) );
  }



  /// Operator '+' between 2 images.

  template <typename L, typename R>
  oln::image_from_pwf< oln::pw::binary_op< oln::f_plus_type,
					   oln::pw::value<L>,
					   oln::pw::value<R> > >
  operator + (const oln::abstract::image<L>& lhs,
	      const oln::abstract::image<R>& rhs)
  {
    return oln::image_for_all_p(oln::pw_value(lhs) + oln::pw_value(rhs));
  }


  /// Operator '-' between 2 images.

  template <typename L, typename R>
  oln::image_from_pwf< oln::pw::binary_op< oln::f_minus_type,
					   oln::pw::value<L>,
					   oln::pw::value<R> > >
  operator - (const oln::abstract::image<L>& lhs,
	      const oln::abstract::image<R>& rhs)
  {
    return oln::image_for_all_p(oln::pw_value(lhs) - oln::pw_value(rhs));
  }


  /// Operator '*' between 2 images.

  template <typename L, typename R>
  oln::image_from_pwf< oln::pw::binary_op< oln::f_times_type,
					   oln::pw::value<L>,
					   oln::pw::value<R> > >
  operator * (const oln::abstract::image<L>& lhs,
	      const oln::abstract::image<R>& rhs)
  {
    return oln::image_for_all_p(oln::pw_value(lhs) * oln::pw_value(rhs));
  }


  /// Operator '/' between 2 images.

  template <typename L, typename R>
  oln::image_from_pwf< oln::pw::binary_op< oln::f_div_type,
					   oln::pw::value<L>,
					   oln::pw::value<R> > >
  operator / (const oln::abstract::image<L>& lhs,
	      const oln::abstract::image<R>& rhs)
  {
    return oln::image_for_all_p(oln::pw_value(lhs) / oln::pw_value(rhs));
  }


  /// Operator '%' between 2 images.

  template <typename L, typename R>
  oln::image_from_pwf< oln::pw::binary_op< oln::f_mod_type,
					   oln::pw::value<L>,
					   oln::pw::value<R> > >
  operator % (const oln::abstract::image<L>& lhs,
	      const oln::abstract::image<R>& rhs)
  {
    return oln::image_for_all_p(oln::pw_value(lhs) % oln::pw_value(rhs));
  }


} // end of namespace oln



// FIXME...

// /// Operator - (unary) on image.

// template <typename R>
// oln::image_from_pw< oln::pw::minus< oln::pw::literal< oln_pw_type_of(R, value) >,
// 				    oln::pw::image<R> > >
// operator - (const oln::abstract::image<R>& rhs)
// {
//   return oln::image_for_all_p( - oln::p_value(rhs));
// }


// template <typename I, typename F>
// void operator + (const oln::abstract::image<I>&,
// 		 const oln::pw::abstract::function<F>&)
// {
//   struct OLENA_ERROR__args_are_not_compatible();
// }
// template <typename F, typename I>
// void operator + (const oln::pw::abstract::function<F>&,
// 		 const oln::abstract::image<I>&)
// {
//   struct OLENA_ERROR__args_are_not_compatible();
// }
// FIXME: to be continued...


#endif // ! OLENA_OPS_ARITH_HH
