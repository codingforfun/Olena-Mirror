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

#ifndef OLENA_MORPHO_GEODESIC_DILATION_HH
# define OLENA_MORPHO_GEODESIC_DILATION_HH

# include <oln/basics.hh>
# include <oln/morpho/stat.hh>
# include <oln/morpho/dilation.hh>
# include <oln/convert/conversion_ng_se.hh>
# include <oln/level/compare.hh>
# include <oln/arith/ops.hh>

# include <ntg/basics.hh>

# include <mlc/cmp.hh>

namespace oln {
  namespace morpho {

    /*!
    ** \brief Processing a geodesic dilation.
    **
    ** \param I1 Exact type of image marker.
    ** \param I2 Exact type of image mask.
    ** \param N Exact type of neighborhood.
    **
    ** \arg marker Image to work on.
    ** \arg mask Image used for geodesic dilation.
    ** \arg Ng Neighborhood to use.
    **
    ** Compute  the geodesic dilation  of marker  with respect  to the
    ** mask  image  using  se  as  structuring  element.  Soille
    ** p.156.
    ** \pre Mask must be greater or equal than marker.
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/morpho/opening.hh>
    ** #include <oln/morpho/geodesic_dilation.hh>
    ** #include <oln/level/compare.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   typedef oln::image2d<ntg::int_u8>	im_type;
    **
    **   im_type	im1(oln::load(IMG_IN "lena128.pgm"));
    **   im_type	im2(oln::morpho::opening(im1, oln::win_c4p()));
    **
    **   save(oln::morpho::geodesic_dilation(im2, im1, oln::neighb_c4()),
    **                                       IMG_OUT "oln_morpho_geodesic_dilation.pbm");
    **   return  0;
    ** }
    ** \endcode
    **
    ** \image html lena128_pgm.png
    ** \image latex lena128_pgm.png
    ** =>
    ** \image html oln_morpho_geodesic_dilation.png
    ** \image latex oln_morpho_geodesic_dilation.png
    **
    */
    template<class I1, class I2, class N>
    oln_concrete_type(I1)
      geodesic_dilation(const abstract::non_vectorial_image<I1> & marker,
			const abstract::non_vectorial_image<I2> & mask,
			const abstract::neighborhood<N>& Ng)
    {
      mlc::eq<I1::dim, I2::dim>::ensure();
      mlc::eq<I1::dim, N::dim>::ensure();
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(mask, marker));
      return arith::min<oln_concrete_type(I1)>(dilation(marker,
							convert::ng_to_cse(Ng)),
					       mask);
    }

    namespace sure {
      /*!
      ** \brief Processing a geodesic dilation.
      **
      ** \param I1 Exact type of image marker.
      ** \param I2 Exact type of image mask.
      ** \param N Exact type of neighborhood.
      **
      ** \arg marker Image to work on.
      ** \arg mask Image used for geodesic dilation.
      ** \arg Ng Neighborhood to use.
      **
      ** Compute  the geodesic dilation  of marker  with respect  to the
      ** mask  image  using  se  as  structuring  element.  Soille
      ** p.156.
      ** \pre Mask must be greater or equal than marker.
      **
      ** \warning This version shouldn't  be use, since it exists only
      ** to have a reference algorithm.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/opening.hh>
      ** #include <oln/morpho/geodesic_dilation.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena128.pgm"));
      **   im_type	im2(oln::morpho::opening(im1, oln::win_c4p()));
      **
      **   save(oln::morpho::sure::geodesic_dilation(im2, im1, oln::neighb_c4()),
      **                                       IMG_OUT "oln_morpho_sure_geodesic_dilation.pbm");
      **   return  0;
      ** }
      ** \endcode
      **
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_sure_geodesic_dilation.png
      ** \image latex oln_morpho_sure_geodesic_dilation.png
      **
      */
      template<class I1, class I2, class N>
      oln_concrete_type(I1)
	geodesic_dilation(const abstract::non_vectorial_image<I1> & marker,
			  const abstract::non_vectorial_image<I2> & mask,
			  const abstract::neighborhood<N>& Ng)
      {
	mlc::eq<I1::dim, I2::dim>::ensure();
	mlc::eq<I1::dim, N::dim>::ensure();
	precondition(marker.size() == mask.size());
	precondition(level::is_greater_or_equal(mask, marker));

	oln_concrete_type(I1) output(marker.size());
	marker.border_adapt_copy(Ng.delta());
	oln_iter_type(I1) p(marker);
	for_all (p)
	  output[p] = std::min(morpho::max(marker, p, convert::ng_to_cse(Ng)), mask[p]);
	return output;
      }

    } // sure
  } // morpho
} // oln

#endif // OLENA_MORPHO_GEODESIC_DILATION_HH
