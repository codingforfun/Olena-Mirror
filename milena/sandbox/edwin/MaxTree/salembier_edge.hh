// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SALEMBIER_EDGE_HH
# define SALEMBIER_EDGE_HH

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/extend.hh>
#include <mln/core/routine/initialize.hh>

#include <mln/core/image/attribute_image.hh>
#include <mln/util/hqueues.hh>

#include "cplx2d.hh"


namespace mln
{
  typedef image2d<value::int_u8> I;


  template <typename I, typename N>
  struct salembier_args
  {
    typedef mln_psite(I) P;
    typedef mln_value(I) V;

    enum { nvalues = mln_card(V) };

    const I&					f;
    const N&					nbh;
    unsigned					hmin;
    util::hqueues< mln_psite(I), mln_value(I) >	hqueue;
    P						levroot[nvalues];
    bool					is_node_at_level[nvalues];
    mln_ch_value(I, P)				parent;
    mln_ch_value(I, bool)			deja_vu;
    mln_ch_value(I, bool)			active_edges;
    mln_ch_value(I, int)			count_edges;
  };



  template <typename I, typename N>
  int flood(salembier_args<I, N>& args, const int h)
  {
    typedef mln_psite(I) P;

    mln_assertion(args.is_node_at_level[h]);

    P root = args.levroot[h];

    while (!args.hqueue[h].empty())
      {
	P p = args.hqueue[h].pop_front();
	args.parent(p) = root;

	mln_niter(N) n(args.nbh, p);
	for_all(n)
	  if (args.f.domain().has(n) && !args.deja_vu(n))
	    {
	      int n_idx = args.f(n); // en faite vset.index_of(...)
	      args.hqueue[n_idx].push(n);
	      args.deja_vu(n) = true;

	      if (!args.is_node_at_level[n_idx])
		{
		  args.levroot[n_idx] = n;
		  args.is_node_at_level[n_idx] = true;
		}

	      while (h < n_idx)
		n_idx = flood(args, n_idx);
	    }

	mln_niter(N) e(cplx2d::p2e(), p * 2);
	for_all(e)
	  if (args.active_edges(e))
	    {
	      args.active_edges(e) = false;
	      args.count_edges(root)--; // attribute.untake(e)
	    }
	  else
	    {
	      args.active_edges(e) = true;
	      args.count_edges(root)++; // attribute.take(e)
	    }
      }

    args.is_node_at_level[h] = false;
    unsigned c = h;
    while (c > args.hmin && !args.is_node_at_level[c])
      --c;

    if (args.is_node_at_level[c])
      {
	P q = args.levroot[c];
	args.parent(root) = q;
	args.count_edges(q) += args.count_edges(root); // attribute.merge
      }
    else
      args.parent(root) = root;

    return c;
  }

  image2d<int>
  salembier(const image2d<value::int_u8>& f,
	    const neighb2d& nbh = c4())
  {
    // Init
    typedef image2d<value::int_u8> I;
    typedef neighb2d N;

    enum { nv = mln_card(mln_value_(I)) };

    salembier_args<I, N> args = { f, nbh, };
    {
      box2d b(2 * f.nrows() - 1, 2 * f.ncols() - 1);

      initialize(args.parent, f);
      initialize(args.deja_vu, f);
      initialize(args.count_edges, f);
      args.active_edges.init_(b);

      std::fill(args.is_node_at_level, args.is_node_at_level + nv, false);
      data::fill(args.deja_vu, false);
      data::fill(args.active_edges, false);
      data::fill(args.count_edges, 0);
    }

    // Get start value
    mln_psite_(I) pstart = f.domain().pmin();
    mln_value_(I) hstart = f(pstart);
    {
      unsigned h[nv] = {0};
      mln_piter_(I) p(f.domain());
      for_all(p)
      {
	mln_value_(I) v = f(p);
	++h[v];
	if (v < hstart)
	  {
	    hstart = v;
	    pstart = p;
	  }
      }

      for (unsigned i = 0; i < nv; ++i)
	args.hqueue[i].reserve(h[i]);
    }

    // Start flooding
    args.hmin = hstart;
    args.deja_vu(pstart) = true;
    args.hqueue[hstart].push(pstart);
    args.levroot[hstart] = pstart;
    args.is_node_at_level[hstart] = true;

    int r = flood(args, hstart);
    mln_assertion(r == hstart);
    return args.count_edges;
  }

}

#endif // !SALEMBIER_EDGE_HH
