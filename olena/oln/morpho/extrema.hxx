// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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


namespace internal {
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



/*=processing sure_minima_imposition
 * ns: morpho
 * what: Minima Imposition.
 * arg: const abstract::non_vectorial_image<I1>&, input, IN, input image
 * arg: const abstract::non_vectorial_image<I2>&, minima_map, IN, bin image
 * arg: const abstract::neighborhood<N>& Ng, IN, neighborhood
 * ret:oln_concrete_type(I)
 * doc:
 * Impose minima defined by \var{minima_map} on \var{input}
 * using \var{Ng} as neighborhood. \var{minima_map} must
 * be a bin image (true for a minimum, false for a non minimum).
 * Soille p.172.
 * see: morpho::sure_geodesic_reconstruction_erosion
 * ex:
 * $ image2d<int_u8> light = load("lena.pgm");
 * $ image2d<ntg::bin> minima = load("map.pbm");
 * $ save(morpho::sure_minima_imposition(light, minima, neighb_c4()), "out.pgm");
 * exi: lena.pgm map.pbm
 * exo: out.pgm
 =*/
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


/*=processing sure_regional_minima
 * ns: morpho
 * what: Regional minima.
 * arg: const abstract::non_vectorial_image<I1>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se, IN, structural element
 * ret: typename mute<I, ntg::bin>::ret
 * doc:
 * Extract regional minima of \var{input}
 * using \var{Ng}
 *   as neighborhhod. Soille p.169.  The algorithm uses
 * sure_geodesic_reconstruction_erosion.
 * see: morpho::sure_geodesic_reconstruction_erosion
 * ex:
 * $ image2d<int_u8> light = load("lena.pgm");
 * $ save(morpho::sure_regional_minima(lena,neighb_c4()), "out.pgm");
 * exi: lena.pgm
 * exo: out.pgm
 =*/
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
