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

#ifndef OLENA_MORPHO_EXTREMA_COMPONENTS_HH
# define OLENA_MORPHO_EXTREMA_COMPONENTS_HH

# include <map>
# include <queue>
# include <functional>

# include "oln/level/level_components.hh"

namespace oln {

  namespace level {

    namespace internal {

      /// Find the extrema level components of \a input, using \param
      /// Cmp as comparison function.
      template <typename DestValue, typename Cmp, typename I>
      typename ch_value_type<I, DestValue>::ret
      extrema_components(const abstract::image_with_nbh<I>& input)
      {
	mlc_is_a(I, abstract::scalar_valued_image)::ensure();
	Cmp cmp;

	// Compute level components.
	typename ch_value_type<I, DestValue>::ret comps =
	  level_components<DestValue>(input);
	std::set<DestValue> extrema;
	std::set<DestValue> non_extrema;

	// Search extrema components.
	oln_type_of(I, piter) p(input.size());
	for_all_p (p)
	  {
	    DestValue comp = comps[p];
	    if (non_extrema.find(comp) == non_extrema.end ())
	      {
		// A new level is a (potential) extrema by default.
		extrema.insert(comp);

		oln_type_of(I, niter) n(input);
		for_all_n_of_p (n, p)
		  if (input.hold(n) && cmp(input[n], input[p]))
		    {
		      extrema.erase(comp);
		      non_extrema.insert(comp);
		    }
	      }
	  }

	// Re-label the extrema.  label_map hold the assigned labels.
	std::map<DestValue, DestValue> label_map;
	{
	  DestValue cur_label = ntg_min_val(DestValue) + 1;
	  for (typename std::set<DestValue>::const_iterator i =
		 extrema.begin(); i != extrema.end(); ++i, ++cur_label)
	    label_map[*i] = cur_label;
	}
	typename ch_value_type<I, DestValue>::ret output (input.size());
	level::fill (output, ntg_min_val(DestValue));
	for_all_p (p)
	  {
	    DestValue comp = comps[p];
	    if (label_map.find(comp) != label_map.end())
	      output[p] = label_map[comp];
	  }
	return output;
      }

    } // end of namespace oln::level::internal.


    /// Find the minima level components of \a input.
    template <typename DestValue, typename I>
    typename ch_value_type<I, DestValue>::ret
    minima_components(const abstract::image_with_nbh<I>& input)
    {
      mlc_is_a(I, abstract::scalar_valued_image)::ensure();
      return internal::extrema_components< DestValue,
                                           std::less<DestValue> >(input);
    }

    /// Find the maxima level components of \a input.
    template <typename DestValue, typename I>
    typename ch_value_type<I, DestValue>::ret
    maxima_components(const abstract::image_with_nbh<I>& input)
    {
      mlc_is_a(I, abstract::scalar_valued_image)::ensure();
      return internal::extrema_components< DestValue,
                                           std::greater<DestValue> >(input);
    }

  } // end of namespace oln::level.

} // end of namespace oln.

#endif // ! OLENA_MORPHO_EXTREMA_COMPONENTS_HH
