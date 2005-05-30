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

#ifndef OLENA_MORPHO_SHORTEST_PATH_WATERSHED_HH
# define OLENA_MORPHO_SHORTEST_PATH_WATERSHED_HH

# include <queue>

# include "oln/level/fill.hh"
# include "oln/level/extrema_components.hh"
# include "oln/morpho/lower_completion.hh"

namespace oln {

  namespace morpho {

    namespace internal {

      /// Functor used in ordered queues of points.
      template <typename I>
      class greater_point_type
      {
      public:
	typedef oln_type_of(I, point) point_type;

	greater_point_type(const abstract::image<I>& im) :
	  im_ (im)
	{
	}

	/// Is \a x greater than \a y?
	bool operator()(const point_type& x, const point_type& y)
	{
	  return im_[x] > im_[y];
	}

      private:
	const abstract::image<I>& im_;
      };

      /// Facade to build a oln::level::greater_point_type.
      template <typename I>
      greater_point_type<I>
      greater_point(const abstract::image<I>& im)
      {
	return greater_point_type<I>(im);
      }


      // FIXME: To be rewritten. Moreover, the distance d(p, q) is not
      // taken into account in this version of cost (but it should not
      // bother us as long as we are using first-order neighborhoods).
      template <typename I>
      int
      cost (const abstract::image_with_nbh<I>& im,
	    const oln_type_of(I, point)& p, const oln_type_of(I, point)& q)
      {
	mlc_is_a(I, abstract::scalar_valued_image)::ensure();

	oln_type_of(I, niter) n(im);

	int ls_p = 0;
	for_all_n_of_p (n, p)
	  if (im.hold(n) and int(im[p] - im[n]) > ls_p)
	    ls_p = im[p] - im[n];

	int ls_q = 0;
	for_all_n_of_p (n, q)
	  if (im.hold(n) and int(im[q] - im[n]) > ls_q)
	    ls_q = im[q] - im[n];

	if (im[p] > im[q])
	  return ls_p;
	else if (im[q] > im[p])
	  return ls_q;
	else
	  return (ls_p + ls_q) / 2;
      }

    } // End of namespace internal.


    template <typename DestValue, typename I>
    typename ch_value_type<I, DestValue>::ret
    shortest_path_watershed(const abstract::image_with_nbh<I>& input)
    {
      mlc_is_a(I, abstract::scalar_valued_image)::ensure();

      const DestValue wshed = ntg_min_val(DestValue);

      // Get a lower complete image of the input.
      typename ch_value_type<I, unsigned>::ret lc =
	lower_completion<unsigned>(input);

      // We keep a track of already processed points.
      typename ch_value_type<I, bool>::ret processed (lc.size(),
						      lc.nbh_get());
      level::fill (processed, false);

      // Initialise output with the minima components.
      typename ch_value_type<I, DestValue>::ret output =
	level::minima_components<DestValue>(lc);

      // Distance.
      typedef typename ch_value_type<I, unsigned>::ret dist_type;
      dist_type dist (lc.size(), lc.nbh_get());
      level::fill(dist, ntg_max_val(DestValue));
      // Initialize distance with values of minima, and mark these
      // points as processed (remember that points of LC who have a
      // value greater than ntg_min_val(DestValue) belongs to a
      // minimum).
      oln_type_of(I, piter) p(lc.size());
      for_all_p (p)
	if (output[p].value() > ntg_min_val(DestValue))
	  dist[p] = lc[p];

      // Ordered queue.
      typedef oln_type_of(I, point) point_type;
      typedef
	std::priority_queue<point_type,
			    std::vector<point_type>,
			    internal::greater_point_type<dist_type> >
	ordered_queue_type;
      ordered_queue_type q(internal::greater_point(dist));
      // Fill the ordered queue with the points of the border of the
      // minima of the (lower complete) input.
      for_all_p (p)
	if (output[p].value() > ntg_min_val(DestValue))
	  {
	    bool border_p = false;
	    oln_type_of(I, niter) n(lc);
	    for_all_n_of_p (n, p)
	      if (lc.hold(n) and output[n].value() == ntg_min_val(DestValue))
		{
		  // P is both in a minima and adjacent to a non minima:
		  // it's on the border of a minima.
		  border_p = true;
		  break;
		};
	    if (border_p)
	      q.push(p);
	    else
	      // Inner points of minima have already been processed.
	      processed[p] = true;
	  }

      while (!q.empty())
	{
	  point_type p = q.top();
	  q.pop();
	  if (processed[p])
	    continue;
	  processed[p] = true;

	  oln_type_of(I, niter) n(lc);
	  for_all_n_of_p (n, p)
	    {
	      if (lc.hold(n))
		if (dist[p] + internal::cost(lc, p, n) < dist[n])
		  {
		    dist[n] = dist[p] + internal::cost(lc, p, n);
		    output[n] = output[p];
		    q.push(n);

		  }
		else if (output[n] != wshed and
			 dist[p] + internal::cost(lc, p, n) == dist[n] and
			 output[n] != output[p])
		  {
		    output[n] = wshed;
		    q.push(n);
		  }
	    }
	}

      return output;
    }

  } // end of namespace oln::morpho

} // end of namespace oln

#endif // ! OLENA_MORPHO_SHORTEST_PATH_WATERSHED_HH
