// Copyright (C) 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_ATTRIBUTE_CLOSING_OPENING_MAP
# define OLENA_MORPHO_ATTRIBUTE_CLOSING_OPENING_MAP
# include <oln/morpho/attributes.hh>
# include <oln/level/fill.hh>
# include <oln/basics2d.hh>
# include <ntg/int.hh>
# include <oln/utils/histogram.hh>
# include <oln/utils/special_points.hh>
# include <oln/morpho/attributes.hh>

# include <iterator>
# include <vector>
# include <map>
# include <algorithm>
# include <utility>


namespace oln
{
  namespace morpho
  {
    /*!
    ** \brief oln::morpho::slow namespace
    **
    ** In this namespace you will find algorithm slower than those you
    ** can find in oln::morpho::fast, but with less memory needs.
    */
    namespace slow
    {
      /*! Attribute closing using map. Smaller memory usage, but
      **  slower computation than the attribute_closing_opening
      **
      ** See "Fast morphological attribute operations using Tarjan's union-find
      ** algorithm" by Michael H. F. Wilkinson and Jos B. T. M. Roerdink
      **
      ** \param I Image exact type.
      ** \param D Attribute exact type.
      ** \param Env Type of environment.
      */
      template <class I, class D, class Env = morpho::NullEnv>
      struct f_tarjan_map
      {
      public:
	typedef abstract::image<I>			input_type; ///< Image type (abstract).
	typedef oln_concrete_type(I)			img_type; ///< Image type (concrete).

	typedef oln_point_type(input_type)		point_type; ///< Associated point type.
	typedef oln_value_type(input_type)		value_type; ///< Value type fo the image.
	typedef typename mute<input_type, point_type>::ret parent_type; ///< Image of points.
	typedef typename mute<input_type, bool>::ret	is_proc_type; ///< Image of bool.
	typedef attr_lambda_type(D)			lambda_type; ///< dTthreshold type.

	// e.g.,
	// when I is image2d<int_u8> and D is area_type, we've got:
	// +--------------+------------------+
	// | input_type   | image2d<int_u8>  |
	// | point_type   | point2d          |
	// | parent_type  | image2d<point2d> |
	// | is_proc_type | image2d<bool>    |
	// | lambda_type  | int              |
	// +--------------+------------------+

	/*!
	** \brief Perform an attribute opening/closing.
	**
	** \param N Exact type of neighborhood
	**
	** \arg is_closing Choose between closing and opening.
	** \arg input Input image.
	** \arg ng Neighborhood to use.
	** \arg lambda Threshold.
	** \arg env Environment.
	*/
	template <class N>
	f_tarjan_map(bool is_closing,
		     const input_type& input,
		     const abstract::neighborhood<N>& ng,
		     const lambda_type& lambda,
		     const Env & env = Env());
	/*!
	** \brief Return the result of the opening/closing.
	*/
	oln_concrete_type(I)
	  res()
	{
	  return output;
	}
      protected:

	/*!
	** \brief Return the value of an inactive point.
	*/
	static const point_type
	inactive();

	/*!
	** \brief Return the value of an active point.
	*/
	static const point_type
	active();

	/*!
	** \brief Make a new component from a point.
	** \arg x Root of the component.
	*/
	void
	make_set(const point_type& x);

	/*!
	** \brief link two components
	** \arg x A point of the first component.
	** \arg y A point of the second component.
	*/
	void
	link(const point_type& x, const point_type& y);

	/*!
	** \brief find the root of a component.
	** \arg x A point of the component.
	*/
	point_type
	find_root(const point_type& x);

	/*!
	** \brief check if two components are equivalent.
	*/
	bool
	equiv(const point_type& x, const point_type& y) const;


	/*!
	** \brief link two components if they have to be linked
	** \arg n A point of the first component.
	** \arg p A point of the second component.
	*/
	void
	do_union(const point_type& n, const point_type& p);

	const bool							is_closing; ///< Do you want a closing or an openng ?
	const input_type&						input; ///< Input image.
	lambda_type							lambda; ///<  Trheshold.
	parent_type							parent; ///< Give a parent of a point.
	is_proc_type							is_proc; ///< Tell if a point has already been preceded
	img_type							output; ///< Image to store the result
	std::map<point_type, D, oln::internal::default_less<point_type> > auxdata; ///< Map to store attributes.
	Env								env; ///< The envronment.
      };


