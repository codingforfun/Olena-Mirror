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

#ifndef OLENA_MORPHO_WATERSHED_HXX
# define OLENA_MORPHO_WATERSHED_HXX

# include <oln/config/system.hh>
# include <ntg/all.hh>
# include <oln/level/fill.hh>
# include <queue>
# include <map>

namespace oln {
  namespace morpho {

    /* GCC's optimizer is smart enough to compute these values at compile
       time and really use them as constants.  That's great.  */
# define oln_morpho_declare_soille_watershed_consts_(DestValue)				\
    const DestValue mask    = ntg::cast::force<DestValue>(ntg_max_val(DestValue) - 2);	\
    const DestValue wshed   = ntg_max_val(DestValue);					\
    const DestValue init    = ntg::cast::force<DestValue>(ntg_max_val(DestValue) - 1);	\
    const DestValue inqueue = ntg::cast::force<DestValue>(ntg_max_val(DestValue) - 3);	\
    const DestValue maxlevel = ntg::cast::force<DestValue>(inqueue - 1) /* no ; */

    namespace internal {

      struct watershed_seg_point_handler_
      {
	template<class Fifo, class II, class IO, class N>
	static inline void
	process (Fifo& fifo, const II&, IO& im_o, const N& Ng)
	{
	  typedef oln_value_type(IO) destvalue_type;
	  oln_morpho_declare_soille_watershed_consts_(destvalue_type);
	  (void) init; /* unused */

	  oln_point_type(II) p = fifo.front();
	  fifo.pop();

	  oln_iter_type(N) p_prime(Ng);

	  bool flag = false;
	  for_all (p_prime)
	    if (im_o.hold(p + p_prime))
	      {
		if (im_o[p + p_prime] <= maxlevel)
		  {
		    // (p + p_prime) belongs to an already labelled basin
		    if (im_o[p] == inqueue
			|| (im_o[p] == wshed && flag == true))
		      im_o[p] = im_o[p + p_prime];
		    else if (im_o[p] <= maxlevel
			     && (im_o[p] != im_o[p + p_prime]))
		      {
			im_o[p] = wshed;
			flag = false;
		      }
		  }
		else if (im_o[p + p_prime] == wshed)
		  {
		    if (im_o[p] == inqueue)
		      {
			im_o[p] = wshed;
			flag = true;
		      }
		  }
		else if (im_o[p + p_prime] == mask)
		  {
		    im_o[p + p_prime] = inqueue;
		    fifo.push(p + p_prime);
		  }
	      }
	}
      };

      struct watershed_con_point_handler_
      {
	template<class Fifo, class II, class IO, class N>
	static inline void
	process (Fifo& fifo, const II& im_i, IO& im_o, const N& Ng)
	{
	  typedef oln_value_type(IO) destvalue_type;
	  oln_morpho_declare_soille_watershed_consts_(destvalue_type);
	  (void) init; (void) wshed; /* unused */
	  
	  oln_point_type(II) p = fifo.front();
	  fifo.pop();

	  oln_neighb_type(N) p_prime(Ng, p);

	  if (im_o[p] == inqueue)
	    {
	      // Find the labeled neighbor with the lower level
	      for_all(p_prime)
		if (im_o.hold(p_prime) && im_o[p_prime] <= maxlevel)
		  {
		    oln_point_type(II) l = p_prime;
		    
		    for_all_remaining(p_prime)
		      if (im_o.hold(p_prime)
			  && im_o[p_prime] <= maxlevel
			  && im_i[p_prime] < im_i[l])
			l = p_prime;

		    im_o[p] = im_o[l];
		    break;
		  }
	    }

	  /* Enqueue all masked neighbors */
	  for_all (p_prime)
	    if (im_o.hold(p_prime) && im_o[p_prime] == mask)
	      {
		im_o[p_prime] = inqueue;
		fifo.push(p_prime);
	      }
	}
      };
      
