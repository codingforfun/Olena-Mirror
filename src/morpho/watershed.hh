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

#ifndef OLENA_MORPHO_WATERSHED_HH
# define OLENA_MORPHO_WATERSHED_HH

# include "basics.hh"

namespace oln {

  namespace morpho {

    /*=processing watershed_seg
     * what: Segmented Watershed.
     * ns: morpho
     * tpl: class, DestValue, type of output labels
     * arg: const image<I>&, im, IN, image of levels
     * arg: const neighborhood<N>&, ng, IN, neighborhood to consider
     * ret: typename mute<I, DestValue>::ret
     * doc:
     *   Compute the segmented watershed for image @var{im} using
     *   neighborhood @var{ng}.
     *
     *   @code{watershed_seg} creates an ouput image whose values have
     *   type @var{DestValue} (which should be discrete).  In this output
     *   image, @code{DestValue::max()} indicates a watershed, and all
     *   basins are labeled using values from @code{DestValue::min()} to
     *   @code{DestValue::max() - 4} (the remaining values are used internally
     *   by the algorithm).
     *
     *   When there are more basins than @code{DestValue} can hold,
     *   wrapping occurs (i.e., the same label is used for several
     *   basin).
     * ref:
     *   This is based on the original algorithm
     *   presented by Vincent and Soille. (FIXME: ref?)
    =*/
    template<class DestValue, class I, class N>
    typename mute<I, DestValue>::ret
    watershed_seg(const image<I>& im_i, const neighborhood<N>& Ng);

    /*=processing watershed_con
     * what: Connected Watershed.
     * ns: morpho
     * tpl: class, DestValue, type of output labels
     * arg: const image<I>&, im, IN, image of levels
     * arg: const neighborhood<N>&, ng, IN, neighborhood to consider
     * ret: typename mute<I, DestValue>::ret
     * doc:
     *   Compute the connected watershed for image @var{im} using
     *   neighborhood @var{ng}.
     *
     *   @code{watershed_con} creates an ouput image whose values have
     *   type @var{DestValue} (which should be discrete).  In this output
     *   all basins are labeled using values from @code{DestValue::min()} to
     *   @code{DestValue::max() - 4} (the remaining values are used internally
     *   by the algorithm).
     *
     *   When there are more basins than @code{DestValue} can hold, wrapping
     *   occurs (i.e., the same label is used for several basin).  This is
     *   potentially harmful, because if two connected basins are labeled
     *   with the same value they will appear as one basin.
     * ref:
     *   This is based on the original algorithm
     *   presented by Vincent and Soille, but modified to not output
     *   watersheds.
    =*/
    template<class DestValue, class I, class N>
    typename mute<I, DestValue>::ret
    watershed_con(const image<I>& im_i, const neighborhood<N>& Ng);

    /*=processing watershed_seg_or
     * what: Segmented Watershed with user-supplied starting points.
     * ns: morpho
     * arg: const image<I1>&, levels, IN, image of levels
     * arg: image<I2>&, markers, INOUT, image of markers
     * arg: const neighborhood<N>&, ng, IN, neighborhood to consider
     * ret: Concrete(I2)&
     * doc:
     *   Compute a segmented watershed for image @var{levels} using
     *   neighborhood @var{ng}, and @var{markers} as starting point for
     *   the flooding algorithm.
     *
     *   @var{markers} is an image of the same size as @var{levels}
     *   and containing discrete values indicating label associated to
     *   each basin.  On input, fill @var{markers} with
     *   @code{Value(I2)::min()} (this is the @emph{unknown} label)
     *   and mark the starting points or regions
     *   (usually these are minima in @var{levels}) using a value
     *   between @code{Value(I2)::min()+1} and @code{Value(I2)::max()-1}.
     *
     *   @code{watershed_seg_or} will flood @var{levels} from these
     *   non-@emph{unknown} starting points, labeling basins using
     *   the value you assigned to them, and markining watershed lines
     *   with @code{Value(I2)::max()}.  @var{markers} should not contains
     *   any @code{Value(I2)::min()} value on output.
     * ref:
     *   This is based on the original algorithm
     *   presented by D'Ornellas et al. (FIXME: ref?)
    =*/
    template<class I1, class I2, class N>
    Concrete(I2)&
    watershed_seg_or(const image<I1>& D,
		     image<I2>& M,
		     const neighborhood<N>& Ng);

  } // end of morpho

} // end of oln

#include "watershed.hxx"

#endif // ! OLENA_MORPHO_WATERSHED_HH
