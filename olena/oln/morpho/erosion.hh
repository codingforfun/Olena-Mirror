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

#ifndef OLENA_MORPHO_EROSION_HH
# define OLENA_MORPHO_EROSION_HH

# include <oln/basics.hh>
# include <oln/morpho/stat.hh>
# include <oln/morpho/fast_morpho.hh>
# include <mlc/cmp.hh>

namespace oln {

  namespace morpho {

    /*=processing erosion
     * ns: morpho, morpho::fast
     * what: Morphological erosion.
     * arg: const abstract::non_vectorial_image<I>&, input, IN, input image
     * arg: const abstract::struct_elt<E>&, se, IN, structural element
     * ret: Concrete(I)
     * doc:
     *   Compute the morphological erosion of \var{input} using \var{se}
     *   as structural element.
     *
     *   On grey-scale images, each point is replaced by the minimum value
     *   of its neighbors, as indicated by \var{se}.  On binary images,
     *   a logical \code{and} is performed between neighbors.
     *   The \code{morpho::fast} version of this function use a different
     *   algorithm: an
     *
     *   histogram of the value of the neighborhood indicated by
     *   \var{se} is updated while iterating over all point of the
     *   image.  Doing so is more efficient  when the
     *   structural element is large.
     * see: morpho::n_erosion
     * see: morpho::dilation
     * ex:
     * $ image2d<ntg::bin> im = load("object.pbm");
     * $ save(morpho::erosion(im, win_c8p()), "out.pbm");
     * exi: object.pbm
     * exo: out.pbm
    =*/
    template<class I, class E>
    Concrete(I) erosion(const abstract::non_vectorial_image<I>& input, 
			const abstract::struct_elt<E>& se)
    {
      mlc::eq<I::dim, E::dim>::ensure();
      Concrete(I) output(input.size());
      se.delta();
      input.border_adapt_copy(se.delta());
      Iter(I) p(input);
      for_all (p)
	output[p] = morpho::min(input, p, se);
      return output;
    }

    /*=processing n_erosion
     * ns: morpho
     * what: Morphological erosion itered n times.
     * arg: const abstract::non_vectorial_image<I>&, input, IN, input image
     * arg: const abstract::struct_elt<E>&, se, IN, structural element
     * arg: unsigned, n, IN, number of iterations
     * ret: Concrete(I)
     * doc:
     *   Apply \code{morpho::erosion} \var{n} times.
     * see: morpho::erosion
     * see: morpho::n_dilation
    =*/

    template<class I, class E>
    Concrete(I) n_erosion(const abstract::non_vectorial_image<I> & input,
			  const abstract::struct_elt<E>& se,
			  unsigned n)
    {
      //mlc::eq<I::dim, E::dim>::ensure();
      precondition(n > 0);
      Concrete(I) output = input.clone();
      for (unsigned i = 0; i < n; ++i)
	{
	  Concrete(I) work = erosion(output, se);
	  output = work;
 	}
      return output;
    }

    namespace fast {
      template<class I, class E>
      Concrete(I) erosion(const abstract::non_vectorial_image<I>& input, 
			  const abstract::struct_elt<E>& se)
      {
        return fast_morpho<I, E, utils::histogram_min>(input, se);
      }
    }

  } // end of morpho

} // end of oln


#endif // ! OLENA_MORPHO_EROSION_HH
