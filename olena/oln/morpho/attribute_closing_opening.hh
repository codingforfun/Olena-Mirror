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

      namespace internal {

	// attribute closing
	template<class I, class N, class A>
	oln_concrete_type(I)
	  attr_closing_(const abstract::non_vectorial_image<I>& input,
			const abstract::neighborhood<N>& Ng,
			const attr_lambda_type(A) &lambda)
	{
	  typedef tarjan::tarjan_set<oln_concrete_type(I), A > tarjan_set_type;
	  tarjan_set_type area_closing(input.exact(), attr_env_type(A)());
	  return area_closing.get_comptute(lambda, Ng, true);
	}

	// attribute opening
	template<class I, class N, class A>
	oln_concrete_type(I)
	  attr_opening_(const abstract::non_vectorial_image<I>& input,
			const abstract::neighborhood<N>& Ng,
			const attr_lambda_type(A) &lambda)
	{
	  typedef tarjan::tarjan_set<oln_concrete_type(I), A > tarjan_set_type;
	  tarjan_set_type area_opening(input.exact(), attr_env_type(A)());
	  return area_opening.get_comptute(lambda , Ng, false);
	}

      } // !internal

// macro for some attribute opening/closing declarations
# define xxx_opening_decl(T) \
      template<class I, class N> \
      oln_concrete_type(I) \
	T##_opening(const abstract::non_vectorial_image<I>& input, \
		       const abstract::neighborhood<N>& Ng, \
		       const attr_lambda_type(T##_type<unsigned>) &lambda) \
      { \
	return internal::attr_opening_<I, N, T##_type<unsigned> >(input, Ng, lambda); \
      }

# define xxx_closing_decl(T) \
      template<class I, class N> \
      oln_concrete_type(I) \
	T##_closing(const abstract::non_vectorial_image<I>& input, \
		       const abstract::neighborhood<N>& Ng, \
		       const attr_lambda_type(T##_type<unsigned>) &lambda) \
      { \
	return internal::attr_closing_<I, N, T##_type<unsigned> >(input, Ng, lambda); \
      }


      // same as previous, but for attribute based on image types
# define xxx_opening_im_decl(T) \
      template<class I, class N> \
      oln_concrete_type(I) \
	T##_opening(const abstract::non_vectorial_image<I>& input, \
		       const abstract::neighborhood<N>& Ng, \
		       const attr_lambda_type(T##_type<I>) &lambda) \
      { \
	return internal::attr_opening_<I, N, T##_type<I> >(input, Ng, lambda); \
      }

# define xxx_closing_im_decl(T) \
      template<class I, class N> \
      oln_concrete_type(I) \
	T##_closing(const abstract::non_vectorial_image<I>& input, \
		       const abstract::neighborhood<N>& Ng, \
		       const attr_lambda_type(T##_type<I>) &lambda) \
      { \
	return internal::attr_closing_<I, N, T##_type<I> >(input, Ng, lambda); \
      }

      /*=processing area_closing
       * ns: morpho
       * what: Area closing
       * arg: const abstract::non_vectorial_image<I>&, input, IN, input image
       * arg: const abstract::neighborhood<N>&, se, IN, neighborhood to consider
       * arg: unsigned int, area, IN, area
       * ret:oln_concrete_type(I)
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
      xxx_closing_decl(area)

      /*=processing area_opening
       * ns: morpho
       * what: Area opening
       * arg: const abstract::non_vectorial_image<I>&, input, IN, input image
       * arg: const abstract::neighborhood<N>&, se, IN, neighborhood to consider
       * arg: unsigned int, area, IN, area
       * ret:oln_concrete_type(I)
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
	xxx_opening_decl(area)

	xxx_opening_decl(volume)
	xxx_closing_decl(volume)
	xxx_opening_decl(height)
	xxx_closing_decl(height)
	xxx_opening_decl(maxvalue)
	xxx_closing_decl(maxvalue)
	xxx_opening_decl(minvalue)
	xxx_closing_decl(minvalue)
	xxx_opening_im_decl(disk)
	xxx_closing_im_decl(disk)
	xxx_opening_im_decl(dist)
	xxx_closing_im_decl(dist)
	xxx_closing_im_decl(square)
	xxx_opening_im_decl(square)
	xxx_closing_im_decl(rectangle)
	xxx_opening_im_decl(rectangle)

    }
  }
}

#endif
