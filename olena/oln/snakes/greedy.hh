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

#ifndef OLENA_SNAKES_GREEDY_HH
# define OLENA_SNAKES_GREEDY_HH

#include <oln/snakes/snakes_base.hh>
#include <oln/snakes/energies.hh>
#include <mlc/array/2d.hh>

namespace oln {

  namespace snakes {

    /// N is the size of the neighborhood.
    template <int N, class I, template<typename> class external_energy = dummy_energy>
    class greedy
    {
    public:
      typedef I							 image_type;
      typedef typename I::point_type				 point_type;
      typedef mlc::array2d<mlc::array2d_info<N,N>, ntg::float_s> store_type;
      // FIXME: why only 2d?

    public:
      greedy(ntg::float_s alpha,
	     ntg::float_s beta,
	     ntg::float_s gamma,
	     ntg::float_s khi);

    public:
      inline
      int
      update_snake(const I& gradient, snake<greedy>& s);
      ///< Asynchronous update for more efficient convergence.

      void
      converge(const I& gradient, snake<greedy>& s);


    private:
      inline
      int
      update_segment(const I& gradient, segment<I>& s);

      inline
      bool
      update_node(const I& gradient,
		  const node<I>& prev, node<I>& n, const node<I>& next);

    private:
      template <class energy_functor>
      inline
      store_type
      compute_and_normalize_energy
      (const typename energy_functor::image_type& gradient,
       const node<typename energy_functor::image_type>& prev,
       const node<typename energy_functor::image_type>& current,
       const node<typename energy_functor::image_type>& next,
       energy_functor functor);

    private:
      ntg::float_s average_dist;
      ///< Average distance between the points of the contour.

      inline
      ntg::float_s
      compute_average_dist(const segment<I>& s);

    private:
      continuity_energy<I>	continuity_e;
      curvature_energy<I>	curvature_e;
      image_energy<I>		image_e;
      external_energy<I>	external_e;

    private:
      ///< Each energy is weighted by a its own coefficient.
      ntg::float_s alpha; ///< Weight of the continuity energy.
      ntg::float_s beta;  ///< Weight of the curvature energy.
      ntg::float_s gamma; ///< Weight of the image energy.
      ntg::float_s khi;   ///< Weight of the image energy.

    private:
      static const int	threshold;
      // FIXME: think about a real stop condition.
    private:
      static window2d	neighborhood;
      // FIXME: better write an iterator on arrays.
    };

  } // end snakes

} // end oln

#include <oln/snakes/greedy.hxx>

#endif // !OLENA_SNAKES_GREEDY_HH
