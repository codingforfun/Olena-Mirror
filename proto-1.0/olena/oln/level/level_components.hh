// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_LEVEL_COMPONENTS_HH
# define OLENA_MORPHO_LEVEL_COMPONENTS_HH

# include <queue>

# include "oln/level/fill.hh"

namespace oln {

  namespace level {

    /// Compute the level components of \a input.
    template <typename DestValue, typename I>
    typename ch_value_type<I, DestValue>::ret
    level_components(const abstract::image_with_nbh<I>& input)
    {
      mlc_is_a(I, abstract::scalar_valued_image)::ensure();

      typename ch_value_type<I, DestValue>::ret labels(input.size());
      typename ch_value_type<I, bool>::ret processed(input.size());
      level::fill (processed, false);

      DestValue cur_label = ntg_min_val(DestValue);

      typedef oln_type_of(I, point) point_type;
      std::queue<point_type> q;

      oln_type_of(I, piter) p(input.size());
      for_all_p (p)
	if (!processed[p])
	  {
	    labels[p] = cur_label;
	    q.push(p);
	    while (!q.empty())
	      {
		point_type s = q.front();
		q.pop();
		oln_type_of(I, niter) n(input);
		for_all_n_of_p (n, s)
		  if (input.hold(n) && input[n] == input[s] && !processed[n])
		    {
		      labels[n] = cur_label;
		      processed[n] = true;
		      q.push(n);
		    }
	      }
	    ++cur_label;
	  }
      return labels;
    }

  } // end of namespace oln::level

} // end of namespace oln

#endif // ! OLENA_MORPHO_LEVEL_COMPONENTS_HH
