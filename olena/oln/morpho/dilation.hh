// Copyright (C) 2001, 2002, 2004  EPITA Research and Development Laboratory
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
// As a special exception, you may use this filek as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_MORPHO_DILATION_HH
# define OLENA_MORPHO_DILATION_HH

# include <oln/basics.hh>
# include <oln/morpho/stat.hh>
# include <oln/morpho/fast_morpho.hh>
# include <mlc/cmp.hh>

namespace oln {

  namespace morpho {

    /*!
    ** \brief Processing dilation.
    **
    ** Compute the morphological dilation of input using se
    ** as structural element.
    **
    ** On grey-scale images, each point is replaced by the maximum value
    ** of its neighbors, as indicated by se.  On binary images,
    ** a logical or is performed between neighbors.
    **
    ** The morpho::fast version of this function use a different
    ** algorithm: This algorithm is described in
    ** Implementation of morphological operations from:
    ** M. Van Droogenbroeck and H. Talbot.
    ** "Fast computation of morphological operations with arbitrary
    ** structuring elements". Pattern Recognition Letters,
    ** 17(14):1451-1460, 1996.
    **
    ** An histogram of the value of the neighborhood indicated by
    ** se is updated while iterating over all point of the
    ** image.  Doing so is more efficient  when the
    ** structural element is large.
    **
    ** \param I Exact type of the input image.
    ** \param E Exact type of the neighborhood.
    **
    ** \arg input The input image.
    ** \arg se Structuring element to use.
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/morpho/dilation.hh>
    ** #include <oln/level/compare.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   typedef oln::image2d<ntg::bin>	im_type;
    **
    **   im_type im1(oln::load(IMG_IN "object.pbm"));
    **   save(oln::morpho::dilation(im1, oln::win_c8p()), IMG_OUT "oln_morpho_dilation.pbm");
    **   return  0;
    ** }
    ** \endcode
    **
    ** \image html object.png
    ** \image latex object.png
    ** =>
    ** \image html oln_morpho_dilation.png
    ** \image latex oln_morpho_dilation.png
    */
    template<class I, class E>
    oln_concrete_type(I)
      dilation(const abstract::non_vectorial_image<I> &input,
	       const abstract::struct_elt<E>& se)
    {
      mlc::eq<I::dim, E::dim>::ensure();

      oln_concrete_type(I) output(input.size());
      input.border_adapt_copy(se.delta());
      oln_iter_type(I) p(input);

      for_all (p)
	output[p] = morpho::max(input, p, se);
      return output;
    }

    /*!
    ** \brief Perform morphological dilation iterated n times.
    **
    **
    ** \param I Exact type of the input image.
    ** \param E Exact type of the structuring element.
    **
    ** \arg input Input image.
    ** \arg se Structuring element to use.
    ** \arg n Number of iterations.
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/morpho/dilation.hh>
    ** #include <oln/level/compare.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   typedef oln::image2d<ntg::bin>	im_type;
    **
    **   im_type im1(oln::load(IMG_IN "object.pbm"));
    **   save(oln::morpho::n_dilation(im1, oln::win_c8p(), 5), IMG_OUT "oln_morpho_n_dilation.pbm");
    **   return  0;
    ** }
    ** \endcode
    **
    ** \image html object.png
    ** \image latex object.png
    ** =>
    ** \image html oln_morpho_n_dilation.png
    ** \image latex oln_morpho_n_dilation.png
    **
    */
    template <class I, class E>
    oln_concrete_type(I)
      n_dilation(const abstract::non_vectorial_image<I> & input,
		 const abstract::struct_elt<E>& se,
		 unsigned n)
    {
      precondition(n > 0);
      oln_concrete_type(I) output = input.clone();
      for (unsigned i = 0; i < n; ++i)
	{
	 oln_concrete_type(I) work = dilation(output, se);
	  output = work;
 	}
      return output;
    }

    namespace fast {
      template<class I, class E>
      oln_concrete_type(I)
	dilation(const abstract::non_vectorial_image<I>& input,
		 const abstract::struct_elt<E>& se)
      {
	return fast_morpho<I, E, utils::histogram_max>(input, se);
      }
    }
  } // end of morpho

} // end of oln

#endif // ! OLENA_MORPHO_DILATION_HH
