// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_DILATION_HH
# define OLENA_MORPHO_DILATION_HH

# include <mlc/cmp.hh>

# include <oln/utils/record.hh>
# include <oln/basics.hh>
# include <oln/morpho/local.hh>
# include <oln/core/2d/window2d.hh>


namespace oln {

  namespace morpho {



    // Fwd decl of dilation facade.

    template<typename I, typename W>
    oln_type_of(I, concrete) dilation(const abstract::image<I>& input,
				      const abstract::window<W>& win);


    namespace impl {


      // Generic on set.

      template<typename I, typename W>
      oln_type_of(I, concrete)
      dilation_(const abstract::binary_image<I>& input,
		const abstract::window<W>& win)
      {
	entering("->generic_on_set");
	registering(input, "input");

	oln_type_of(I, concrete) output(input.size(), "output");

	oln_type_of(I, fwd_piter) p(input.size());
	for_all_p (p)
	  output[p] = win_or_value(input, p, win);

	exiting("->generic_on_set");
	return output;
      }


      // Generic on function.

      template<typename I, typename W>
      oln_type_of(I, concrete)
      dilation_(const abstract::not_binary_image<I>& input,
		const abstract::window<W>& win)
      {
	entering("->generic_on_function");
	registering(input, "input");

	oln_type_of(I, concrete) output(input.size(), "output");

	oln_type_of(I, fwd_piter) p(input.size());
	for_all_p (p)
	  output[p] = win_sup_value(input, p, win);

	exiting("->generic_on_function");
	return output;
      }


      // win is a 2D rectangle.

      template<typename I>
      oln_type_of(I, concrete) dilation_(const abstract::image2d<I>& input,
					 const win_rectangle2d& win)
      {
	entering("->(image2d,win_rectangle2d)");
	registering(input, "input");

	oln_type_of(I, concrete) temp("temp"), output("output");

	win_hline2d hline(win.width); // FIXME: Cf. Soille, use L(i,dp)
	win_vline2d vline(win.height);

	temp = dilation(input, hline);
	output = dilation(temp, vline);

	exiting("->(image2d,win_rectangle2d)");
	return output;
      }


      // add some other impls here...

      
    } // end of namespace oln::morpho::impl




    /// Generic dilation (facade).

    template<typename I, typename W>
    oln_type_of(I, concrete) dilation(const abstract::image<I>& input,
				      const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();
      entering("morpho::dilation");

      oln_type_of(I, concrete) output("output");
      output = impl::dilation_(input.exact(), win.exact());
      
      exiting("morpho::dilation");
      return output;
    }


  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_DILATION_HH
