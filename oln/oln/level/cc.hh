// Copyright (C) 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_LEVEL_CC_HH	// connected components
# define OLENA_LEVEL_CC_HH

# include <oln/basics.hh>
// FIXME: really need all ?
# include <oln/arith/ops.hh>
# include <oln/meta/optional.hh>
# include <oln/types/all.hh>
# include <oln/level/fill.hh>
# include <oln/level/invert.hh>
# include <oln/level/compare.hh>
# include <set>
# include <list>
# include <vector>

namespace oln {
  namespace level {

    // optional behaviour for this algorithm.
    struct update_label;

    /*=processing frontp_connected_component
     * ns: level
     * what: Connected Component.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const neighborhood<E>&, se, IN, neighbourhood
     * arg: numeric_value&, nb, IN, nb_label (optional)
     * ret: typename mute<I, DestType>::ret
     * doc: It removes the small (in area) connected components of the upper
     * level sets of \var{input} using \var{se} as structural element. 
     * The implementation
     * uses front propagation.
     * see: level::connected_component
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ save(level::frontp_connected_component<int_u16>(light, win_c8p()), 
     * $ "out.pgm");
     * exi: light.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/


    // Number the connected components i.e label true. background(i.e
    // label false) has the label 0; in the output
    // FIXME: Should probably be turned into a class.
    template <class DestType, class _I, class _E>
    typename mute<_I, DestType>::ret
    frontp_connected_component(const image<_I>& _input,
			       const neighborhood<_E>& _se, 
			       unsigned& nb_label)
    {
      // FIXME: ensure the Value(I) is bin.
      Exact_cref(I, input);
      Exact_cref(E, se);
      typename mute<_I, DestType>::ret output(input.size());
      level::fill(output, 0);

      typedef std::set<Point(I), oln::internal::default_less< Point(I) > > 
	points_set;

      I is_processed(input.size());
      level::fill(is_processed, false);
      DestType cur_label = 1;
      Iter(I) p(input);
      for_all(p) if ((input[p] == true)&& (is_processed[p] == false))
	{
	  //propagation front
	  points_set component;
	  component.insert(p.cur());
	  points_set points_to_process;
	  points_to_process.insert(p.cur());
	  while (!points_to_process.empty())
	    {
	      //set label and net neighbors
	      points_set next;
	      for (typename points_set::const_iterator i = 
		     points_to_process.begin();
		   i != points_to_process.end();
		   ++i)
		{
		  component.insert(*i);
		  Neighb(E) p_prime(se, *i);
		  for_all (p_prime) if(input.hold(p_prime) &&
				       (input[p_prime] == true))
		    next.insert(p_prime.cur());
		}
	      points_to_process.clear();
	      set_difference(next.begin(), next.end(),
			     component.begin(), component.end(),
			     inserter(points_to_process,
				      points_to_process.begin()),
			     oln::internal::default_less< Point(_I) >());
	    }
	  for (typename points_set::const_iterator i = component.begin();
	       i != component.end();
	       ++i)
	    {
	      output[*i] = cur_label;
	      is_processed[*i] = true;
	    }
	  cur_label++;
	}
      nb_label = cur_label;
      return output;
    }

    template <class DestType, class _I, class _E>
    typename mute<_I, DestType>::ret
    frontp_connected_component(const image<_I>& _input,
			       const neighborhood<_E>& _se)
    {
      unsigned dummy;
      return frontp_connected_component(_input, _se, dummy);
    }

    template <class _I>
    typename mute<_I, bin>::ret
    extract_i_cc(const image<_I>& _input,
		 Value(_I) i)
    {
      Exact_cref(I, input);

      typename mute<_I, bin>::ret output(input.size());
      level::fill(output, false);
      Iter(I) p(input);
      for_all(p)
	if (input[p] == i)
	  output[p] = true;
      return output;
    }

    template <class _I>
    Value(_I) get_n_cc(const image<_I>& _input)
    {
      Exact_cref(I, input);
      return  fold(arith::f_max<Value(I)>(), input);
    }

  } // end of level.

} // end of oln.

#endif
