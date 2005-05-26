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

#ifndef OLENA_MORPHO_EROSION_HH
# define OLENA_MORPHO_EROSION_HH

# include <mlc/cmp.hh>
# include <mlc/is_a.hh>
# include <mlc/implies.hh>

# include <oln/utils/record.hh>
# include <oln/basics.hh>
# include <oln/core/2d/window2d.hh>
# include <oln/morpho/tags.hh>


namespace oln {

  namespace morpho {



    // Fwd decl of erosion's facade.

    template<typename K, typename I, typename W>
    oln_type_of(I, concrete) erosion(const tag::kind<K>& kind,
				     const abstract::image<I>& input,
				     const abstract::window<W>& win);

    // Facade for classical erosion.

    template<typename I, typename W>
    oln_type_of(I, concrete) erosion(const abstract::image<I>& input,
				     const abstract::window<W>& win)
    {
      return erosion(tag::classical, input, win);
    }


    namespace impl {

      // generic

      template<typename K, typename I, typename W>
      oln_type_of(I, concrete) erosion_(const tag::kind<K>& kind,
					const abstract::image<I>& input,
					const abstract::window<W>& win)
      {
	entering("->generic");
	registering(input, "input");

	oln_type_of(I, concrete) output(input.size(), "output");

	oln_type_of(I, fwd_piter) p(input.size());
	for_all_p (p)
	  output[p] = kind.min(input, p, win);

	exiting("->generic");
	return output;
      }



      // win is a 2D rectangle

      template<typename K, typename I>
      oln_type_of(I, concrete) erosion_(const tag::kind<K>& kind,
					const abstract::image2d<I>& input,
					const win_rectangle2d& win)
      {
	entering("->(image2d,win_rectangle2d)");
	registering(input, "input");

	oln_type_of(I, concrete) temp("temp"), output("output");

	win_hline2d hline(win.width); // FIXME: Cf. Soille, use L(i,dp)
	win_vline2d vline(win.height);

	temp = erosion(kind, input, hline);
	output = erosion(kind, temp, vline);

	exiting("->(image2d,win_rectangle2d)");
	return output;
      }


      // add some other impls here...

      
    } // end of namespace oln::morpho::impl




    /// Generic erosion (facade).

    template<typename K, typename I, typename W>
    oln_type_of(I, concrete) erosion(const tag::kind<K>& kind,
				     const abstract::image<I>& input,
				     const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();
      mlc::implies< mlc_is_a(I, abstract::binary_image),
	            mlc_eq(K, tag::classical_type) >::ensure();
      entering("morpho::erosion");

      oln_type_of(I, concrete) output("output");
      output = impl::erosion_(kind, input.exact(), win.exact());
      
      exiting("morpho::erosion");
      return output;
    }




  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_EROSION_HH
