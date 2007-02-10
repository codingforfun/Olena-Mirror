// Copyright (C) 2004  EPITA Research and Development Laboratory
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


#ifndef OLN_UTILS_SE_STAT_HH
# define OLN_UTILS_SE_STAT_HH

# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/struct_elt.hh>

# include <ntg/core/abstract_hierarchy.hh>
# include <ntg/all.hh>
# include <ntg/core/macros.hh>
# include <oln/math/macros.hh>

namespace oln {
  namespace utils {
    namespace internal {
      /*! This class is used to transform a type to a floating type equivalent.
      **
      ** \todo FIXME: There should be a way to use the standard conversion,
      ** but I failed to find a good one. The problem is that a color<...>
      ** derived of vec<..> but the conversion between color and vec should be
      ** done through c.to_float(). \n
      ** Example:
      ** \verbatim
      ** rgb_u8::float_vec_type v = rgb_u8(127, 0, 255);
      ** // v contains 127., 0., 255  but should contain 0.5, 0,1.
      ** rgb_u8 c = v;
      ** // c = (255., 255., 255). Error.
      **
      ** rgb_u8::float_vec_type v2 = f_to_float_<rgb_u8::float_vec_type>
      **	 				rgb_u8(127, 0, 255);
      ** rgb_u8 c2 = v2;
      ** // c2 = (127, 0, 255). Ok.
      ** \endverbatim
      */
      template <typename DestT,
		typename SrcT>
      struct f_to_float_
      {
	static
	DestT
	doit(const SrcT& s)
	{
	  return s;
	}
      };

      /// Specialization of the \a f_to_float struct for the colors.
      template <unsigned ncomps,
		unsigned qbits,
		template <unsigned> class color_system>
      struct f_to_float_<typename ntg::color<ncomps,
					     qbits,
					     color_system>::float_vec_type,
			 ntg::color<ncomps, qbits, color_system> >
      {
	static
	typename ntg::color<ncomps, qbits, color_system>::float_vec_type
	doit(const ntg::color<ncomps, qbits, color_system>& s)
	{
	  return s.to_float();
	}
      };
    } // end namespace internal


    /*! \brief Compute the variance and the mean within a window.
    **
    ** \param Sum type used to compute the sum and the average.
    ** \param Var type used to compute the variance.
    **
    ** \note There are two parameters because for vectorial types the sum
    ** is usually a vector, and the variance a non vectorial type.
    */
    template <typename Sum = ntg::float_s, typename Var = ntg::float_s>
    class se_stat
    {
    public:
      /// type of used to sum values (usually floating point values)
      typedef Sum sum_type;
      /// type of the variance (usually floating point values)
      typedef Var variance_type;

      /*! Build a se stat.
      ** \arg im Input image.
      ** \arg p Origin of the structuring element.
      ** \arg s Window.
      */
      template <class I, class S>
      se_stat(const oln::abstract::image<I> &im,
	      const oln_point_type(I) &p,
	      const oln::abstract::struct_elt<S> &s)
      {
	compute(im, p, s);
      }

      /// Compute the mean and the variance.
      template <class I, class S>
      se_stat &
      compute(const oln::abstract::image<I> &im,
	      const oln_point_type(I) &p,
	      const oln::abstract::struct_elt<S> &s);

      /// Mean.
      inline Sum
      mean() const
      {
	return mean_;
      }

      /// Variance.
      inline Var
      variance() const
      {
	return variance_;
      }

      /// Sum.
      inline Sum
      sum() const
      {
	return sum_;
      }

      /// Cardinal of the structuring element.
      inline unsigned
      card() const
      {
	return card_;
      }

    protected:
      Var variance_;	///< Variance.
      Sum mean_;	///< Mean.
      Sum sum_;		///< Sum.
      int card_;	///< Card of the input structuring element.
    };

  } // end namespace utils

} // end namespace oln

#include "se_stat.hxx"

#endif //! OLN_UTILS_SE_STAT_HH
