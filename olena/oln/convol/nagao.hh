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


#ifndef OLN_CONVOL_NAGAO_HH
# define OLN_CONVOL_NAGAO_HH

# include <oln/utils/se_stat.hh>
# include <oln/core/abstract/struct_elt.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/image.hh>
# include <oln/core/abstract/image_with_type_with_dim.hh>
# include <mlc/array/1d.hh>
# include <mlc/cmp.hh>


namespace oln {
  namespace convol{
    namespace internal {
      /*! array of structuring elements.
      **
      ** \param Size Number of structuring elements.
      ** \param S structuring element type.
      */
      template <unsigned Size, class S>
      struct se_array
      {
	/// Type of the struturing elements.
	typedef mlc_exact_type(oln::abstract::struct_elt<S>) struct_elt_type;

	/// Type of the array.
	typedef typename mlc::array1d<typename mlc::array1d_info<Size>,
				      struct_elt_type > array_type;

	/// Number of structuring elements.
	enum {size = Size};

	se_array():delta_(0){}

	/// nth struturing element.
	struct_elt_type&
	operator[](int i)
	{
	  delta_ = -1;
	  precondition(0 <= i < Size);
	  return tab[i];
	}

	/// nth struturing element.
	struct_elt_type
	operator[](int i) const
	{
	  precondition(0 <= i < Size);
	  return tab[i];
	}

	/// Maximum of the delta of the structuring elements.
	coord
	delta() const
	{
	  // if not updated, compute the maximum.
	  if (delta_ <= 0)
	    {
	      for (unsigned i = 0; i < Size; ++i)
		if (tab[i].delta() > delta_)
		  delta_ = tab[i].delta();
	    }
	  postcondition (delta_ >= 0);
	  return delta_;
	}
      protected:
	/// Array of structuring elements.
	array_type tab;
	/*! \brief Maximum delta.
	**
	** \note delta_ < 0 means that it must be updated before
	** a read access.
	*/
	mutable coord delta_;
      };

      /*! Return the mean of the windows that has the smallest variance.
      **
      ** \arg im Input image.
      ** \arg p Origin of the structuring elements.
      ** \arg w array of structuring elements.
      **
      ** \param I Input image type.
      ** \param SE Structuring element type.
      ** \param Size Number of windows in the array.
      ** \param Sum Type use to add values of the input image.
      **
      ** \note \a Sum might be a float_s if the image is an image
      ** of integer, and might be an rgb_8::float_vec_type if I is
      ** an image of rgb_8.
      */
      template<class I, class SE, unsigned Size, typename Sum>
      inline Sum
      mean_of_smaller_variance(const oln::abstract::image<I> &im,
			       const oln_point_type(I) &p,
			       const internal::se_array<Size, SE> &w);
    } // end namespace internal

    /*! A Nagao filter generalized.
    **
    ** Each point in the input correspond to the mean of the
    ** window in which has the smallest variance.
    **
    ** \paran in Input image.
    ** \param sa array of structuring elements.
    **
    ** \see oln::convol::nagao
    */
    template <class I, unsigned Size, typename SE, typename Sum>
    inline oln_concrete_type(I)
    nagao_generalized(const oln::abstract::image<I>& in,
         	      const internal::se_array<Size, SE> &sa);

    template <class I>
    inline oln_concrete_type(I)
    nagao(const oln::abstract::non_vectorial_image_with_dim<2, I>& in);


    template <class I>
    inline oln_concrete_type(I)
    nagao(const oln::abstract::vectorial_image_with_dim<2, I>& in);
  } // end namespace convol

} // end namespace oln

#include "nagao.hxx"

#endif // OLENA_CONVOL_NAGAO_HH
