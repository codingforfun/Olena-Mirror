// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

# include <ntg/basics.hh>
# include <oln/level/lut.hh>
# include <oln/level/fill.hh>
# include <oln/morpho/splitse.hh>
# include <oln/convert/conversion_ng_se.hh>

namespace oln {

  namespace level {

    /*=processing connected_component
     * ns: level
     * what: Connected Component.
     * arg: const abstract::image<I1>&, marker, IN, marker image
     * arg: const abstract::neighborhood<E>&, se, IN, structural element
     * ret: typename mute<I, DestType>::ret
     * doc: It removes the small (in area) connected components of the upper
     * level sets of \var{input} using \var{se} as structural element. The
     * implementation comes from \emph{Cocquerez et Philipp, Analyse d'images,
     * filtrages et segmentations} p.62.
     * see: level::frontp_connected_component
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ save(level::connected_component<int_u8>(light, win_c8p()), "out.pgm");
     * exi: light.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/

    // Number the connected components i.e label true. background(i.e
    // label false) has the label 0; This algorithm works only for 2D images
    template <class DestType, class I, class N>
    typename mute<I, DestType>::ret
    connected_component(const abstract::image<I>& input,
			const abstract::neighborhood<N>& Ng)
    {
      // FIXME: ensure the oln_value_type(I) is bin.
      mlc::eq<I::dim, N::dim>::ensure();
      mlc::eq<I::dim, 2>::ensure();
      typename mute<I, DestType>::ret output(input.size());
      level::hlut< DestType, DestType > T;
      DestType k = 1;

      fill(output, 0);
     oln_iter_type(I) p(input);
      for_all(p)
	{
	  if (input[p] == true)
	    {
	      typedef typename abstract::neighborhood<N>::win_type E;
	      E se_plus = morpho::get_plus_se_only(convert::ng_to_se(Ng));
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
		      DestType min = ntg_max_val(DestType);
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
