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

#ifndef OLENA_MORPHO_STAT_HH
# define OLENA_MORPHO_STAT_HH

# include <mlc/cmp.hh>
# include <ntg/bin.hh>
# include <oln/basics.hh>
# include <oln/funobj/accum.hh>


namespace oln {

  namespace morpho {


    /// Local min on a function.

    template <typename I, typename W>
    oln_type_of(I, value) local_min(const abstract::image<I>&    input,
				    const oln_type_of(I, point)& p,
				    const abstract::window<W>&   win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      oln_wn_type_of(W, fwd_iter) q(win);
      funobj::min_accumulator<oln_type_of(I, value)> minval;
      
      for_all_q_of_p (q, p)
	if (input.hold(q))
	  minval(input[q]);

      return minval;
    }


    template <typename I>
    oln_type_of(I, value) local_min_nbh(const abstract::image_with_nbh<I>& input,
					const oln_type_of(I, point)& p)
    {
      oln_type_of(I, niter) n(input);
      funobj::min_accumulator_init<oln_type_of(I, value)> minval(input[p]);
      
      for_all_n_of_p (n, p)
	if (input.hold(n))
	  minval(input[n]);

      return minval;
    }


    /// Local min on a set.

    template <typename I, typename W>
    bool local_min(const abstract::binary_image<I>& input,
		   const oln_type_of(I, point)& p,
		   const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      oln_wn_type_of(W, fwd_iter) q(win);
      for_all_q_of_p (q, p)
	if (input.hold(q) and not input[q])
	  return false;
      return true;
    }


    /// Local max on a function.

    template <typename I, typename W>
    oln_type_of(I, value) local_max(const abstract::image<I>& input,
				    const oln_type_of(I, point)& p,
				    const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      oln_wn_type_of(W, fwd_iter) q(win);
      funobj::max_accumulator<oln_type_of(I, value)> maxval;
      
      for_all_q_of_p (q, p)
	if (input.hold(q))
	  maxval(input[q]);

      return maxval;
    }


    /// Local max on a set.

    template <typename I, typename W>
    bool local_max(const abstract::binary_image<I>& input,
		   const oln_type_of(I, point)& p,
		   const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      oln_wn_type_of(W, fwd_iter) q(win);
      for_all_q_of_p (q, p)
	if (input.hold(q) and input[q])
	  return true;
      return false;
    }


  } // end of namespace morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_STAT_HH
