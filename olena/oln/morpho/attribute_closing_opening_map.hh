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
    namespace slow
    {
      /*! Attribute closing using map. Smaller memory usage, but
       *  slower computation than the attribute_closing_opening
       *
       * See "Fast morphological attribute operations using Tarjan's union-find
       * algorithm" by Michael H. F. Wilkinson and Jos B. T. M. Roerdink
       */
      template <class I, class D, class Env = morpho::NullEnv>
      struct f_tarjan_map
      {
      public:
	typedef abstract::image<I>			input_type;
	typedef oln_concrete_type(I)			img_type;

	typedef oln_point_type(input_type)		point_type;
	typedef oln_value_type(input_type)		value_type;
	typedef typename mute<input_type, point_type>::ret parent_type;
	typedef typename mute<input_type, bool>::ret	is_proc_type;
	typedef attr_lambda_type(D)			lambda_type;

	// e.g.,
	// when I is image2d<int_u8> and D is area_type, we've got:
	// +--------------+------------------+
	// | input_type   | image2d<int_u8>  |
	// | point_type   | point2d          |
	// | parent_type  | image2d<point2d> |
	// | is_proc_type | image2d<bool>    |
	// | lambda_type  | int              |
	// +--------------+------------------+

	template <class N>
	f_tarjan_map(bool is_closing,
		     const input_type& input,
		     const abstract::neighborhood<N>& ng,
		     const lambda_type& lambda,
		     const Env & env = Env());

	oln_concrete_type(I)
	  res()
	{
	  return output;
	}
      protected:

	static const point_type
	inactive();

	static const point_type
	active();

	void
	make_set(const point_type& x);

	void
	link(const point_type& x, const point_type& y);

	point_type
	find_root(const point_type& x);

	bool
	equiv(const point_type& x, const point_type& y) const;

	void
	do_union(const point_type& n, const point_type& p);

	const bool is_closing;
	const input_type& input;
	lambda_type lambda;
	parent_type parent;
	is_proc_type is_proc;
	img_type output;
	std::map<point_type, D, oln::internal::default_less<point_type> > auxdata;
	Env env;
      };


      /*! Attribute closing using map. Smaller memory usage, but
       *  slower computation than the attribute_closing_opening
       *
       * See "Fast morphological attribute operations using Tarjan's union-find
       * algorithm" by Michael H. F. Wilkinson and Jos B. T. M. Roerdink
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

      /*! Some macro to declare friendly function for different attributes
       * opening/closing
       */
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

      xxx_opening_map_decl(area);
      xxx_closing_map_decl(area);
      xxx_opening_map_decl(volume);
      xxx_closing_map_decl(volume);
      xxx_opening_map_decl(height);
      xxx_closing_map_decl(height);
      xxx_opening_map_decl(maxvalue);
      xxx_closing_map_decl(maxvalue);
      xxx_opening_map_decl(minvalue);
      xxx_closing_map_decl(minvalue);
      xxx_opening_im_map_decl(disk);
      xxx_closing_im_map_decl(disk);
      xxx_opening_im_map_decl(dist);
      xxx_closing_im_map_decl(dist);
      xxx_closing_im_map_decl(square);
      xxx_opening_im_map_decl(square);
      xxx_closing_im_map_decl(rectangle);
      xxx_opening_im_map_decl(rectangle);


    } // end of namespace slow
  } // end of namespace morpho
} // end of namespace oln

#include "attribute_closing_opening_map.hxx"

#endif
