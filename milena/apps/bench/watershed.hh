// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef APPS_BENCH_WATERSHED_HH
# define APPS_BENCH_WATERSHED_HH

/// \file
/// \brief Watershed transform by flooding benchmark cases.
///
/// Generic code extracted from mln/morpho/watershed/flooding.hh and
/// modified.

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/window2d.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/label.hh>

# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>

# include <mln/literal/zero.hh>
# include <mln/labeling/regional_minima.hh>

# include <mln/core/site_set/p_queue_fast.hh>
# include <mln/core/site_set/p_priority.hh>

# include <mln/extension/adjust_fill.hh>

# include <mln/util/timer.hh>

# include "apps/bench/static_neighborhood.hh"
# include "apps/bench/static_dpoints_pixter.hh"
# include "apps/bench/trait.hh"


/* FIXME: We should not take into account the time passed into
   labeling::regional_minima, since this function is the same for in
   versions of the algorithm.  */

/*----------------------.
| Non-generic version.  |
`----------------------*/

namespace nongen
{
  using namespace mln;

  typedef mln::image2d<mln::value::int_u8> image;
  typedef mln::value::label<24> label;
  typedef mln::image2d<label> labels;

  labels
  watershed(const image& input, label& n_basins)
  {
    // FIXME: Simplify the body of this routine even more?

    typedef label marker;
    const marker unmarked = 0;

    typedef mln_value_(image) V;
    const V max = mln_max(V);

    // Initialize the output with the markers (minima components).
    typedef labels O;
    O output = labeling::regional_minima(input, c4(), n_basins);

    // Ordered queue.
    typedef p_queue_fast<point2d> Q;
    p_priority<V, Q> queue;

    // In_queue structure to avoid processing sites several times.
    mln_ch_value_(image, bool) in_queue;
    initialize(in_queue, input);
    data::fill(in_queue, false);


    // Insert every neighbor (R, C) of every marked area in a
    // hierarchical queue, with a priority level corresponding to
    // the grey level input.at_(R, C).
    for (unsigned int r = 0; r < input.nrows(); ++r)  // Iterate on rows.
      for (unsigned int c = 0; c < input.ncols(); ++c)  // Iterate on columns.
      {
	if (output.at_(r, c) == unmarked)
	  {
	    if ((   r != 0                 && output.at_(r-1, c) != unmarked)
		|| (c != 0                 && output.at_(r, c-1) != unmarked)
		|| (c != input.ncols() - 1 && output.at_(r, c+1) != unmarked)
		|| (r != input.nrows() - 1 && output.at_(r+1, c) != unmarked))
	    {
	      queue.push(max - input.at_(r, c), point2d(r, c));
	      in_queue.at_(r, c) = true;
	    } 
	  }
      }

    /* Until the queue is empty, extract a point (R, C) from the
       hierarchical queue, at the highest priority level, that is,
       the lowest level.  */
    while (! queue.is_empty())
      {
	point2d p = queue.front();
	queue.pop();
	unsigned int r = p.row();
	unsigned int c = p.col();

	// Last seen marker adjacent to (R, C).
	marker adjacent_marker = unmarked;
	// Has (R, C) a single adjacent marker?
	bool single_adjacent_marker_p = true;

	const size_t nneighbs = 4;
	point2d neighb[nneighbs] = { point2d(r-1, c),
				     point2d(r, c-1),
				     point2d(r, c+1),
				     point2d(r+1, c) };
	for (size_t n = 0; n < nneighbs; ++n)
	  if (output.domain().has(neighb[n]) && output(neighb[n]) != unmarked)
	    {
	      if (adjacent_marker == unmarked)
		{
		  adjacent_marker = output(neighb[n]);
		  single_adjacent_marker_p = true;
		}
	      else
		if (adjacent_marker != output(neighb[n]))
		  {
		    single_adjacent_marker_p = false;
		    break;
		  }
	    }
	/* If the neighborhood of (R, C) contains only points with the
	   same label, then (R, C) is marked with this label, and its
	   neighbors that are not yet marked are put into the
	   hierarchical queue.  */
	if (single_adjacent_marker_p)
	  {
	    output.at_(r, c) = adjacent_marker;
	    for (size_t n = 0; n < nneighbs; ++n)
	      if (output.domain().has(neighb[n])
		  && output(neighb[n]) == unmarked
		  && ! in_queue(neighb[n]))
		{
		  queue.push(max - input(neighb[n]), neighb[n]);
		  in_queue(neighb[n]) = true;
		}
	  }
      }

    return output;
  }
}

