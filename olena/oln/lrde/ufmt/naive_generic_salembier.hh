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


// FIXME: This version of Salembier et al. algorithm has been
// superseded by oln::lrde::ufmt::generic_simplified_salembier, and
// should not be useful anymore.


#ifndef OLENA_LRDE_UFMT_NAIVE_GENERIC_SALEMBIER_HH
# define OLENA_LRDE_UFMT_NAIVE_GENERIC_SALEMBIER_HH

# include <iostream>

# include <values.h> // for MAXINT...
# include <queue>
# include <map>

# include <set>

# include <oln/level/fill.hh>
# include <oln/lrde/ufmt/utils.hh>



// Forward declaration.
namespace ntg
{
  template <unsigned nbits, class behavior>
  class int_u;
}

namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {

      /*------------------------------------------------------------.
      | A generic version of Salembier's algorithm (in particular,  |
      | working on float values), but naive w.r.t. equal levels (it |
      | assumes that each level is unique in the input image).      |
      `------------------------------------------------------------*/

      /* FIXME: This version of generic_hqueue is naive, since it
	 expects that each value in an image is unique.  Provide
	 another version where a value can appear more than once.  */

      /// A more generic version of hqueue.
      template <class V, class P>
      struct naive_generic_hqueue_t
      {
	void add(V h, const P& p)
	{
	  if (q.find (h) != q.end())
	    std::cerr
	      << "oln::lrde::ufmt::naive_generic_hqueue_t: Warning: "
	      << "two points at level " << h << " inserted in hqueue"
	      << std::endl;
	  q.insert(std::make_pair(h, p));
	}

	bool empty(V h) const
	{
	  return q.count(h) == 0;
	}

	P first(V h)
	{
	  P tmp = q[h];
	  q.erase (h);
	  return tmp;
	}

	unsigned size() const
	{
	  return q.size;
	}

	std::map<V, P> q;
      };


      /** \brief A more generic version of Salembier's algorithm.

          Like basic_salembier, but working on a larger set of image
          types (basic_salembier is faster, but works only on images of
          unsigned integers).  */
      template <class I>
      struct naive_generic_salembier
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
	std::map<value, unsigned> number_nodes;
	std::set<value> node_at_level;
	naive_generic_hqueue_t<value, point> hqueue;
	value h_min;

	typedef std::set< value, std::greater<value> > levels_t;
	typedef typename levels_t::const_iterator level_iterator;
	// Set of all values present in the input image, sorting in
	// descending order.
	levels_t levels;

	typedef std::pair<unsigned, value> pair_t;
	std::map<pair_t, pair_t> father;


	naive_generic_salembier(const abstract::image<I>& f,
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
	  level::fill(status, not_analyzed);

	  oln_iter_type(I) p(f);
	  p = mlc::begin;
	  point p_min = p;
	  h_min = f[p];
	  for_all(p)
	    {
	      // Init levels.
	      levels.insert(f[p]);
	      // Find the global minimum.
	      if (f[p] < h_min)
		{
		  h_min = f[p];
		  p_min = p;
		}
	  }
	  hqueue.add(h_min, p_min);
	}

	level_iterator flood(value h)
	{
	  level_iterator m;
	  while (not hqueue.empty(h))
	    {
	      point p = hqueue.first(h);
	      status[p] = get(number_nodes, h, 0u);

	      oln_neighb_type(Nbh) q(nbh, p);
	      for_all(q)
		if (f.hold(q))
		  {
		    if (status[q] == not_analyzed)
		      {
			hqueue.add(f[q], q);
			status[q] = in_the_queue;
			node_at_level.insert(f[q]); // err. 1
			if (f[q] > f[p])
			  {
			    m = levels.find(f[q]);
			    if (m == levels.end())
			      abort();
			    while (m != levels.end() and *m > h) // err. 2
			      m = flood(*m);
			  }
		      }
		  }
	    }
	  number_nodes[h] = get(number_nodes, h, 0u) + 1;

	  // Let *m be the greatest level lower than h.
	  m = levels.find(h);
	  if (m == levels.end())
	    abort();
	  ++m;
	  while (m != levels.end() and
		 node_at_level.find(*m) == node_at_level.end())
	    ++m;
	  int i = get(number_nodes, h, 0u) - 1;
	  if (m != levels.end())
	    {
	      int j = get(number_nodes, *m, 0u);
	      father[pair_t(i, h)] = pair_t(j, *m);
	    }
	  else
	    father[pair_t(i, h)] = pair_t(i, h);
	  node_at_level.erase(h);
	  return m;
	}

	unsigned n_level_roots() const
	{
	  return father.size();
	}

	/// An alternative to std::map::operator[] handling non
	/// existent keys (using \a default_val).
	template <typename Key, typename Data,
		  typename Compare, typename Alloc>
	Data get(const std::map<Key, Data, Compare, Alloc>& map,
		 const Key& key, const Data& default_val) const
	{
	  typename std::map<Key, Data, Compare, Alloc>::const_iterator n =
	    map.find(key);
	  if (n != map.end())
	    return n->second;
	  else
	    return default_val;
	}

      }; // end of struct naive_generic_salembier



    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_NAIVE_GENERIC_SALEMBIER_HH
