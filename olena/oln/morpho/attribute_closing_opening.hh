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

#ifndef OLENA_ATTRIBUTE_CLOSING_OPENING_HH
# define OLENA_ATTRIBUTE_CLOSING_OPENING_HH

#include <oln/morpho/attribute_union_find.hh>

namespace oln {
  namespace morpho {
    namespace tarjan {


      /*=processing area_closing
       * ns: morpho
       * what: Area closing
       * arg: const image<I_>&, _input, IN, input image
       * arg: const neighborhood<N_>&, se, IN, neighborhood to consider
       * arg: unsigned int, area, IN, area
       * ret: Concrete(I_)
       * doc:
       * Compute an area closing using union/find algorithm.
       * See A. Meijster and M. Wilkinson. A Comparison of Algorithms For Connected
       * Set Openings and Closings. PAMI 24(2), p484--494
       * see: morpho::simple_geodesic_dilation
       * ex:
       * $ image2d<int_u8> im = load("lena256.pgm");
       * $ save(morpho::tarjan::area_closing(im, neighb_c4(),500), "out.pgm");
       * exi: lena256.pgm
       * exo: out.pgm
       =*/
      template<class I, class N>
      Concrete(I) area_closing(const image<I>& input,
			       const neighborhood<N>& Ng,
			       const unsigned int area)
      {
	typedef T_attribute<unsigned int> area_t;

	typedef tarjan::tarjan_set<Concrete(I_), area_t > tarjan_set_t;
	tarjan_set_t area_closing(input);
	return area_closing.get_comptute(area_t(area) , Ng, true);
      }


      /*=processing area_opening
       * ns: morpho
       * what: Area opening
       * arg: const image<I_>&, _input, IN, input image
       * arg: const neighborhood<N_>&, se, IN, neighborhood to consider
       * arg: unsigned int, area, IN, area
       * ret: Concrete(I_)
       * doc:
       * Compute an area opening using union/find algorithm.
       * See A. Meijster and M. Wilkinson. A Comparison of Algorithms For Connected
       * Set Openings and Closings. PAMI 24(2), p484--494
       * see: morpho::simple_geodesic_dilation
       * ex:
       * $ image2d<int_u8> im = load("lena256.pgm");
       * $ save(morpho::tarjan::area_opening(im, neighb_c4(),500), "out.pgm");
       * exi: lena256.pgm
       * exo: out.pgm
       =*/
      template<class I, class N>
      Concrete(I) area_opening(const image<I>& input,
			       const neighborhood<N>& Ng,
			       const unsigned int area)
      {
	typedef T_attribute<unsigned int> area_t;

	typedef tarjan::tarjan_set<Concrete(I_), T_attribute<unsigned int> > tarjan_set_t;
	tarjan_set_t area_closing(input);
	return area_closing.get_comptute(area_t(area) , Ng, false);
      }


    }
  }
}

#endif
