// Copyright 2001  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_EXTREMA_HH
# define OLENA_MORPHO_EXTREMA_HH

# include "basics.hh"
# include "morpho/reconstruction.hh"

// FIXME: should _se be neighborhoods?

namespace oln {
  namespace morpho {

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
	  output[p] = (input[p] ? DestType::min() : DestType::max());
	return output;
      }
    }

    // Minima Imposition
    template<class _I, class _I2, class _E>
    Concrete(_I) sure_minima_imposition(const image<_I>& _input,
					const image<_I2>& _minima_map,
					const struct_elt<_E>& _se)
    {
      Exact_cref(I, input);
      Exact_cref(I2, minima_map);
      Exact_cref(E, se);
      precondition(input.size() == minima_map.size());
      Concrete(I) mm =
	internal::_create_minima_image_from_bin<Value(I)>(minima_map);
      // FIXME: p172. Soille says marker = 0 || T::max(). What about float ?
      // This is definitely not generic !! the highest value of input should
      // be fine.
      // 0 means minimum else set value to infty
      // We suppose we have such an image in minima_map...
      //       return hybrid_geodesic_reconstruction_erosion(minima_map,
      //  						    input,
      //  						    se);

      //FIXME: min must be authorized even if types are not the same.
      return sure_geodesic_reconstruction_erosion(mm,
						  arith::min(input, mm), se);
    }

    template<class _I, class _I2, class _E>
    Concrete(_I) sequential_minima_imposition(const image<_I>& _input,
					      const image<_I2>& _minima_map,
					      const struct_elt<_E>& _se)
    {
      Exact_cref(I, input);
      Exact_cref(I2, minima_map);
      Exact_cref(E, se);
      precondition(input.size() == minima_map.size());
      Concrete(I) mm =
	internal::_create_minima_image_from_bin<Value(I)>(minima_map);
      return sequential_geodesic_reconstruction_erosion(mm,
							arith::min(input, mm),
							se);
    }

    template<class _I, class _I2, class _E>
    Concrete(_I) hybrid_minima_imposition(const image<_I>& _input,
					  const image<_I2>& _minima_map,
					  const struct_elt<_E>& _se)
    {
      Exact_cref(I, input);
      Exact_cref(I2, minima_map);
      Exact_cref(E, se);
      precondition(input.size() == minima_map.size());
      Concrete(I) mm =
	internal::_create_minima_image_from_bin<Value(I)>(minima_map);
      return hybrid_geodesic_reconstruction_erosion(mm, arith::min(input, mm),
						    se);

    }


    // Regional minima

    template<class _I, class _E>
    Concrete(_I) sure_regional_minima(const image<_I>& _input,
				      const struct_elt<_E>& _se)
    {
      Exact_cref(I, input);
      Exact_cref(E, se);
      return
	arith::minus(convert::force<Value(I)>(),
		     sure_geodesic_reconstruction_erosion
		     (arith::plus_cst(input, Value(I) (1)), input, se),
		     input);
    }

    template<class _I, class _E>
    Concrete(_I) sequential_regional_minima(const image<_I>& _input,
					    const struct_elt<_E>& _se)
    {
      Exact_cref(I, input);
      Exact_cref(E, se);
      return
	arith::minus(convert::force<Value(I)>(),
		     sequential_geodesic_reconstruction_erosion
		     (arith::plus_cst(input, Value(I) (1)), input, se),
		     input);
    }

    template<class _I, class _E>
    Concrete(_I) hybrid_regional_minima(const image<_I>& _input,
					const struct_elt<_E>& _se)
    {
      Exact_cref(I, input);
      Exact_cref(E, se);
      return
	arith::minus(convert::force<Value(I)>(),
		     hybrid_geodesic_reconstruction_erosion
		     (arith::plus_cst(input, Value(I) (1)), input, se),
		     input);
    }

  } // morpho
} // oln


#endif // OLENA_MORPHO_EXTREMA_HH
