// Copyright 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_EXTREMA_KILLER_HH
# define OLENA_MORPHO_EXTREMA_KILLER_HH

#include <oln/config/system.hh>
#include <oln/basics2d.hh>
#include <oln/types/all.hh>
#include <oln/morpho/extrema.hh>
#include <oln/arith/ops.hh>
#include <oln/morpho/watershed.hh>

#include <oln/level/connected.hh>
#include <oln/level/lut.hh>

#include <oln/level/fill.hh>

#include <set>
#include <algorithm>


namespace oln {

  namespace morpho {

    template<class _I, class _N>
    typename mute<_I, bin>::ret
    internal_kill_cc_area(const image<_I>& _input,
			  const unsigned int area,
			  const neighborhood<_N>& _Ng)
    {
      Exact_cref(I, input);
      Exact_cref(N, Ng);

      typename mute<_I, int_u<20> >::ret cc = level::connected_component<int_u<20> >(input, Ng);
      // label 0 is background
      int_u<20> max_label = 0;
      image2d<int_u<20> >::iter p(cc);
      for_all(p)
	if (cc[p] > max_label)
	  max_label = cc[p];
      level::hlut_def<int_u<20> > region_area;
      for_all(p)
	region_area.set(cc[p], cast::force<int_u<20> >(region_area(cc[p]) + 1));
      image2d<bin> output(input.size());
      for_all(p)
	if (input[p] == true)
	  {
	    if (region_area(cc[p]) >= int_u<20> (area))
	      output[p] = true;
	    else
	      output[p] = false;
	  }
	else
	  output[p] = false;
      return output;
    }


    // SURE VERSIONS