      /*! Attribute closing using map. Smaller memory usage, but
      **  slower computation than the attribute_closing_opening
      **
      ** See "Fast morphological attribute operations using Tarjan's union-find
      ** algorithm" by Michael H. F. Wilkinson and Jos B. T. M. Roerdink
      */
      template <class D, class I, class N>
      oln_concrete_type(I)
	tarjan_map(bool is_closing,
		   const abstract::image<I>& input,
		   const abstract::neighborhood<N>& ng,
		   const attr_lambda_type(D)& lambda,
		   const attr_env_type(D)& env/* = Env()*/)
      {
	oln::morpho::slow::f_tarjan_map<I, D, attr_env_type(D) > t(is_closing,
								   input,
								   ng,
								   lambda,
								   env);
	return t.res();
      }

// Some macro to declare friendly function for different attributes
// opening/closing
# define xxx_opening_map_decl(T)						\
template <class I, class N>							\
oln_concrete_type(I)								\
T##_opening(const abstract::image<I>& input,					\
	    const abstract::neighborhood<N>& ng,				\
	    const attr_lambda_type(T##_type<>)& lambda)				\
    {										\
      return tarjan_map<T##_type<>, I, N>(false, input, ng, lambda,		\
						    attr_env_type(T##_type<>)()); \
    }

      // same but attribute take care of the image type
# define xxx_opening_im_map_decl(T)						\
template <class I, class N>							\
oln_concrete_type(I)								\
T##_opening(const abstract::image<I>& input,					\
	    const abstract::neighborhood<N>& ng,				\
	    const attr_lambda_type(T##_type<I>)& lambda)			\
    {										\
      return tarjan_map<T##_type<I>, I, N>(false, input, ng, lambda,		\
						    attr_env_type(T##_type<I>)()); \
    }

      //return input;
      //      return tarjan_map<T##_type, I, N>(false, input, ng, lambda, attr_env_type(T##_type<>)());
# define xxx_closing_map_decl(T)						\
template <class I, class N>							\
oln_concrete_type(I)								\
T##_closing(const abstract::image<I>& input,					\
	    const abstract::neighborhood<N>& ng,				\
	    const attr_lambda_type(T##_type<>)& lambda)				\
    {										\
      return tarjan_map<T##_type<>, I, N>(true, input, ng, lambda, attr_env_type(T##_type<>)()); \
    }

      // same but attribute take care of the image type
# define xxx_closing_im_map_decl(T)						\
template <class I, class N>							\
oln_concrete_type(I)								\
T##_closing(const abstract::image<I>& input,					\
	    const abstract::neighborhood<N>& ng,				\
	    const attr_lambda_type(T##_type<I>)& lambda)				\
    {										\
      return tarjan_map<T##_type<I>, I, N>(true, input, ng, lambda, attr_env_type(T##_type<I>)()); \
    }

      /*!
      ** \brief Perform a cardinal closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::card_closing(im1, oln::neighb_c4(), 200);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_card_closing.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_card_closing.png
      ** \image latex oln_morpho_fast_card_closing.png
      **
      */
      xxx_closing_map_decl(card);

      /*!
      ** \brief Perform a cardinal opening.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::card_opening(im1, oln::neighb_c4(), 200);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_card_opening.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_card_opening.png
      ** \image latex oln_morpho_fast_card_opening.png
      **
      */
      xxx_opening_map_decl(card);

      /*!
      ** \brief Perform an integral closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::integral_closing(im1, oln::neighb_c4(), 200);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_integral_closing.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_integral_closing.png
      ** \image latex oln_morpho_fast_integral_closing.png
      **
      */
      xxx_closing_map_decl(integral);

      /*!
      ** \brief Perform an integral opening.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::integral_opening(im1, oln::neighb_c4(), 200);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_integral_opening.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_integral_opening.png
      ** \image latex oln_morpho_fast_integral_opening.png
      **
      */
      xxx_opening_map_decl(integral);

      /*!
      ** \brief Perform a height closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::height_opening(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_height_opening.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_height_opening.png
      ** \image latex oln_morpho_fast_height_opening.png
      **
      */
      xxx_opening_map_decl(height);

      /*!
      ** \brief Perform a height closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::height_closing(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_height_closing.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_height_closing.png
      ** \image latex oln_morpho_fast_height_closing.png
      **
      */
      xxx_closing_map_decl(height);


      /*!
      ** \brief Perform a maxvalue closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::maxvalue_closing(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_maxvalue_closing.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_maxvalue_closing.png
      ** \image latex oln_morpho_fast_maxvalue_closing.png
      **
      */
      xxx_closing_map_decl(maxvalue);

      /*!
      ** \brief Perform a maxvalue opening.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::maxvalue_opening(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_maxvalue_opening.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_maxvalue_opening.png
      ** \image latex oln_morpho_fast_maxvalue_opening.png
      **
      */
      xxx_opening_map_decl(maxvalue);

      /*!
      ** \brief Perform a minvalue opening.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::minvalue_opening(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_minvalue_opening.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_minvalue_opening.png
      ** \image latex oln_morpho_fast_minvalue_opening.png
      **
      */
      xxx_opening_map_decl(minvalue);

      /*!
      ** \brief Perform a minvalue closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::minvalue_closing(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_minvalue_closing.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_minvalue_closing.png
      ** \image latex oln_morpho_fast_minvalue_closing.png
      **
      */
      xxx_closing_map_decl(minvalue);

      /*!
      ** \brief Perform a ball opening.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::ball_opening(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_ball_opening.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_ball_opening.png
      ** \image latex oln_morpho_fast_ball_opening.png
      **
      */
      xxx_opening_im_map_decl(ball);

      /*!
      ** \brief Perform a ball closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::ball_closing(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_ball_closing.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_ball_closing.png
      ** \image latex oln_morpho_fast_ball_closing.png
      **
      */
      xxx_closing_im_map_decl(ball);

      /*!
      ** \brief Perform a dist opening.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::dist_opening(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_dist_opening.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_dist_opening.png
      ** \image latex oln_morpho_fast_dist_opening.png
      **
      */
      xxx_opening_im_map_decl(dist);

      /*!
      ** \brief Perform a dist closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::dist_closing(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_dist_closing.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_dist_closing.png
      ** \image latex oln_morpho_fast_dist_closing.png
      **
      */
      xxx_closing_im_map_decl(dist);

      /*!
      ** \brief Perform a cube closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::cube_closing(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_cube_closing.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_cube_closing.png
      ** \image latex oln_morpho_fast_cube_closing.png
      **
      */
      xxx_closing_im_map_decl(cube);

      /*!
      ** \brief Perform a cube opening.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::cube_opening(im1, oln::neighb_c4(), 5);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_cube_opening.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_cube_opening.png
      ** \image latex oln_morpho_fast_cube_opening.png
      **
      */
      xxx_opening_im_map_decl(cube);

      /*!
      ** \brief Perform a box closing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **   ntg::vec<2, unsigned, mlc::final>	lambda;
      **   lambda[0] = lambda[1] = 50;
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::box_closing(im1, oln::neighb_c4(), lambda);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_box_closing.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_box_closing.png
      ** \image latex oln_morpho_fast_box_closing.png
      **
      */
      xxx_closing_im_map_decl(box);

      /*!
      ** \brief Perform a box opening.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/attribute_closing_opening_map.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** #include <iostream>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **   ntg::vec<2, unsigned, mlc::final>	lambda;
      **   lambda[0] = lambda[1] = 50;
      **   im_type im1(oln::load(IMG_IN "lena128.pgm"));
      **   im1 = oln::morpho::slow::box_opening(im1, oln::neighb_c4(), lambda);
      **   oln::save(im1, IMG_OUT "oln_morpho_fast_box_opening.ppm");
      **   return  0;
      ** }
      ** \endcode
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_fast_box_opening.png
      ** \image latex oln_morpho_fast_box_opening.png
      **
      */
      xxx_opening_im_map_decl(box);


    } // end of namespace slow
  } // end of namespace morpho
} // end of namespace oln

#include "attribute_closing_opening_map.hxx"

#endif
