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

/*!
** \brief Internal purpose only.
*/
namespace internal {

  /*!
  ** \brief Create extremum image from another one.
  **
  ** \param DestType Type of data in the wanted image.
  ** \param I Exact type of the input image.
  **
  ** \arg input Input image.
  */
  template <class DestType, class I>
  typename mute<I, DestType>::ret
  create_minima_image_from_bin_(const abstract::non_vectorial_image<I>& input)
  {
    oln_iter_type(I) p(input);
    typename mute<I, DestType>::ret output(input.size());
    for_all (p)
      // FIXME: min() and max() should be inf() and sup()
      // once these functions exist.  Otherwise it doesn't
      // work on float.
      output[p] = (input[p] ?
		   ntg_min_val(DestType) :
		   ntg_max_val(DestType));
    return output;
  }

  /*!
  ** \brief Create binary image from another one.
  **
  ** \param I Exact type of the image.
  **
  ** \arg input Input image.
  */
  template <class I>
  typename mute<I, ntg::bin>::ret
  ima_to_bin_(const abstract::non_vectorial_image<I>& input)
  {
    oln_iter_type(I) p(input);
    typename mute<I, ntg::bin>::ret output(input.size());
    for_all (p)
      output[p] = (input[p] ? true : false);
    return output;
  }

} // internal



// Minima Imposition

/*!
** \brief Perform a minima imposition.
**
** Impose  minima   defined  by  minima_map  on  input   using  Ng  as
** neighborhood. minima_map must  be a bin image (true  for a minimum,
** false for a non minimum).  Soille p.172.
**
** \param I Exact type of the first image.
** \param I2 Exact type of the second image.
** \param N Exact type of the neighborhood.
**
** \arg input Input image.
** \arg minima_map Minima map image.
** \arg Ng Neighborhood to use.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/extrema.hh>
** #include <ntg/all.hh>
** #include <iostream>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**   typedef oln::image2d<ntg::bin>	bin_im_type;
**
**   im_type		light(oln::load(IMG_IN "lena.pgm"));
**   bin_im_type	minima(oln::load(IMG_IN "map.pbm"));
**
**   oln::save(oln::morpho::sequential::minima_imposition(light,
**							  minima,
**							  oln::neighb_c4()),
**             IMG_OUT "oln_morpho_sequential_minima_imposition.pgm");
** }
** \endcode
** \image html lena_pgm.png
** \image latex lena_pgm.png
** and
** \image html map_pbm.png
** \image latex map_pbm.png
** =>
** \image html oln_morpho_sequential_minima_imposition.png
** \image latex oln_morpho_sequential_minima_imposition.png
**
*/
template<class I, class I2, class N>
oln_concrete_type(I)
  minima_imposition(const abstract::non_vectorial_image<I>& input,
		    const abstract::non_vectorial_image<I2>& minima_map,
		    const abstract::neighborhood<N>& Ng)
{
  mlc::eq<I::dim, I2::dim>::ensure();
  mlc::eq<I::dim, N::dim>::ensure();
  precondition(input.size() == minima_map.size());
  oln_concrete_type(I) mm =
    internal::create_minima_image_from_bin_<oln_value_type(I)>(minima_map);
  return geodesic_reconstruction_erosion(mm,
					 arith::min(arith::plus_cst(input,oln_value_type(I)(1)),
						    (arith::plus_cst(mm,oln_value_type(I)(0)))), Ng);
}




// Regional minima

/*!
** \brief Extract regional minima.
**
** \param I Exact type of input image.
** \param Exact type of neighborhood.
**
** \arg input Input image.
** \arg Ng Neighborhood to use.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/extrema.hh>
** #include <ntg/all.hh>
** #include <iostream>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type		im(oln::load(IMG_IN "lena.pgm"));
**
**   oln::save(oln::morpho::sequential::regional_minima(im, oln::neighb_c4()),
**             IMG_OUT "oln_morpho_sequential_regional_minima.pgm");
** }
** \endcode
** \image html lena_pgm.png
** \image latex lena_pgm.png
** =>
** \image html oln_morpho_sequential_regional_minima.png
** \image latex oln_morpho_sequential_regional_minima.png
**
*/
template<class I, class N>
typename mute<I, ntg::bin>::ret
regional_minima(const abstract::non_vectorial_image<I>& input,
		const abstract::neighborhood<N>& Ng)
{
  mlc::eq<I::dim, N::dim>::ensure();
  return
    internal::ima_to_bin_(arith::minus(convert::force<oln_value_type(I)>(),
				       geodesic_reconstruction_erosion
				       (arith::plus_cst(input,oln_value_type(I) (1)), input, Ng),
				       input));
}
