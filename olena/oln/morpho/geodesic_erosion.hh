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

#ifndef OLENA_MORPHO_GEODESIC_EROSION_HH
# define OLENA_MORPHO_GEODESIC_EROSION_HH

# include <oln/basics.hh>
# include <oln/morpho/stat.hh>
# include <oln/morpho/erosion.hh>
# include <oln/convert/conversion_ng_se.hh>
# include <oln/level/compare.hh>
# include <oln/arith/ops.hh>
# include <mlc/cmp.hh>

namespace oln {
  namespace morpho {

    /*!
    ** \brief Processing a geodesic erosion.
    **
    ** \param I1 Exact type of image marker.
    ** \param I2 Exact type of image mask.
    ** \param N Exact type of neighborhood.
    **
    ** \arg marker Image to work on.
    ** \arg mask Image used for geodesic erosion.
    ** \arg Ng Neighborhood to use.
    **
    ** Compute  the geodesic erosion  of marker  with respect  to the
    ** mask mask image using se as structural element. Soille p.158.
    **
    ** \pre Marker must be greater or equal than mask.
    **
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/morpho/opening.hh>
    ** #include <oln/morpho/geodesic_erosion.hh>
    ** #include <oln/level/compare.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   typedef oln::image2d<ntg::int_u8>	im_type;
    **
    **   im_type	im1(oln::load(IMG_IN "lena128.pgm"));
    **   im_type	im2 = oln::morpho::opening(im1, oln::win_c4p());
    **
    **   save(oln::morpho::geodesic_erosion(im1, im2, oln::neighb_c4()),
    **                                       IMG_OUT "oln_morpho_geodesic_erosion.pbm");
    **   return  0;
    ** }
    ** \endcode
    **
    ** \image html lena128_pgm.png
    ** \image latex lena128_pgm.png
    ** =>
    ** \image html oln_morpho_geodesic_erosion.png
    ** \image latex oln_morpho_geodesic_erosion.png
    **
    */
    template<class I1, class I2, class N>
    oln_concrete_type(I1)
      geodesic_erosion(const abstract::non_vectorial_image<I1> & marker,
		       const abstract::non_vectorial_image<I2> & mask,
		       const abstract::neighborhood<N>& Ng)
    {
      mlc::eq<I1::dim, I2::dim>::ensure();
      mlc::eq<I1::dim, N::dim>::ensure();
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(marker, mask));
      return arith::max_with_ret<oln_concrete_type(I1)>
	(erosion(marker, convert::ng_to_cse(Ng)),
	 mask);
    }

    namespace sure {
      /*!
      ** \brief Processing a geodesic erosion.
      **
      ** \param I1 Exact type of image marker.
      ** \param I2 Exact type of image mask.
      ** \param N Exact type of neighborhood.
      **
      ** \arg marker Image to work on.
      ** \arg mask Image used for geodesic erosion.
      ** \arg Ng Neighborhood to use.
      **
      ** Compute the  geodesic erosion of marker with  respect to the
      ** mask  image  using  se as  structural  element.  Soille
      ** p.156. Computation is performed  by hand (i.e without calling
      ** erosion).
      **
      ** \pre Marker must be greater or equal than mask.
      **
      ** \warning This version shouldn't  be use, since it exists only
      ** to have a reference algorithm.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/opening.hh>
      ** #include <oln/morpho/geodesic_erosion.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena128.pgm"));
      **   im_type	im2 = oln::morpho::opening(im1, oln::win_c4p());
      **
      **   save(oln::morpho::sure::geodesic_erosion(im1, im2, oln::neighb_c4()),
      **                                       IMG_OUT "oln_morpho_sure_geodesic_erosion.pbm");
      **   return  0;
      ** }
      ** \endcode
      **
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_sure_geodesic_erosion.png
      ** \image latex oln_morpho_sure_geodesic_erosion.png
      **
      */
      template<class I1, class I2, class N>
      oln_concrete_type(I1)
	geodesic_erosion(const abstract::non_vectorial_image<I1> & marker,
			 const abstract::non_vectorial_image<I2> & mask,
			 const abstract::neighborhood<N>& Ng)
      {
	mlc::eq<I1::dim, I2::dim>::ensure();
	mlc::eq<I1::dim, N::dim>::ensure();
	precondition(marker.size() == mask.size());
	precondition(level::is_greater_or_equal(marker, mask));

	oln_concrete_type(I1) output(marker.size());
	marker.border_adapt_copy(Ng.delta());
	oln_iter_type(I1) p(marker);
	for_all (p)
	  output[p] = ntg::max(morpho::min(marker, p, convert::ng_to_cse(Ng)), mask[p]);
	return output;
      }

    }// sure
  } // morpho
} // oln

#endif // OLENA_MORPHO_GEODESIC_EROSION_HH