    /*=processing sure_maxima_killer
     * ns: morpho
     * what: Maxima killer.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const unsigned int area, area, IN, area
     * arg: const struct_elt<E>&, se, IN, structural element
     * ret: Concrete(I1)
     * doc: It removes the small (in area) connected components of the upper
     * level sets of \var{input} using \var{se} as structual element. The implementation
     * uses the threshold superposition principle; so it is very slow ! it works only for
     * int_u8 images.
     * see: morpho::fast_maxima_killer
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ save(morpho::sure_maxima_killer(light, 20, win_c8p()), "out.pgm");
     * exi: light.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/
    template<class _I, class _N>
    Concrete(_I) sure_maxima_killer(const image<_I>& _input,
				    const unsigned int area,
				    const neighborhood<_N>& _Ng)
    {
      Exact_cref(I, input);
      Exact_cref(N, Ng);
      meta::eq<_I::dim, _N::dim>::ensure();
      typedef typename mute<_I, bin >::ret ima_bin_t;

      ima_bin_t* cc_level_sets = new (image2d<bin> [256]);
      for (unsigned int i=0; i <= 255; ++i)
	{
	  image2d<bin> level_sets_i(input.size());
	  image2d<int_u8>::iter p(input);
	  for_all(p)
	    if (input[p] >= Value(I) (i))
	      level_sets_i[p] = true;
	    else
	      level_sets_i[p] = false;
	  cc_level_sets[i] = internal_kill_cc_area(level_sets_i, area, Ng);
	}
      image2d<int_u8> output(input.size());
      for (int i=0; i < 255 ; ++i)
	{
	  image2d<int_u8>::iter p(input);
	  for_all(p)
	    {
	      if ((cc_level_sets[i])[p] == true)
		output[p] = i;
	    }
	}
      delete[] cc_level_sets;

      return output;
    }


    /*=processing sure_minima_killer
     * ns: morpho
     * what: Minima killer.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const unsigned int area, area, IN, area
     * arg: const struct_elt<E>&, se, IN, structural element
     * ret: Concrete(I1)
     * doc: It removes the small (in area) connected components of the lower
     * level sets of \var{input} using \var{se} as structual element. The implementation
     * uses the threshold superposition principle; so it is very slow ! it works only for
     * int_u8 images.
     * see: morpho::fast_maxima_killer
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ save(morpho::sure_minima_killer(light, 20, win_c8p()), "out.pgm");
     * exi: light.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/
    template<class _I, class _N>
    image2d<int_u8> sure_minima_killer(const image<_I>& _input,
				       const unsigned int area,
				       const neighborhood<_N>& _Ng)
    {
      Exact_cref(I, input);
      Exact_cref(N, Ng);
      meta::eq<_I::dim, _N::dim>::ensure();

      typedef image2d<bin> ima_bin_t;

      ima_bin_t* cc_level_sets = new (image2d<bin> [256]);
      for (unsigned int i=0; i <= 255; ++i)
	{
	  image2d<bin> level_sets_i(input.size());
	  image2d<int_u8>::iter p(input);
	  for_all(p)
	    if (input[p] <= Value(I) (i))
	      level_sets_i[p] = true;
	    else
	      level_sets_i[p] = false;
	  cc_level_sets[i] = internal_kill_cc_area(level_sets_i, area, Ng);
	}
      image2d<int_u8> output(input.size());
      for (int i=255; i >= 0 ; --i)
	{
	  image2d<int_u8>::iter p(input);
	  for_all(p)
	    {
	      if ((cc_level_sets[i])[p] == true)
		output[p] = i;
	    }
	}
      delete[] cc_level_sets;

      return output;
    }

    // END OF SURE VERSION


    // FAST VERSIONS

    template<class _P, class _I, class _N>
    inline
    static
    bool is_a_strict_minimum(const point<_P>& _p,
			     const image<_I>& _input,
			     const neighborhood<_N>& _Ng)
    {
      Exact_cref(P, p);
      Exact_cref(I, input);
      Exact_cref(N, Ng);
      meta::eq<_I::dim, _N::dim>::ensure();
      meta::eq<_P::dim, _N::dim>::ensure();

      bool is_p_lower = true;
      bool is_p_at_least_one_stricly_lower = false;
      Neighb(N) p_prime(Ng, p);
      for_all(p_prime) if (input.hold(p_prime))
	{
	  if (input[p] < input[p_prime])
	    is_p_at_least_one_stricly_lower = true;
	  if (input[p] > input[p_prime])
	    is_p_lower = false;
	}
      return (is_p_lower && is_p_at_least_one_stricly_lower);
    }


    template<class _P, class _I, class _N>
    inline
    static
    bool is_a_strict_maximum(const point<_P>& _p,
			     const image<_I>& _input,
			     const neighborhood<_N>& _Ng)
    {
      Exact_cref(P, p);
      Exact_cref(I, input);
      Exact_cref(N, Ng);
      meta::eq<_I::dim, _N::dim>::ensure();
      meta::eq<_P::dim, _N::dim>::ensure();

      bool is_p_upper = true;
      bool is_p_at_least_one_stricly_upper = false;
      Neighb(N) p_prime(Ng, p);
      for_all(p_prime) if (input.hold(p_prime))
	{
	  if (input[p] > input[p_prime])
	    is_p_at_least_one_stricly_upper = true;
	  if (input[p] < input[p_prime])
	    is_p_upper = false;
	}
      return (is_p_upper && is_p_at_least_one_stricly_upper);
    }



    /*=processing fast_minima_killer
     * ns: morpho
     * what: Minima killer.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const unsigned int area, area, IN, area
     * arg: const neighborhood<_N>&, Ng, IN, neighboorhood
     * ret: Concrete(_I1)
     * doc: It removes the small (in area) connected components of the lower
     * level sets of \var{input} using \var{Ng} as neighboorhood. The implementation
     * is based on stak. Guichard and Morel, Image iterative smoothing and PDE's.
     * Book in preparation. p 265.
     * see: morpho::sure_minima_killer
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ save(morpho::fast_minima_killer(light, 20, win_c8p()), "out.pgm");
     * exi: light.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/
    // Guichard and Morel, Image iterative smoothing and PDE's. Book in preparation. p 265.
    template<class _I, class _N>
    Concrete(_I) fast_minima_killer(const image<_I>& _input,
				    const unsigned int area,
				    const neighborhood<_N>& _Ng)
    {
      Exact_cref(I, input);
      Exact_cref(N, Ng);
      meta::eq<_I::dim, _N::dim>::ensure();

      std::vector<Point(I)> cur_minimum;
      cur_minimum.reserve(15000);
      Concrete(I) working_input = input.clone();
      typename mute<_I, bin>::ret not_processed_map(input.size());
      level::fill(not_processed_map, true);

      // STEP 2: search for a local miminum
      Iter(I) p(working_input);
      for_all(p)
	{
	  if (is_a_strict_minimum(p.cur(), working_input, Ng)
	      && not_processed_map[p])
	    {
	      cur_minimum.push_back(p);
	      Value(I) lambda = working_input[p];

	      // FIXME: This should better be moved out of the loop.
	      typename mute<_I, bin>::ret is_visited(input.size());
	      level::fill(is_visited, false);
	      is_visited[p] = true;
	      //STEP 3
	      bool go_on = true; // FIXME: Use break instead of go_on = false.
	      while (go_on)
		{
		  typedef Value(I) I_type;
		  Point(I) arg_min = p;
		  Value(I) min =  optraits<I_type>::max();
		  for (unsigned i = 0; i < cur_minimum.size(); ++i)
		    {
		      Neighb(N) p_prime(Ng, cur_minimum[i]);
		      for_all(p_prime) if (working_input.hold(p_prime) &&
					   (is_visited[p_prime] == false))
			{
			  if (working_input[p_prime] <= min)
			    {
			      min = working_input[p_prime];
			      arg_min = p_prime;
			    }
			}
		    }
		  // go to step 4
		  if (working_input[arg_min] >= lambda)
		    {
		      // step 5
		      if (cur_minimum.size() < area)
			{
			  lambda = working_input[arg_min]; // END MODIF2
			  cur_minimum.push_back(arg_min); //MODIF 1
			  is_visited[arg_min] = true;
			  // go to step 3
			}
		      else
			{
			  go_on = false;
			  //go to step 6
			}
		    }
		  else
		    {
		      go_on = false; //go to step 6
		    }
		} // end while "go on"

	      // going  to step 6
	      for (unsigned i = 0; i < cur_minimum.size(); ++i)
		{
		  working_input[cur_minimum[i]] = lambda;
		  not_processed_map[cur_minimum[i]] = false;
		}
	      cur_minimum.erase(cur_minimum.begin(), cur_minimum.end());

	      // STEP 7: look for a new minimum
	    } // end processing current minimum

	} // END looking for minimum
      return working_input;
    }

    /*=processing fast_maxima_killer
     * ns: morpho
     * what: Maxima killer.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const unsigned int area, area, IN, area
     * arg: const neighborhood<_N>&, Ng, IN, neighboorhood
     * ret: Concrete(_I1)
     * doc: It removes the small (in area) connected components of the upper
     * level sets of \var{input} using \var{Ng} as neighboorhood. The implementation
     * is based on stak. Guichard and Morel, Image iterative smoothing and PDE's. Book in preparation. p 265.
     * see: morpho::sure_maxima_killer
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ save(morpho::fast_maxima_killer(light, 20, win_c8p()), "out.pgm");
     * exi: light.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/
    // Guichard and Morel, Image iterative smoothing and PDE's. Book in preparation. p 265.
    template<class _I, class _N>
    Concrete(_I) fast_maxima_killer(const image<_I>& _input,
				    const unsigned int area,
				    const neighborhood<_N>& _Ng)
    {
      Exact_cref(I, input);
      Exact_cref(N, Ng);
      meta::eq<_I::dim, _N::dim>::ensure();

      std::vector<Point(I)> cur_maximum;
      Concrete(I) working_input = input.clone();
      typename mute<_I, bin>::ret not_processed_map(input.size());
      level::fill(not_processed_map, true);

      // STEP 2: search for a local miminum
      Iter(I) p(working_input);
      for_all(p)
	{
	  if (is_a_strict_maximum(p.cur(), working_input, Ng)
	      && not_processed_map[p])
	    {
	      cur_maximum.push_back(p);
	      Value(I) lambda = working_input[p];

	      typename mute<_I, bin>::ret is_visited(input.size());
	      level::fill(is_visited, false);
	      is_visited[p] = true;
	      //STEP 3
	      bool go_on = true; // FIXME: ditto
	      while (go_on)
		{
		  typedef Value(I) I_type;
		  Point(I) arg_max = p;
		  Value(I) max =  optraits<I_type>::min();
		  for (unsigned i = 0; i < cur_maximum.size(); ++i)
		    {
		      Neighb(N) p_prime(Ng, cur_maximum[i]);
		      for_all(p_prime) if (working_input.hold(p_prime) &&
					   (is_visited[p_prime] == false))
			{
			  if (working_input[p_prime] >= max)
			    {
			      max = working_input[p_prime];
			      arg_max = p_prime;
			    }
			}
		    }
		  // go to step 4
		  if (working_input[arg_max] <= lambda)
		    {
		      // step 5
		      if (cur_maximum.size() < area)
			{
			  lambda = working_input[arg_max]; // END MODIF2
			  cur_maximum.push_back(arg_max); //MODIF 1
			  is_visited[arg_max] = true;
			  // go to step 3
			}
		      else
			{
			  go_on = false;
			  //go to step 6
			}
		    }
		  else
		    {
		      go_on = false;
		      //go to step 6
		    }
		} // end while "go on"

	      // going  to step 6
	      for (unsigned i = 0; i < cur_maximum.size(); ++i)
		{
		  //	      cout << cur_minimum[i] << "   " << lambda << endl;
		  working_input[cur_maximum[i]] = lambda;
		  not_processed_map[cur_maximum[i]] = false;
		}
	      cur_maximum.erase(cur_maximum.begin(), cur_maximum.end());

	      // STEP 7: look for a new minimum
	    } // end processing current minimum

	} // END looking for minimum
      return working_input;
    }
  }
} // oln
#endif
