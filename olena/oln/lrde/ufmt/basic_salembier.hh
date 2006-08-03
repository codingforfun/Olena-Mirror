// Copyright (C) 2006  EPITA Research and Development Laboratory
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

#ifndef OLENA_LRDE_UFMT_BASIC_SALEMBIER_HH
# define OLENA_LRDE_UFMT_BASIC_SALEMBIER_HH

# include <values.h> // for MAXINT...
# include <queue>
# include <map>

# include <oln/level/fill.hh>
# include <oln/lrde/ufmt/utils.hh>



namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {

      template <class P>
      struct hqueue_t
      {
	unsigned nvalues;
	
	hqueue_t()
	{
	  nvalues = 0;
	}

	void set_nvalues(unsigned nvalues)
	{
	  assert(nvalues > 0);
	  this->nvalues = nvalues;
	  q = new std::queue<P>[nvalues];
	}

	void add(unsigned h, const P& p)
	{
	  assert(h < nvalues);
	  q[h].push(p);
	}

	bool empty(unsigned h) const
	{
	  assert(h < nvalues);
	  return q[h].empty();
	}

	P first(unsigned h)
	{
	  assert(h < nvalues);
	  P tmp = q[h].front();
	  q[h].pop();
	  return tmp;
	}

	unsigned size() const
	{
	  unsigned s = 0;
	  for (unsigned i = 0; i < nvalues; ++i)
	    s += q[i].size();
	  return s;
	}

	std::queue<P>* q;
      };


      template <class I>
      struct basic_salembier
      {
	typedef oln_point_type(I) point;
	typedef oln_value_type(I) value;
	typedef oln_neighborhood_type(I) Nbh;
	typedef oln_iter_type(Nbh) niter;

	enum {
	  not_analyzed = MAXINT - 2,
	  in_the_queue = MAXINT - 1
	};

	const I& f;
	const Nbh& nbh;

	typename mute<I, int>::ret status;

	size_t nvalues;
	unsigned* number_nodes;
	bool* node_at_level;
	hqueue_t<point> hqueue;
	int h_min;

	typedef std::pair<unsigned,unsigned> pair_t;
	std::map<pair_t, pair_t> father;


	basic_salembier(const abstract::image<I>& f,
			const oln_neighborhood_type(I)& nbh) :
	  f(f.exact()),
	  nbh(nbh),
	  // FIXME: status should NOT be initialized here! (but in "init()")
	  status(f.size())
	{
	}


	void go()
	{
	  init();
	  flood(h_min);
	}


	void init()
	{
	  nvalues = uint_nvalues(f);
	  number_nodes = new unsigned[nvalues];
	  node_at_level = new bool[nvalues];
	  for (unsigned i = 0; i < nvalues; ++i)
	    {
	      number_nodes[i] = 0;
	      node_at_level[i] = false;
	    }

	  level::fill(status, not_analyzed);

	  hqueue.set_nvalues(nvalues);
	  h_min = nvalues;
	  {
	    point p_min;
	    oln_iter_type(I) p(f);
	    for_all(p)
	      if (f[p] < h_min)
		{
		  h_min = f[p];
		  p_min = p;
		}
	    hqueue.add(h_min, p_min);
	  }
	}


	int flood(int h)
	{
	  int m;
	  while (not hqueue.empty(h))
	    {
	      point p = hqueue.first(h);
	      status[p] = number_nodes[h];

	      oln_neighb_type(Nbh) q(nbh, p);
	      for_all(q)
		if (f.hold(q))
		  {
		    if (status[q] == not_analyzed)
		      {
			hqueue.add(f[q], q);
			status[q] = in_the_queue;
			node_at_level[f[q]] = true; // err. 1
			if (f[q] > f[p])
			  {
			    m = f[q];
			    while (m > h) // err. 2
			      m = flood(m);
			  }
		      }
		  }
	    }
	  number_nodes[h] = number_nodes[h] + 1;

	  m = h - 1;
	  while (m >= 0 and node_at_level[m] == false)
	    m = m - 1;
	  int i = number_nodes[h] - 1;
	  if (m >= 0)
	    {
	      int j = number_nodes[m];
	      father[pair_t(i, h)] = pair_t(j, m );
	    }
	  else
	    {
	      father[pair_t(i, h)] = pair_t(i, h);
	    }
	  node_at_level[h] = false;
	  return m;
	}


	unsigned n_level_roots() const
	{
	  return father.size();
	}

      }; // end of struct basic_salembier



    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_BASIC_SALEMBIER_HH