      template<class Point, class T> inline
      bool 
      watershed_seg_sort_(const std::pair<Point, T>& p1,
			  const std::pair<Point, T>& p2)
      {
	return p1.second < p2.second;
      }

      // Algorithm by Vincent and Soille
      template<class PointHandler, class DestValue, class I, class N>
      typename mute<I, DestValue>::ret
      soille_watershed_(const abstract::non_vectorial_image<I>& im_i, 
			const abstract::neighborhood<N>& Ng)
      {
	oln_morpho_declare_soille_watershed_consts_(DestValue);

	// Initializations
	typedef typename mute<I, DestValue>::ret result_type;
	result_type im_o(im_i.size());
	level::fill(im_o, init);
	std::queue<oln_point_type(I) > fifo;
	DestValue current_label = ntg_min_val(DestValue);

	// Sort the pixels of im_i in the increasing order of their grey
	// values.
	std::vector< std::pair<oln_point_type(I),oln_value_type(I)> >
	  histo(im_i.npoints());

	{
	  oln_iter_type(I) p(im_i);
	  unsigned int i = 0;
	  for_all (p)
	    histo[i++] = std::pair<oln_point_type(I),oln_value_type(I)>(p, im_i[p]);
	  // FIXME: We don't use a distrib_sort for now because that would
	  // not work with float values.  We should have a specialized
	  // sorting proc to handle this in the future.
	  sort(histo.begin(), histo.end(),
	       watershed_seg_sort_<oln_point_type(I),oln_value_type(I)>);
	}

	for (unsigned int i = 0; i < histo.size(); )
	  // geodesic SKIZ of level h-1 inside level h
	  {
	    unsigned int level_start = i;	// First index for current level.
	    
	    oln_value_type(I) h = histo[i].second;
	    
	    oln_point_type(I) p;
	    while (i < histo.size())
	      {
		p = histo[i].first;
		if (im_i[p] != h)
		  break;

		im_o[p] = mask;
		oln_iter_type(N) p_prime(Ng);
		bool flag_exist = false;
		for_all (p_prime)
		  if (im_o.hold(p + p_prime))
		    {
		      if (im_o[p + p_prime] == wshed
			  || im_o[p + p_prime] <= maxlevel)
			flag_exist = true;
		    }
		if (flag_exist == true)
		  {
		    im_o[p] = inqueue;
		    fifo.push(p);
		  }
		
		++i;
	      }
	    
	    // ================================================================
	    
	    while (!fifo.empty())
	      PointHandler::process (fifo, im_i, im_o, Ng);
	    
	    // ================================================================
	    
	    /* Check for new minima.  */
	    
	    unsigned int j = level_start;
	    while (j < histo.size())
	      {
		p = histo[j].first;
		if (im_i[p] != h)
		  break;
		
		if (im_o[p] == mask)
		  {
		    current_label += 1;
		    /* If we run out of labels, restart from the initial
		       value.  FIXME: We should have a mean to tell
		       the caller that such 'wrapping' occured.  */
		    if (current_label > maxlevel)
		      current_label = ntg_min_val(DestValue);
		    
		    fifo.push(p);
		    im_o[p] = current_label;
		    while (!fifo.empty())
		      {
			oln_point_type(I) p_prime = fifo.front();
			fifo.pop();
			oln_iter_type(N) p_pprime(Ng);
			for (p_pprime = begin; p_pprime != end; ++ p_pprime)
			  if (im_o.hold(p_prime + p_pprime)
			      && im_o[p_prime + p_pprime] == mask)
			    {
			      fifo.push(p_prime + p_pprime);
			      im_o[p_prime + p_pprime] = current_label;
			    }
		      }
		  }
		++j;
	      }
	  }
	return im_o;
      }

    } // internal


    // Algorithm by Vincent and Soille
    template<class DestValue, class I, class N>
    typename mute<I, DestValue>::ret
    watershed_seg(const abstract::non_vectorial_image<I>& im_i,
		  const abstract::neighborhood<N>& Ng)
    {
      return internal::soille_watershed_<
	internal::watershed_seg_point_handler_, DestValue> (im_i, Ng);
    }
    
