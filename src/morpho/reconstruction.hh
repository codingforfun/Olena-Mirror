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

#ifndef OLENA_MORPHO_RECONSTRUCTION_HH
# define OLENA_MORPHO_RECONSTRUCTION_HH

# include  <queue>
# include "basics.hh"
# include "morpho/stat.hh"
# include "morpho/geodesic_dilation.hh"
# include "morpho/splitse.hh"
# include "level/compare.hh"
# include "value/ops.hh"

namespace oln {
  namespace morpho {

    template<class _I1, class _I2, class _E>
    Concrete(_I1)
    sure_geodesic_reconstruction_dilation(const image<_I1> & _marker,
					  const image<_I2> & _mask,
					  const struct_elt<_E>& se)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(mask, marker));
      Concrete(_I1) output = marker.clone();
      bool non_stability = true;
      while (non_stability)
	{
	  Concrete(_I1) work = simple_geodesic_dilation(output, mask, se);
	  non_stability = !(level::is_equal(work, output));
	  output = work;
	}
      return output;
    }


    template<class _I1, class _I2, class _E> inline
    Concrete(_I1)
    sequential_geodesic_reconstruction_dilation(const image<_I1> & _marker,
						const image<_I2> & _mask,
						const struct_elt<_E>& _se)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(E, se);
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(mask, marker));
      E se_plus = get_plus_se_p(se);
      E se_minus = get_minus_se_p(se);
      Concrete(I1) output = marker.clone();

      bool non_stability = true;
      typename I1::fwd_iter fwd_p(output);
      typename I1::bkd_iter bkd_p(output);
      while (non_stability)
	{
	  Concrete(I1) work = output.clone();
	  border::adapt_copy(work, se.delta());
	  for_all (fwd_p)
	    work[fwd_p] = min(morpho::max(work, fwd_p, se_plus), mask[fwd_p]);
  	  for_all (bkd_p)
	    work[bkd_p] = min(morpho::max(work, bkd_p, se_minus), mask[bkd_p]);
	  non_stability = !(level::is_equal(work, output));
	  output = work;
	}
      return output;
    }


    template<class _P, class _I1, class _I2, class _E> inline
    static
    bool exist_init_dilation(const point<_P>& _p,
			     const image<_I1>& _marker,
			     const image<_I2>& _mask,
			     const struct_elt<_E>& _se)
    {
      Exact_cref(P, p);
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(E, se);

      Neighb(E) q(se, p);
      for_all (q)
	if (marker.hold(q) && (marker[q] < marker[p]) && (marker[q] < mask[q]))
	  return true;
      return false;
    }


    template<class _I1, class _I2, class _E> inline
    Concrete(_I1)
    hybrid_geodesic_reconstruction_dilation(const image<_I1> & _marker,
					    const image<_I2> & _mask,
					    const struct_elt<_E>& _Ng)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(E, Ng);

      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(mask, marker));

      Concrete(I1) output = marker.clone();
      border::adapt_copy(output, Ng.delta());
      {
	E Ng_plus = get_plus_se_p(Ng);
	E Ng_minus = get_minus_se_p(Ng);
	typename I1::fwd_iter fwd_p(output);
	typename I1::bkd_iter bkd_p(output);
	for_all (fwd_p)
	  output[fwd_p] = min(morpho::max(output, fwd_p, Ng_plus),
			      mask[fwd_p]);

	std::queue< Point(I1) > fifo;
	for_all (bkd_p)
	  {
	    output[bkd_p] = min(morpho::max(output, bkd_p, Ng_minus),
				mask[bkd_p]);
	    if (exist_init_dilation(bkd_p.cur(), output, mask, Ng_minus))
	      fifo.push(bkd_p);
	  }
	// Propagation Step
	while (!fifo.empty())
	  {
	    Point(I1) p = fifo.front();
	    fifo.pop();
	    Neighb(E) q(Ng, p);
	    for_all (q) if (output.hold(q))
	      {
		if ((output[q] < output[p]) && (mask[q] != output[q]))
		  {
		    output[q] = min(output[p], mask[q]);
		    fifo.push(q);
		  }
	      }
	  }
      }
      return output;
    }

    //GEODESIC RECONSTRUCTION EROSION

    template<class _I1, class _I2, class _E>
    Concrete(_I1)
    sure_geodesic_reconstruction_erosion(const image<_I1> & _marker,
					 const image<_I2> & _mask,
					 const struct_elt<_E>& se)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(marker, mask));
      Concrete(_I1) output = marker.clone();
      bool non_stability = true;
      while (non_stability)
	{
	  Concrete(_I1) work = simple_geodesic_erosion(output, mask, se);
	  non_stability = !(level::is_equal(work, output));
	  output = work;
	}
      return output;
    }

    template<class _I1, class _I2, class _E> inline
    Concrete(_I1)
    sequential_geodesic_reconstruction_erosion(const image<_I1>& _marker,
					       const image<_I2>& _mask,
					       const struct_elt<_E>& _se)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(E, se);
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(marker, mask));
      E se_plus = get_plus_se_p(_se);
      E se_minus = get_minus_se_p(se);
      Concrete(I1) output = marker.clone();

      bool non_stability = true;
      typename I1::fwd_iter fwd_p(output);
      typename I1::bkd_iter bkd_p(output);
      while (non_stability)
	{
	  Concrete(I1) work = output.clone();
	  border::adapt_copy(work, se.delta());
	  for_all (fwd_p)
	    work[fwd_p] = max(morpho::min(work, fwd_p, se_plus), mask[fwd_p]);
  	  for_all (bkd_p)
	    work[bkd_p] = max(morpho::min(work, bkd_p, se_minus), mask[bkd_p]);
	  non_stability = !(level::is_equal(work, output));
	  output = work;
	}
      return output;
    }


    template<class _P, class _I1, class _I2, class _E> inline
    static
    bool exist_init_erosion(const point<_P>& _p,
			    const image<_I1>& _marker,
			    const image<_I2>& _mask,
			    const struct_elt<_E>& _se)
    {
      Exact_cref(P, p);
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(E, se);

      Neighb(E) q(se, p);
      for_all (q)
	if (marker.hold(q) && (marker[q] > marker[p]) && (marker[q] > mask[q]))
	  return true;
      return false;
    }


    template<class _I1, class _I2, class _E> inline
    Concrete(_I1)
    hybrid_geodesic_reconstruction_erosion(const image<_I1> & _marker,
					   const image<_I2> & _mask,
					   const struct_elt<_E>& _Ng)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(E, Ng);

      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(marker, mask));

      Concrete(I1) output = marker.clone();
      border::adapt_copy(output, Ng.delta());
      {
     	E Ng_plus = get_plus_se_p(Ng);
     	E Ng_minus = get_minus_se_p(Ng);
     	typename I1::fwd_iter fwd_p(output);
     	typename I1::bkd_iter bkd_p(output);
     	for_all (fwd_p)
     	  output[fwd_p] = max(morpho::min(output, fwd_p, Ng_plus),
			      mask[fwd_p]);

     	std::queue< Point(I1) > fifo;
     	for_all (bkd_p)
     	  {
     	    output[bkd_p] = max(morpho::min(output, bkd_p, Ng_minus),
				mask[bkd_p]);
     	    if (exist_init_erosion(bkd_p.cur(), output, mask, Ng_minus))
     	      fifo.push(bkd_p);
     	  }
	//  	 Propagation Step
     	while (!fifo.empty())
     	  {
     	    Point(I1) p = fifo.front();
     	    fifo.pop();
     	    Neighb(E) q(Ng, p);
     	    for_all (q) if (output.hold(q))
     	      {
     		if ((output[q] > output[p]) && (mask[q] != output[q]))
     		  {
     		    output[q] = max(output[p], mask[q]);
     		    fifo.push(q);
     		  }
     	      }
     	  }
      }
      return output;
    }

  } // morpho
} // oln

#endif // OLENA_MORPHO_RECONSTRUCTION_HH
