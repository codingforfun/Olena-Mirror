// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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
  template <class DestType, class _I>
  typename mute<_I, DestType>::ret
  _create_minima_image_from_bin(const image<_I>& _input)
  {
    Exact_cref(I, input);
    Iter(I) p(input);
    typename mute<_I, DestType>::ret output(input.size());
    for_all (p)
      // FIXME: min() and max() should be inf() and sup()
      // once these functions exist.  Otherwise it doesn't
      // work on float.
      output[p] = (input[p] ?
		   optraits<DestType>::min() :
		   optraits<DestType>::max());
    return output;
  }


  template <class _I>
  typename mute<_I, bin>::ret
  _ima_to_bin(const image<_I>& _input)
  {
    Exact_cref(I, input);
    Iter(I) p(input);
    typename mute<_I, bin>::ret output(input.size());
    for_all (p)
      output[p] = (input[p] ? true : false);
    return output;
  }

} // internal



// Minima Imposition



/*=processing sure_minima_imposition
 * ns: morpho
 * what: Minima Imposition.
 * arg: const image<_I1>&, _input, IN, input image
 * arg: const image<_I2>&, _minima_map, IN, bin image
 * arg: const neighborhood<_N>& _Ng, IN, neighborhood
 * ret: Concrete(_I)
 * doc:
 * Impose minima defined by \var{_minima_map} on \var{input}
 * using \var{_Ng} as neighborhood. \var{_minima_map} must
 * be a bin image (true for a minimum, false for a non minimum).
 * Soille p.172.
 * see: morpho::sure_geodesic_reconstruction_erosion
 * ex:
 * $ image2d<int_u8> light = load("lena.pgm");
 * $ image2d<bin> minima = load("map.pbm");
 * $ save(morpho::sure_minima_imposition(light, minima, neighb_c4()), "out.pgm");
 * exi: lena.pgm map.pbm
 * exo: out.pgm
 =*/
template<class _I, class _I2, class _N>
Concrete(_I) minima_imposition(const image<_I>& _input,
			       const image<_I2>& _minima_map,
			       const neighborhood<_N>& _Ng)
{
  Exact_cref(I, input);
  Exact_cref(I2, minima_map);
  Exact_cref(N, Ng);
  meta::eq<I::dim, I2::dim>::ensure();
  meta::eq<I::dim, N::dim>::ensure();
  precondition(input.size() == minima_map.size());
  Concrete(I) mm =
    internal::_create_minima_image_from_bin<Value(I)>(minima_map);
  return geodesic_reconstruction_erosion(mm,
					 arith::min(arith::plus_cst(input, Value(_I)(1)),
						    (arith::plus_cst(mm, Value(_I)(0)))), Ng);
}




// Regional minima


/*=processing sure_regional_minima
 * ns: morpho
 * what: Regional minima.
 * arg: const image<I1>&, input, IN, input image
 * arg: const struct_elt<E>&, se, IN, structural element
 * ret: typename mute<_I, bin>::ret
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
template<class _I, class _N>
typename mute<_I, bin>::ret regional_minima(const image<_I>& _input,
					    const neighborhood<_N>& _Ng)
{
  Exact_cref(I, input);
  Exact_cref(N, Ng);
  meta::eq<I::dim, N::dim>::ensure();
  return
    internal::_ima_to_bin(arith::minus(convert::force<Value(I)>(),
				       geodesic_reconstruction_erosion
				       (arith::plus_cst(input, Value(I) (1)), input, Ng),
				       input));
}
