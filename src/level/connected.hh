// Copyright 2001  EPITA Research and Development Laboratory
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

#ifndef OLENA_LEVEL_CONNECTED_HH
# define OLENA_LEVEL_CONNECTED_HH

# include "level/lut.hh"
# include "level/fill.hh"
# include "morpho/splitse.hh"

namespace oln {
  namespace level {


    /*=processing connected_component
     * ns: level
     * what: Connected Component.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const struct_elt<E>&, se, IN, structural element
     * ret: typename mute<_I, DestType>::ret
     * doc: It removes the small (in area) connected components of the upper
     * level sets of @var{input} using @var{se} as structural element. FIXME: The implementation
     * is in REF Maitre.
     * see: level::frontp_connected_component
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ save(level::connected_component<int_u8>(light, win_c8p()), "out.pgm");
     * exi: light.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/

    // Number the connected components i.e label true. background(i.e
    // label false) has the label 0; in the output
    template <class DestType, class _I, class _E>
    typename mute<_I, DestType>::ret
    connected_component(const image<_I>& _input, const struct_elt<_E>& _se)
    {
      // FIXME: ensure the Value(I) is bin.
      Exact_cref(I, input);
      Exact_cref(E, se);
      typename mute<_I, DestType>::ret output(input.size());
      level::hlut< DestType, DestType > T;
      DestType k = 1;

      fill(output, 0);

      Iter(I) p(input);
      for_all(p)
	{
	  if (input[p] == true)
	    {
	      E se_plus = morpho::get_plus_se_only(se);
	      Neighb(E) p_prime(se_plus, p);
	      bool all_zero = true;
	      for_all(p_prime)
		if (input.hold(p_prime) && input[p_prime])
		  {
		    all_zero = false;
		    break;
		  }
	      if (all_zero)
		{
		  output[p] = k++;
		}
	      else
		{
		  // Check if all neighbors have the same label.
		  bool same_label = true;
		  DestType cl = 0;
		  // Find the first label.
		  for_all (p_prime)
		    if (output.hold(p_prime))
		      {
			cl = output[p_prime];
			break;
		      }
		  // Check whether all remaining neighbors are equal.
		  for_all_remaining (p_prime)
		    if (output.hold(p_prime) && cl != output[p_prime])
		      {
			same_label = false;
			break;
		      }

		  if (same_label)
		    output[p] = cl;
		  else
		    {
		      DestType min = DestType::max();
		      for_all(p_prime)
			if (output.hold(p_prime))
			  if (output[p_prime] && T(output[p_prime]) < min)
			    min = T(output[p_prime]);
		      output[p] = min;

		      for_all(p_prime)
			if (input.hold(p_prime) && output[p_prime])
			  {
			    DestType m;
			    DestType beta = T(output[p_prime]);
			    while (T(beta) != min)
			      {
				m = T(beta);
				T.set(beta, min);
				beta = m;
			      }
			  }
		    }
		}
	    }
	}

      for (DestType i = 1; i <= k; ++i)
	{
	  DestType m = i;
	  while (T(m) != m)
	    m = T(m);
	  T.set(i,m);
	}
      for_all (p)
	output[p] = T(output[p]);
      return output;
    }

  } // level
} // oln


#endif // OLENA_LEVEL_CONNECTED_HH
