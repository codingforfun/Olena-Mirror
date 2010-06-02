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

#ifndef SALEMBIER_HH
# define SALEMBIER_HH

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/extension/fill.hh>
#include <mln/extension/adjust.hh>
#include <mln/data/fill.hh>
#include <mln/util/hqueues.hh>

namespace mln
{

  template <typename T>
  struct salembier
  {
    enum { nvalues = mln_card(T) };

    template <typename N>
    salembier(const image2d<T>& f, const N& n);

  private:
    unsigned flood(unsigned h);

  public:
    const image2d<T>&		f;
    image2d<unsigned>		parent;

  private:
    image2d<bool>		deja_vu;
    unsigned			levroot[nvalues];
    bool			is_node_at_level[nvalues];
    util::hqueues<unsigned, T>	hqueues;

    // Aux
    unsigned			hmin;
    util::array<int>		dp;
    unsigned			n_nbhs;
  };

# ifndef MLN_INCLUDE_ONLY

  template <typename T>
  template <typename N>
  salembier<T>::salembier(const image2d<T>& f_, const N& nbh)
    : f(f_)
  {
    // Retrieve pmin, hmin
    unsigned	 pmin;
    T		 vmin;
    {
      util::array< unsigned > h(nvalues, 0);

      pmin = f.index_of_point(f.domain().pmin());
      vmin = f.element(pmin);

      mln_fwd_pixter(const image2d<T>) pxl(f);
      for_all(pxl)
      {
	++h[pxl.val()];
	if (pxl.val() < vmin)
	  {
	    vmin = pxl.val();
	    pmin = pxl.offset();
	  }
      }

      for (unsigned i = 0; i < nvalues; ++i)
	hqueues[i].reserve(h[i]);
    }

    // Initialize aux data
    {
      dp = offsets_wrt(f, nbh);
      n_nbhs = dp.nelements();

      extension::adjust(f, nbh);
      initialize(parent, f);
      initialize(deja_vu, f);

      data::fill(deja_vu, false);
      extension::fill(deja_vu, true);

      //std::fill(levroot, levroot + 256, 0);
      std::fill(is_node_at_level, is_node_at_level + nvalues, false);
    }

    // Start flooding
    hmin = vmin;
    hqueues[hmin].push(pmin);
    levroot[hmin] = pmin;
    is_node_at_level[hmin] = true;
    deja_vu.element(pmin) = true;
    flood(hmin);
  }


  template <typename T>
  unsigned
  salembier<T>::flood(unsigned h)
  {
    while (!hqueues[h].empty())
      {
  	unsigned p = hqueues[h].pop_front();
  	parent.element(p) = levroot[h];

  	for (unsigned j = 0; j < n_nbhs; ++j)
  	  {
  	    unsigned n = p + dp[j];
  	    // offset 0 does not belong to domain.
  	    // parent.element(n) == 0 imples:
  	    // n has not been processed and n belongs to domain.
  	    if (!deja_vu.element(n))
  	      {
  		unsigned l = f.element(n);
  		hqueues[l].push(n);
		deja_vu.element(n) = true;

  		if (!is_node_at_level[l])
		  {
		    levroot[l] = n;
		    is_node_at_level[l] = true;
		  }

		while (h < l)
		  l = flood(l);
	      }
	  }
      }

    // Attach to parent.
    unsigned x = levroot[h];
    is_node_at_level[h] = false;
    while (h > hmin && !is_node_at_level[h])
      --h;
    parent.element(x) = levroot[h];
    return h;
  }

# endif // ! MLN_INCLUDE_ONLY

}

#endif // !SALEMBIER_HH
