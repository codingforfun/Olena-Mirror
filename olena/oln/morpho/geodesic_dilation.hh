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

#ifndef OLENA_MORPHO_GEODESIC_DILATION_HH
# define OLENA_MORPHO_GEODESIC_DILATION_HH

# include <oln/basics.hh>
# include <oln/morpho/stat.hh>
# include <oln/morpho/dilation.hh>
# include <oln/convert/conversion_ng_se.hh>
# include <oln/level/compare.hh>
# include <oln/arith/ops.hh>
# include <ntg/global_ops_traits.hh>
# include <mlc/cmp.hh>

namespace oln {
  namespace morpho {

    /*=processing geodesic_dilation
     * ns: morpho
     * what: Geodesic dilation.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const image<I2>&, mask, IN, mask image
     * arg: const struct_elt<E>&, se, IN, structural element
     * ret: Concrete(I1)
     * doc:
     *  Compute the geodesic dilation of \var{marker} with respect
     * to the mask \var{mask} image using \var{se}
     *   as structural element. Soille p.156.
     * Note mask must be greater or equal than marker.
     * see: morpho::simple_geodesic_dilation
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ image2d<int_u8> dark = load("dark.pgm");
     * $ save(morpho::geodesic_dilation(dark, light, win_c8p()), "out.pgm");
     * exi: light.pgm dark.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/

    template<class I1_, class I2_, class N_>
    Concrete(I1_) geodesic_dilation(const image<I1_> & _marker,
				    const image<I2_> & _mask,
				    const neighborhood<N_>& _Ng)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(N, Ng);
      meta::eq<I1_::dim, I2_::dim>::ensure();
      meta::eq<I1::dim, N::dim>::ensure();
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(mask, marker));
      return arith::min(dilation(marker, convert::ng_to_cse(Ng)), mask);
    }

    namespace sure {
      /*=processing simple_geodesic_dilation
       * ns: morpho
       * what: Geodesic dilation.
       * arg: const image<I1>&, marker, IN, marker image
       * arg: const image<I2>&, mask, IN, mask image
       * arg: const struct_elt<E>&, se, IN, structural element
       * ret: Concrete(I1)
       * doc:
       *  Compute the geodesic dilation of \var{marker} with respect
       * to the mask \var{mask} image using \var{se}
       *   as structural element. Soille p.156. Computation is
       * performed by hand (i.e without calling dilation).
       * Note mask must be greater or equal than marker.
       * see: morpho::sure_geodesic_dilation
       * ex:
       * $ image2d<int_u8> light = load("light.pgm");
       * $ image2d<int_u8> dark = load("dark.pgm");
       * $ save(morpho::simple_geodesic_dilation(dark, light,
       * $                                       win_c8p()), "out.pgm");
       * exi: light.pgm dark.pgm
       * exo: out.pgm
       * wontcompile: fixme
       =*/
      template<class I1_, class I2_, class N_>
      Concrete(I1_) geodesic_dilation(const image<I1_> & _marker,
				      const image<I2_> & _mask,
				      const neighborhood<N_>& _Ng)
      {
	Exact_cref(I1, marker);
	Exact_cref(I2, mask);
	Exact_cref(N, Ng);
	meta::eq<I1_::dim, I2_::dim>::ensure();
	meta::eq<I1::dim, N::dim>::ensure();
	precondition(marker.size() == mask.size());
	precondition(level::is_greater_or_equal(mask, marker));

	Concrete(I1) output(marker.size());
	border::adapt_copy(marker, Ng.delta());
	Iter(I1) p(marker);
	for_all (p)
	  output[p] = min(morpho::max(marker, p, convert::ng_to_cse(Ng)), mask[p]);
	return output;
      }

    } // sure
  } // morpho
} // oln

#endif // OLENA_MORPHO_GEODESIC_DILATION_HH
