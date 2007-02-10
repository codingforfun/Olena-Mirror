// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_MATH_MACROS_HH
# define OLENA_MATH_MACROS_HH

# include <ntg/basics.hh>
# include <oln/basics.hh>

namespace oln {

  /*! \brief Useful functions.
  **
  ** \todo FIXME: I'm not proud of the code below
  ** think it could be better...
  **
  ** \todo FIXME: this code sounds really odd. Why does the operator()
  ** take value<Self> instead of Self directly ?
  ** FIXME: Self should be renamed into Exact.
  */
  namespace math {

    //! \brief Square fctor.
    template<class T>
    struct f_sqr
    {
      typedef T output_t;
      const T operator()(const T& val) const
      {
	return val * val;
      }
    };

    //! Square function.
    template<class T>
    const T sqr(const T& val)
    {
      f_sqr<T> f;
      return f(val);
    }

    //! Absolute value fctor.
    template<class T>
    struct f_abs
    {
      typedef T output_t;

      const T
      operator()(const T& val) const
      {
	return ::abs(val);
      }
    };

    //! Absolute value function.
    template<class T>
    const T
    abs(const T& val)
    {
      f_abs<T> f;
      return f(val);
    }

    ///  Internal purpose only.
    namespace internal {
      /// Dot product for non-vectorial types.
      template<typename DestValue, typename I, typename J>
      struct f_dot_product_nv
      {
	typedef typename ntg_is_a(I, ntg::non_vectorial)::ensure_type t;

	static DestValue
	product(const I &i, const J &j)
	{
	  return i * j;
	}
      };

      /// Dot product for vectorial types.
      template<typename DestValue, typename I, typename J>
      struct f_dot_product_v
      {
	typedef typename ntg_is_a(I, ntg::vectorial)::ensure_type t;
	//typedef typename mlc::eq<ntg_nb_comp(I), ntg_nb_comp(J)>::ensure_type t;

	static DestValue
	product(const I &i, const J &j)
	{
	  DestValue d(ntg_zero_val(DestValue));

	  for (unsigned k = 0; k < ntg_nb_comp(I); ++k)
	    d += i[k] * j[k];
	  return d;
	}
      };
    }

    /*! \brief Dot product.
    **
    ** \code
    ** #include <oln/math/macros.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   ntg::int_u8 i(3), j(4);
    **   // Print "12"
    **   std::cout << oln::math::dot_product<ntg::float_s>(i, j) << std::endl;
    **
    **   ntg::rgb_8 blue(0, 0, 200), yellow(0, 30, 10);
    **   // Print "2000"
    **   std::cout << oln::math::dot_product<ntg::float_s>(blue, yellow)
    **             << std::endl;
    ** }
    ** \endcode
    */
    template<typename DestValue, typename I, typename J>
    DestValue
    dot_product(const I &i, const J &j)
    {
      typedef typename mlc::if_<ntg_is_a(I, ntg::vectorial)::ret,
	internal::f_dot_product_v<DestValue, I, J>,
	internal::f_dot_product_nv<DestValue, I, J> >::ret fctor;
      return fctor::product(i, j);
    }

  } // end of math

} // end of oln

#endif // ! OLENA_MATH_MACROS_HH