    template<class DestValue, class I, class N>
    typename mute<I, DestValue>::ret
    watershed_con(const abstract::non_vectorial_image<I>& im_i, 
		  const abstract::neighborhood<N>& Ng)
    {
      return internal::soille_watershed_<
	internal::watershed_con_point_handler_, DestValue> (im_i, Ng);
    }


    // cmp_queue_elt is a comparison function for the elements from
    // the priority queue used in watershed_seg_or.  Note that we
    // return true when l is greater than r, because we when the queue
    // sorted in increasing order.
    template <class T>
    struct cmp_queue_elt
    {
      bool 
      operator()(const std::pair<oln_point_type(T), oln_value_type(T)>& l,
		 const std::pair<oln_point_type(T), oln_value_type(T)>& r) const
      {
	return l.second > r.second;
      }
    };

    // version by D'Ornellas et al.
    template<class I1, class I2, class N> inline
    oln_concrete_type(I2)&
    watershed_seg_or(const abstract::non_vectorial_image<I1>& In,
		     abstract::non_vectorial_image<I2>& Labels,
		     const abstract::neighborhood<N>& Ng)
    {

      typedef oln_value_type(I2) value_type;
      const oln_value_type(I2) wshed   = ntg_max_val(value_type);
      const oln_value_type(I2) unknown = ntg_min_val(value_type);

      typedef std::pair<oln_point_type(I1), oln_value_type(I1)> queue_elt_type;
      std::priority_queue< queue_elt_type, std::vector< queue_elt_type >,
	cmp_queue_elt<I1> > PQ;

      // Initialization
      // Enqueue all labeled points which have a unlabeled neighbor.
      {
	oln_iter_type(I2) p(Labels);
	oln_neighb_type(N) p_prime(Ng, p);
	for_all(p)
	  if (Labels[p] != unknown)
	    for_all (p_prime)
	      if (Labels.hold(p_prime) && Labels[p_prime] == unknown)
		{
		  PQ.push(queue_elt_type(p, In[p]));
		  break;
		}
      }

      // Propagation

      oln_point_type(I1) p;
      oln_neighb_type(N) p_prime(Ng, p);
      oln_neighb_type(N) p_second(Ng, p_prime);
      while (! PQ.empty())
	{
	  // Get the lowest point in the queue.
	  p = PQ.top().first;
	  PQ.pop();
	  // Consider all neighbors of p.
	  for_all (p_prime) if (Labels.hold(p_prime))
	    {
	      // Focus on unlabeled neighbors of p.
	      if (Labels[p_prime] == unknown)
		{
		  // Since p_prime is a neighbor of p, it should
		  // be either labeled using the same label as p,
		  // or marked as watershed.

		  // It's a watershed if, among the neighbors of
		  // p_prime (which itself is a neighbor of p), there
		  // exists a point with a label different from the
		  // label of pt.  EXISTS is set to true in this case.
		  bool exists = false;
		  for_all (p_second)
		    // FIXME: We should not need the iterate over
		    // the neighbors of p_prime which are also
		    // neighbors of p, since none of these can have
		    // have a different label.  It should be possible
		    // to precompute an array of the resulting windows
		    // for each neighbor (of p).
		    if (Labels.hold(p_second)
			&& Labels[p_second] != unknown
			&& Labels[p_second] != wshed
			&& Labels[p_second] != Labels[p])
		      {
			exists = true;
			break;
		      }
		  if (exists)
		    Labels[p_prime] = wshed;
		  else
		    {
		      Labels[p_prime] = Labels[p];
		      PQ.push(queue_elt_type(p_prime, In[p_prime]));
		    }
		}
	    }
	}
      return Labels.exact();
    }

  } // morpho
} // oln


#endif // OLENA_MORPHO_WATERSHED_HXX
