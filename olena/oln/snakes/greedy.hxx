// Copyright (C) 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_SNAKES_GREEDY_HXX
# define OLENA_SNAKES_GREEDY_HXX

namespace oln {

  namespace snakes {


    template <int N, class I, template<typename> class external_energy>
    greedy<N, I, external_energy>::greedy(ntg::float_s alpha,
					  ntg::float_s beta,
					  ntg::float_s gamma,
					  ntg::float_s khi) :
      continuity_e(&average_dist),
      curvature_e(curvature_energy<I>::cookie()),
      image_e(image_energy<I>::cookie()),
      external_e(external_energy<I>::cookie()),
      alpha(alpha), beta(beta), gamma(gamma), khi(khi)
    {
    }


    template <int N, class I, template<typename> class external_energy>
    int
    greedy<N, I, external_energy>::
    update_snake(const I& gradient, snake<greedy<N, I, external_energy> >& s)
    {
      /// This place is left void to make room for a future extension
      /// where a snake will be able to hold several segments.
      average_dist = compute_average_dist(s.s);
      return update_segment(gradient, s.s);
    }

    template <int N, class I, template<typename> class external_energy>
    void
    greedy<N, I, external_energy>::
    converge(const I& gradient, snake<greedy<N, I, external_energy> >& s)
    {
      // FIXME: think of a real stop condition.
      unsigned i = 0;
      while (threshold < update_segment(gradient, s.s))
	{
	  average_dist = compute_average_dist(s.s);
	  std::cout << i << ' ' << s.s << std::endl;
	  ++i;
	};
    }

    template <int N, class I, template<typename> class external_energy>
    int
    greedy<N, I, external_energy>::update_segment(const I& gradient,
						  segment<I>& s)
    {
      int nb_updates = 0;
      typename segment<I>::iter_type p = s.begin();
      typename segment<I>::iter_type c = s.begin();
      typename segment<I>::iter_type n = s.begin();

      ++n;
      nb_updates += update_node(gradient, s.prev_node(), *c, *n) ? 1 : 0;
      for (++c, ++n; n != s.end(); ++p, ++c, ++n)
	nb_updates += update_node(gradient, *p, *c, *n) ? 1 : 0;
      nb_updates += update_node(gradient, *p, *c, s.next_node()) ? 1 : 0;
      return nb_updates;
    }

    template <int N, class I, template<typename> class external_energy>
    bool
    greedy<N, I, external_energy>::update_node(const I&		gradient,
					       const node<I>&	prev,
					       node<I>&		current,
					       const node<I>&	next)
      ///< Return whether the point has changed or not.
    {
      store_type		energy;
      ntg::float_s		minimum = ntg_sup_val(ntg::float_s);
      dpoint2d			minimum_location;

      energy = compute_and_normalize_energy<continuity_energy<I> >
	(gradient, prev, current, next, continuity_e) * alpha;
      energy += compute_and_normalize_energy<curvature_energy<I> >
	(gradient, prev, current, next, curvature_e) * beta;
      energy += compute_and_normalize_energy<image_energy<I> >
	(gradient, prev, current, next, image_e) * gamma;
      energy += compute_and_normalize_energy<external_energy<I> >
	(gradient, prev, current, next, external_e) * khi;

      window2d::iter_type it(neighborhood);
      for_all(it)
	{
	  if (minimum > energy(it.cur().col(), it.cur().row()))
	    {
	      minimum = energy(it.cur().col(), it.cur().row());
	      minimum_location = it;
	    }
	}
      if (0 != minimum_location.col() || 0 != minimum_location.row())
	{
	  current += minimum_location;
	  return true;
	}
      return false;
    }


    template <int N, class I, template<typename> class external_energy>
    template <class energy_functor>
    inline
    typename greedy<N, I, external_energy>::store_type
    greedy<N, I, external_energy>::
    compute_and_normalize_energy
    (const typename energy_functor::image_type&		gradient,
     const node<typename energy_functor::image_type>&	prev,
     const node<typename energy_functor::image_type>&	current,
     const node<typename energy_functor::image_type>&	next,
     energy_functor					functor)
    {
      store_type		energy;
      ntg::float_s		energy_min = ntg_sup_val(ntg::float_s);
      ntg::float_s		energy_max = ntg_inf_val(ntg::float_s);

      // Compute energy of each point of the neighborhood. Those are
      // not normalized yet.
      window2d::iter_type it(neighborhood);
      for_all(it)
	{
	  ntg::float_s e = functor.compute(gradient, prev, current + it, next);

	  // Find minimal and maximal energy on the fly, for we need them
	  // thereafter.
	  if (e > energy_max) energy_max = e;
	  if (e < energy_min) energy_min = e;
	  // Store it in a temporary location.
	  energy(it.cur().col(), it.cur().row()) = e;
	}
      if (energy_max > 0)
	{
	  ntg::float_s invmax = 1 / (energy_max - energy_min);
	  window2d::iter_type itw(neighborhood);
	  for_all(itw)
	    {
	      ntg::float_s tmp = energy(itw.cur().col(), itw.cur().row()) -
		energy_min;
	      tmp *= invmax;
	      energy(itw.cur().col(), itw.cur().row()) = tmp;
	    }
	}
      return energy;
    }


    template <int N, class I, template<typename> class external_energy>
    ntg::float_s
    greedy<N, I, external_energy>::compute_average_dist(const segment<I>& s)
    {
      ntg::float_s mean = 0.f;

      typename segment<I>::const_iter_type prev = s.begin();
      typename segment<I>::const_iter_type cur = s.begin();
      unsigned card = 0;
      for (++cur; cur != s.end(); ++prev, ++cur)
	{
	  mean += (*cur - *prev).norm2();
	  ++card;
	}
      return mean / (ntg::float_s)card;
    }


    template <int N, class I, template<typename> class external_energy>
    const int greedy<N, I, external_energy>::threshold = 3; // FIXME:


    template <int N, class I, template<typename> class external_energy>
    window2d greedy<N, I, external_energy>::neighborhood = mk_win_square(N);


  } // end snakes

} // end oln


#endif // !OLENA_SNAKES_GREEDY_HXX