/*--------------------------------------------------.
| Non-generic version, using up to three pointers.  |
`--------------------------------------------------*/

namespace nongen_3ptr
{
  using namespace mln;

  typedef mln::image2d<mln::value::int_u8> image;
  typedef mln::value::label<24> label;
  typedef mln::image2d<label> labels;

  labels
  watershed(const image& input, label& n_basins)
  {
    // FIXME: Simplify the body of this routine even more?

    typedef label marker;
    const marker unmarked = 0;

    typedef mln_value_(image) V;
    const V max = mln_max(V);


    // Initialize the output with the markers (minima components).
    typedef label L;
    typedef labels O;
    O output = labeling::regional_minima(input, c4(), n_basins);

    const size_t nneighbs = 4;
    // Offsets corresponding to a 4-c neighborhood on OUTPUT.
    ptrdiff_t nbh_offset[nneighbs] =
      { &output.at_(-1,  0) - &output.at_(0, 0),
	&output.at_( 0, -1) - &output.at_(0, 0),
	&output.at_( 0, +1) - &output.at_(0, 0),
	&output.at_(+1,  0) - &output.at_(0, 0) };

    // Ordered queue.
    typedef p_queue_fast<L*> Q;
    p_priority<V, Q> queue;

    // In_queue structure to avoid processing sites several times.
    mln_ch_value_(image, bool) in_queue;
    initialize(in_queue, input);
    data::fill(in_queue, false);

    // Offset between a the pixel located at the same position in
    // OUTPUT and INPUT.
    const ptrdiff_t input_offset =
      (char*)(&input.at_(0, 0)) - (char*)(&output.at_(0, 0));
    // Offset between a the pixel located at the same position in
    // OUTPUT and IN_QUEUE.
    const ptrdiff_t in_queue_offset =
      (char*)(&in_queue.at_(0, 0)) - (char*)(&output.at_(0, 0));

    // Insert every neighbor of every marked area in a hierarchical
    // queue, with a priority level corresponding to the grey level of
    // this area.
    for (unsigned int r = 0; r < output.nrows(); ++r)  // Iterate on rows.
      {
	const V* pi = &input.at_(r, 0);
	L* po = &output.at_(r, 0);
	bool* pq = &in_queue.at_(r, 0);
	for (; pi < &input.at_(r, 0) + input.ncols(); ++pi, ++po, ++pq)
	{
	  if (*po == unmarked)
	    {
	      if (
		  // (-1, 0) neighbor.
		  (   r != 0
		      && *(po + nbh_offset[0]) != unmarked)
		  // (0, -1) neighbor.
		  || (po != &output.at_(r, 0)
		      && *(po + nbh_offset[1]) != unmarked)
		  // (0, +1) neighbor.
		  || (po != &output.at_(r, 0) + output.ncols() - 1
		      && *(po + nbh_offset[2]) != unmarked)
		  // (+1, 0) neighbor.
		  || (r != output.nrows() - 1 
		      && *(po + nbh_offset[3]) != unmarked))
		{
		  queue.push(max - *pi, po);
		  *pq = true;
		} 
	    }
	}
      }

    /* Until the queue is empty, extract a point (R, C) from the
       hierarchical queue, at the highest priority level, that is,
       the lowest level.  */
    while (! queue.is_empty())
      {
	L* po = queue.front();
	queue.pop();

	// Last seen marker adjacent to the current point.
	marker adjacent_marker = unmarked;
	// Has the current point a single adjacent marker?
	bool single_adjacent_marker_p = true;

	for (size_t n = 0; n < nneighbs; ++n)
	  {
	    L* no = po + nbh_offset[n];
	    // In the border, OUTPUT is unmarked so N is ignored.
	    if (*no != unmarked)
	    {
	      if (adjacent_marker == unmarked)
		{
		  adjacent_marker = *no;
		  single_adjacent_marker_p = true;
		}
	      else
		if (adjacent_marker != *no)
		  {
		    single_adjacent_marker_p = false;
		    break;
		  }
	    }
	  }
	/* If the neighborhood of the current point contains only
	   points with the same label, then this point is marked with
	   this label, and its neighbors that are not yet marked are
	   put into the hierarchical queue.  */
	if (single_adjacent_marker_p)
	  {
	    *po = adjacent_marker;
	    for (size_t n = 0; n < nneighbs; ++n)
	      {
		L* no = po + nbh_offset[n];
		/* Deduce the pointers NI and NQ corresponding to NO
		   in IN_QUEUE and INPUT using the memory offsets
		   bewteen each of these images and OUPUT.  This works
		   because the data of qll these images are arranged
		   similarly.

		   The ugly casts to and back from char* are required,
		   since the ptrdiff_t values stored in INPUT_OFFSET
		   and IN_QUEUE_OFFSET are expressed as differences
		   between chars.  */
		V* ni = (V*)((char*) no + input_offset);
		// FIXME: Remove when OK.
		assert(&input.at_(0, 0) < ni);
		assert(ni < &input.element(input.nelements() - 1));
		bool* nq = (bool*)((char*) no + in_queue_offset);
		// FIXME: Remove when OK.
		assert(&in_queue.at_(0, 0) < nq);
		assert(nq < &in_queue.element(in_queue.nelements() - 1));
		if (*no == unmarked && ! *nq)
		  {
		    queue.push(max - *ni, no);
		    *nq = true;
		  }
	      }
	  }
      }

    return output;
  }
}


