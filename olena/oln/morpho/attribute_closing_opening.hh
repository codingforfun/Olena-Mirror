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

#ifndef OLENA_ATTRIBUTE_CLOSING_OPENING_HH
# define OLENA_ATTRIBUTE_CLOSING_OPENING_HH

#include <oln/morpho/attribute_union_find.hh>

/*! \namespace oln
** \brief oln namespace
*/
namespace oln {
  /*! \namespace oln::morpho
  ** \brief oln::morpho namespace
  */
  namespace morpho {
    /*! \namespace oln::morpho::fast
    ** \brief oln::morpho::fast namespace
    */
    namespace fast {
      /*! \namespace oln::morpho::fast::tarjan
      ** \brief oln::morpho::fast::tarjan namespace
      */
      namespace tarjan {
	/*! \namespace oln::morpho::fast::tarjan::internal
	** \brief oln::morpho::fast::tarjan::internal namespace
	*/
	namespace internal {

	  template<class I, class N, class A>
	  oln_concrete_type(I)
	    attr_closing_(const abstract::non_vectorial_image<I>& input,
			  const abstract::neighborhood<N>& Ng,
			  const attr_lambda_type(A) &lambda,
			  const attr_env_type(A) &env = attr_env_type(A)())
	  {
	    typedef tarjan::tarjan_set<oln_concrete_type(I), A > tarjan_set_type;
	    tarjan_set_type attr_closing(input.exact(), env);
	    return attr_closing.template get_comptute<true>(lambda, Ng);
	  }

	  // attribute opening
	  template<class I, class N, class A>
	  oln_concrete_type(I)
	    attr_opening_(const abstract::non_vectorial_image<I>& input,
			  const abstract::neighborhood<N>& Ng,
			  const attr_lambda_type(A) &lambda,
			  const attr_env_type(A) &env = attr_env_type(A)())
	  {
	    typedef tarjan::tarjan_set<oln_concrete_type(I), A > tarjan_set_type;
	    tarjan_set_type attr_opening(input.exact(), env);
	    return attr_opening.template get_comptute<false>(lambda, Ng);
	  }

	} // !internal
      } // !tarjan
      // macro for some attribute opening/closing declarations
# define xxx_opening_decl(T) \
      template<class I, class N> \
      oln_concrete_type(I) \
	T##_opening(const abstract::non_vectorial_image<I>& input, \
		       const abstract::neighborhood<N>& Ng, \
		       const attr_lambda_type(T##_type<unsigned>) &lambda) \
      { \
	return tarjan::internal::attr_opening_<I, N, T##_type<unsigned> >(input, Ng, lambda); \
      }

# define xxx_closing_decl(T) \
      template<class I, class N> \
      oln_concrete_type(I) \
	T##_closing(const abstract::non_vectorial_image<I>& input, \
		       const abstract::neighborhood<N>& Ng, \
		       const attr_lambda_type(T##_type<unsigned>) &lambda) \
      { \
	return tarjan::internal::attr_closing_<I, N, T##_type<unsigned> >(input, Ng, lambda); \
      }


      // same as previous, but for attribute based on image types
# define xxx_opening_im_decl(T) \
      template<class I, class N> \
      oln_concrete_type(I) \
	T##_opening(const abstract::non_vectorial_image<I>& input, \
		       const abstract::neighborhood<N>& Ng, \
		       const attr_lambda_type(T##_type<I>) &lambda) \
      { \
	return tarjan::internal::attr_opening_<I, N, T##_type<I> >(input, Ng, lambda); \
      }

# define xxx_closing_im_decl(T) \
      template<class I, class N> \
      oln_concrete_type(I) \
	T##_closing(const abstract::non_vectorial_image<I>& input, \
		       const abstract::neighborhood<N>& Ng, \
		       const attr_lambda_type(T##_type<I>) &lambda) \
      { \
	return tarjan::internal::attr_closing_<I, N, T##_type<I> >(input, Ng, lambda); \
      }

      /*!
      ** \brief Perform a cardinal closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena256.pgm"));
      **   im1 = oln::morpho::fast::card_closing(im1, oln::neighb_c4(), 200);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_card_closing.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena256.png
      ** \image latex lena256.png
      ** =>
      ** \image html oln_morpho_fast_card_closing.png
      ** \image latex oln_morpho_fast_card_closing.png
      **
      */
      xxx_closing_decl(card);

      /*!
      ** \brief Perform a cardinal opening.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena256.pgm"));
      **   im1 = oln::morpho::fast::card_opening(im1, oln::neighb_c4(), 200);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_card_opening.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena256.png
      ** \image latex lena256.png
      ** =>
      ** \image html oln_morpho_fast_card_opening.png
      ** \image latex oln_morpho_fast_card_opening.png
      **
      */
      xxx_opening_decl(card);

      /*!
      ** \brief Perform an integral closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena256.pgm"));
      **   im1 = oln::morpho::fast::integral_closing(im1, oln::neighb_c4(), 200);
      **   oln::save(im1, IMG_OUT "olena_attribute_closing_opening_hh_integral_clo.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena256.png
      ** \image latex lena256.png
      ** =>
      ** \image html olena_attribute_closing_opening_hh_integral_clo.png
      ** \image latex olena_attribute_closing_opening_hh_integral_clo.png
      **
      */
      xxx_closing_decl(integral);

      /*!
      ** \brief Perform an integral opening.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena256.pgm"));
      **   im1 = oln::morpho::fast::integral_opening(im1, oln::neighb_c4(), 200);
      **   oln::save(im1, IMG_OUT "olena_attribute_closing_opening_hh_integral_op.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena256.png
      ** \image latex lena256.png
      ** =>
      ** \image html olena_attribute_closing_opening_hh_integral_op.png
      ** \image latex olena_attribute_closing_opening_hh_integral_op.png
      **
      */
      xxx_opening_decl(integral);

      /*!
      ** \brief Perform a height closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena256.pgm"));
      **   im1 = oln::morpho::fast::height_opening(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "olena_attribute_closing_opening_hh_height_op.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena256.png
      ** \image latex lena256.png
      ** =>
      ** \image html olena_attribute_closing_opening_hh_height_op.png
      ** \image latex olena_attribute_closing_opening_hh_height_op.png
      **
      */
      xxx_opening_decl(height);

      /*!
      ** \brief Perform a height closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena256.pgm"));
      **   im1 = oln::morpho::fast::height_closing(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "olena_attribute_closing_opening_hh_height_clo.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena256.png
      ** \image latex lena256.png
      ** =>
      ** \image html olena_attribute_closing_opening_hh_height_clo.png
      ** \image latex olena_attribute_closing_opening_hh_height_clo.png
      **
      */
      xxx_closing_decl(height);
      xxx_opening_decl(maxvalue);
      xxx_closing_decl(maxvalue);
      xxx_opening_decl(minvalue);
      xxx_closing_decl(minvalue);
      xxx_opening_im_decl(ball);
      xxx_closing_im_decl(ball);
      xxx_opening_im_decl(dist);
      xxx_closing_im_decl(dist);
      xxx_closing_im_decl(cube);
      xxx_opening_im_decl(cube);
      xxx_closing_im_decl(box);
      xxx_opening_im_decl(box);

    } // !fast
  } // !morpho
} // !oln

#endif
