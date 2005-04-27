// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_OPENING_HH
# define OLENA_MORPHO_OPENING_HH

# include <mlc/cmp.hh>
# include <mlc/to_string.hh>

# include <oln/core/abstract/image_operator.hh>

# include <oln/morpho/dilation.hh>
# include <oln/morpho/erosion.hh>
# include <oln/core/2d/window2d.hh>


namespace oln {


  // fwd decl

  namespace morpho {
    template <typename I> struct opening_ret;
  }

  // super_type

  template <typename I>
  struct set_super_type< morpho::opening_ret<I> >
  {
    typedef oln_type_of(I, concrete) output_type;

    typedef morpho::opening_ret<I> self_type;
    typedef abstract::image_unary_operator<output_type, I, self_type > ret;
  };




  namespace morpho {



    // Opening as a 'classical' procedure returning an image (do not
    // use it; prefer morpho::opening).

    namespace proc {

      template<typename I, typename W>
      oln_type_of(I, concrete) opening(const abstract::image<I>& input,
				       const abstract::window<W>& win)
      {
	mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();
	oln_type_of(I, concrete) output(input.size());
	output = morpho::dilation(morpho::erosion(input, win), win);
	return output;
      }

    } // end of namespace oln::morpho::proc



    /// Opening return type.

    template <typename I>
    struct opening_ret : public oln_super_of_(opening_ret<I>)
    {
      typedef oln_super_of(opening_ret<I>) super_type;

      opening_ret(const abstract::image<I>& input) :
	super_type(input)
	{
	}

    };


    // Various implementation.

    namespace impl {


      /// Generic implementation of opening (type).

      template <typename I, typename W>
      struct generic_opening : public opening_ret<I>
      {
	typedef opening_ret<I> super_type;
	// FIXME: typedef oln_type_of(super_type, output) output_type;

	const W& win;

	generic_opening(const abstract::image<I>& input,
			const abstract::window<W>& win) :
	  super_type(input),
	  win(win.exact())
	{
	}

	void impl_run()
	{
	  oln_type_of(super_type, output) tmp(input.size());
	  tmp = morpho::dilation(morpho::erosion(input, win), win);
	  output = tmp;
	}
      };

      // Generic implementation of opening (routine).

      template<typename I, typename W>
      opening_ret<I> opening(const abstract::image<I>& input,
			     const abstract::window<W>& win)
      {
	impl::generic_opening<I,W> tmp(input, win);
	tmp.run();
	return tmp;
      }

      // FIXME: Add specialized implementations.

    } // end of namespace oln::morpho::impl


    /// Generic opening (facade).

    template<typename I, typename W>
    opening_ret<I> opening(const abstract::image<I>& input,
			   const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();
      return impl::opening(input.exact(), win.exact());
    }


  } // end of namespace oln::morpho


} // end of namespace oln


#endif // ! OLENA_MORPHO_OPENING_HH
