// Copyright (C) 2001, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_HIT_OR_MISS_HH
# define OLENA_MORPHO_HIT_OR_MISS_HH

# include <oln/core/gen/image_with_nbh.hh>
# include <oln/morpho/erosion.hh>
# include <oln/morpho/dilation.hh>
# include <oln/arith/min.hh>
# include <oln/utils/invert.hh>

namespace oln {

  namespace tag {

    // No tags yet

  } // end of namespace oln::morpho::tag


  namespace morpho {
    template <typename I, typename W1, typename W2> struct hit_or_miss_ret;
  } // end of namespace oln::morpho

  // super_type

  template <typename I, typename W1, typename W2>
  struct set_super_type< morpho::hit_or_miss_ret<I, W1, W2> >
  {
    typedef oln_type_of(I, concrete) output_type;

    typedef morpho::hit_or_miss_ret<I, W1, W2> self_type;
    typedef abstract::image_unary_operator<output_type, I, self_type > ret;
  };

  namespace morpho {

    // Reconstruction as a 'classical' procedure returning an image (do not
    // use it; prefer morpho::hit_or_miss).

    namespace proc {

      // FIXME: ...

    } // end of namespace oln::morpho::proc


    template <typename I, typename W1, typename W2>
    struct hit_or_miss_ret :
      // FIXME: oln_super_of_
      public oln::internal::get_super_type< hit_or_miss_ret<I, W1, W2> >::ret
    {
      typedef hit_or_miss_ret<I, W1, W2> self_type;
      typedef typename oln::internal::get_super_type<self_type>::ret super_type;
      const W1& win1;
      const W2& win2;

      hit_or_miss_ret(const abstract::image<I>& input,
	const abstract::regular_window<oln_type_of(I, grid), W1> win1,
	const abstract::regular_window<oln_type_of(I, grid), W2> win2) :
	super_type(input),
	win1(win1.exact()),
	win2(win2.exact())
      {
      }

    };


    namespace impl {

      template <typename I, typename W1, typename W2>
      struct generic_hit_or_miss : public hit_or_miss_ret<I, W1, W2>
      {
	typedef hit_or_miss_ret<I, W1, W2> super_type;

	generic_hit_or_miss(const abstract::image<I>& input,
	    const abstract::regular_window<oln_type_of(I, grid), W1> win1,
	    const abstract::regular_window<oln_type_of(I, grid), W2> win2) :
	  super_type(input, win1, win2)
	{
	}

	void impl_run()
	{
	  this->output = arith::min(erosion(this->input, this->win1),
				    erosion(utils::invert(this->input),
					    this->win2));
	}
      };

      template <typename I, typename W1, typename W2>
      hit_or_miss_ret<I, W1, W2>
      hit_or_miss(const abstract::image<I>& input,
	  const abstract::regular_window<oln_type_of(I, grid), W1> win1,
	  const abstract::regular_window<oln_type_of(I, grid), W2> win2)
      {
	generic_hit_or_miss<I, W1, W2> tmp(input, win1, win2);
	tmp.run();
	return tmp;
      }

    } //end of namespace impl

    template <typename I, typename W1, typename W2>
    hit_or_miss_ret<I, W1, W2>
    hit_or_miss(const abstract::image<I>& input,
		const abstract::regular_window<oln_type_of(I, grid), W1> win1,
		const abstract::regular_window<oln_type_of(I, grid), W2> win2)
    {
      // FIXME: win1 inter win2 must be empty
      return impl::hit_or_miss(input.exact(), win1.exact(), win2.exact());
    }


    template<class I, class W1, class W2>
    dilation_ret<hit_or_miss_ret<I, W1, W2>, W1>
    hit_or_miss_opening(const abstract::image<I>& input,
	const abstract::regular_window<oln_type_of(I, grid), W1> win1,
	const abstract::regular_window<oln_type_of(I, grid), W2> win2)
    {
      return dilation(hit_or_miss(input, win1, win2), -win1);
    }

    template<class I, class W1, class W2>
    dilation_ret<hit_or_miss_ret<I, W2, W1>, W2>
    hit_or_miss_opening_bg(const abstract::image<I>& input,
	const abstract::regular_window<oln_type_of(I, grid), W1> win1,
	const abstract::regular_window<oln_type_of(I, grid), W2> win2)
    {
      return hit_or_miss_opening(input, win2, win1);
    }

    template<class I, class W1, class W2>
    utils::image_inverted<dilation_ret<hit_or_miss_ret<utils::image_inverted<I>,
						       W1, W2>, W1> > // !!!
    hit_or_miss_closing(const abstract::image<I>& input,
	const abstract::regular_window<oln_type_of(I, grid), W1> win1,
	const abstract::regular_window<oln_type_of(I, grid), W2> win2)
    {
      return utils::invert(hit_or_miss_opening(utils::invert(input),
					       win1, win2));
    }

    template<class I, class W1, class W2>
    utils::image_inverted<dilation_ret<hit_or_miss_ret<utils::image_inverted<I>,
						       W2, W1>, W2> > // !!!
    hit_or_miss_closing_bg(const abstract::image<I>& input,
	const abstract::regular_window<oln_type_of(I, grid), W1> win1,
	const abstract::regular_window<oln_type_of(I, grid), W2> win2)
    {
      return hit_or_miss_closing(input, win2, win1);
    }



  } // end of namespace morpho

} // end of namespace oln

#endif // ! OLENA_MORPHO_HIT_OR_MISS_HH