/*----------------------------------------------.
| Non-generic version, using a single pointer.  |
`----------------------------------------------*/

namespace nongen_2ptr
{
  // FIXME: To do.
}


/*------------------.
| Generic version.  |
`------------------*/

namespace gen
{
  using namespace mln;

  template <typename L, typename I, typename N>
  mln_ch_value(I, L)
  watershed(const I& input_, const Neighborhood<N>& nbh_, L& n_basins)
  {
    const I input = exact(input_);
    const N nbh = exact(nbh_);

    typedef L marker;
    const marker unmarked = literal::zero;

    typedef mln_value(I) V;
    const V max = mln_max(V);

    // Initialize the output with the markers (minima components).
    typedef mln_ch_value(I, L) O;
    O output = labeling::regional_minima(input, nbh, n_basins);

    typedef mln_psite(I) psite;

    // Ordered queue.
    typedef p_queue_fast<psite> Q;
    p_priority<V, Q> queue;

    // In_queue structure to avoid processing sites several times.
    mln_ch_value(I, bool) in_queue;
    initialize(in_queue, input);
    data::fill(in_queue, false);

    // Insert every neighbor P of every marked area in a
    // hierarchical queue, with a priority level corresponding to
    // the grey level input(P).
    mln_piter(I) p(output.domain());
    mln_niter(N) n(nbh, p);
    for_all(p)
      if (output(p) == unmarked)
	for_all(n)
	  if (output.domain().has(n) && output(n) != unmarked)
	    {
	      queue.push(max - input(p), p);
	      in_queue(p) = true;
	      break;
	    }

    /* Until the queue is empty, extract a psite P from the
       hierarchical queue, at the highest priority level, that is,
       the lowest level.  */
    while (! queue.is_empty())
      {
	psite p = queue.front();
	queue.pop();

	// Last seen marker adjacent to P.
	marker adjacent_marker = unmarked;
	// Has P a single adjacent marker?
	bool single_adjacent_marker_p = true;
	mln_niter(N) n(nbh, p);
	for_all(n)
	  if (output.domain().has(n) && output(n) != unmarked)
	    {
	      if (adjacent_marker == unmarked)
		{
		  adjacent_marker = output(n);
		  single_adjacent_marker_p = true;
		}
	      else
		if (adjacent_marker != output(n))
		  {
		    single_adjacent_marker_p = false;
		    break;
		  }
	    }
	/* If the neighborhood of P contains only psites with the
	   same label, then P is marked with this label, and its
	   neighbors that are not yet marked are put into the
	   hierarchical queue.  */
	if (single_adjacent_marker_p)
	  {
	    output(p) = adjacent_marker;
	    for_all(n)
	      if (output.domain().has(n) && output(n) == unmarked
		  && ! in_queue(n))
		{
		  queue.push(max - input(n), n);
		  in_queue(n) = true;
		}
	  }
      }

    return output;
  }
}

/*-----------------------------------------------.
| Fast version (with one pixter), less generic.  |
`-----------------------------------------------*/

namespace fast
{
  using namespace mln;

  template <typename I, typename N, typename L>
  mln_ch_value(I, L)
  watershed(const Image<I>& input_, const Neighborhood<N>& nbh_,
	    L& n_basins)
  {
    const I input = exact(input_);
    const N nbh = exact(nbh_);

    typedef L marker;
    const marker unmarked = literal::zero;

    typedef mln_value(I) V;
    const V max = mln_max(V);

    extension::adjust_fill(input, nbh, max);

    // Initialize the output with the markers (minima components).
    typedef mln_ch_value(I, L) O;
    O output = labeling::regional_minima(input, nbh, n_basins);
    extension::fill(output, unmarked);

    // Ordered queue.
    typedef p_queue_fast<unsigned> Q;
    p_priority<V, Q> queue;

    // In_queue structure to avoid processing sites several times.
    mln_ch_value(I, bool) in_queue;
    initialize(in_queue, input);
    data::fill(in_queue, false);
    extension::fill(in_queue, true);

    // Insert every neighbor P of every marked area in a
    // hierarchical queue, with a priority level corresponding to
    // the grey level input(P).
    mln_pixter(const O)    p_out(output);
    mln_nixter(const O, N) n_out(p_out, nbh);
    for_all(p_out)
      if (p_out.val() == unmarked)
	for_all(n_out)
	  if (n_out.val() != unmarked)
	    {
	      unsigned po = p_out.offset();
	      queue.push(max - input.element(po), po);
	      in_queue.element(po) = true;
	      break;
	    }

    /* Until the queue is empty, extract a psite P from the
       hierarchical queue, at the highest priority level, that is,
       the lowest level.  */
    util::array<int> dp = offsets_wrt(input, nbh);
    const unsigned n_nbhs = dp.nelements();
    while (! queue.is_empty())
      {
	unsigned p = queue.front();
	queue.pop();

	// Last seen marker adjacent to P.
	marker adjacent_marker = unmarked;
	// Has P a single adjacent marker?
	bool single_adjacent_marker_p = true;
	for (unsigned i = 0; i < n_nbhs; ++i)
	  {
	    unsigned n = p + dp[i];
	    // In the border, OUTPUT is unmarked so N is ignored.
	    if (output.element(n) != unmarked)
	      {
		if (adjacent_marker == unmarked)
		  {
		    adjacent_marker = output.element(n);
		    single_adjacent_marker_p = true;
		  }
		else
		  if (adjacent_marker != output.element(n))
		    {
		      single_adjacent_marker_p = false;
		      break;
		    }
	      }
	  }
	/* If the neighborhood of P contains only psites with the
	   same label, then P is marked with this label, and its
	   neighbors that are not yet marked are put into the
	   hierarchical queue.  */
	if (single_adjacent_marker_p)
	  {
	    output.element(p) = adjacent_marker;
	    for (unsigned i = 0; i < n_nbhs; ++i)
	      {
		unsigned n = p + dp[i];
		if (output.element(n) == unmarked
		    // In the border, IN_QUEUE is true so N is ignored.
		    && ! in_queue.element(n))
		  {
		    queue.push(max - input.element(n), n);
		    in_queue.element(n) = true;
		  }
	      }
	  }
      }
    return output;
  }
}


/*------------------------------------------------------------.
| Fast version (with one pixter) with a static neighborhood.  |
`------------------------------------------------------------*/

namespace fast_static
{
  using namespace mln;

  template <typename I, typename N, typename L>
  mln_ch_value(I, L)
  watershed(const Image<I>& input_, const Neighborhood<N>& nbh_,
	    L& n_basins)
  {
    // `fast_static' has the same implementation as `fast'.
    return ::fast::watershed(input_, nbh_, n_basins);
  }
}

#endif // ! APPS_BENCH_WATERSHED_HH
