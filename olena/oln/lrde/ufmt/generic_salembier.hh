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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef OLENA_LRDE_UFMT_GENERIC_SALEMBIER_HH
# define OLENA_LRDE_UFMT_GENERIC_SALEMBIER_HH


# include <climits> // For INT_MAX.
# include <cfloat> // For FLT_MAX.
# include <queue>
# include <set>
# include <map>

# include <oln/level/fill.hh>
# include <oln/lrde/ufmt/utils.hh>



namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {

      template <class V, class P>
      struct generic_hqueue_t
      {
	typedef std::map<V, std::queue<P> > multi_queue_t;

	void add(const V& h, const P& p)
	{
	  q[h].push(p);
	}

	// Add an empty queue a level H, if there is none.
	void init(const V& h)
	{
	  q.insert(std::make_pair(h, std::queue<P>()));
	}

	bool empty(const V& h) const
	{ 
	  typename multi_queue_t::const_iterator i = q.find(h);
	  if (i == q.end())
	    return true;
	  else
	    return i->second.empty();
	}

	P first(const V& h)
	{
 	  assertion (not empty(h));

	  P tmp = q[h].front();
	  q[h].pop();
	  return tmp;
	}
	
	unsigned size() const
	{
	  unsigned s = 0;
	  for (typename multi_queue_t::const_iterator i = q.begin();
	       i != q.end(); ++i)
	    s += i->second.size();
	  return s;
	}

	void print (std::ostream& os)
	{
	  os << "hqueue = {";
	  for (typename multi_queue_t::const_iterator i = q.begin();
	       i != q.end(); ++i)
	    if (not i->second.empty())
	      {
		os << "  " << i->first << " -> { ";
		std::queue<P> queue_copy = i->second;
		while (not queue_copy.empty())
		  {
		    os << queue_copy.front () << " ";
		    queue_copy.pop();
		  }
		os << "}";
	      }
	  os << " }" << std::endl;
	}

	multi_queue_t q;
      };


      template < class I,
		 class Greater = std::greater< oln_value_type(I) > >
      struct generic_salembier
      {
	typedef oln_point_type(I) point;
	typedef oln_value_type(I) value;
	typedef oln_neighborhood_type(I) Nbh;
	typedef oln_iter_type(Nbh) niter;

	enum {
	  not_analyzed = INT_MAX - 2,
	  in_the_queue = INT_MAX - 1
	};

	const I& f;
	const Nbh& nbh;

	typename mute<I, int>::ret status;

	typedef std::map<value, unsigned> number_nodes_t;
	number_nodes_t number_nodes;
 	typedef std::set<value> node_at_level_t;
 	node_at_level_t node_at_level;
	generic_hqueue_t<value, point> hqueue;
	value h_min;

	typedef std::set< value, Greater > levels_t;
	typedef typename levels_t::const_iterator level_iterator;
	// Set of all values present in the input image, sorting in
	// descending order.
	levels_t levels;

	// Level comparison function.
	Greater level_greater;

	typedef std::pair<unsigned, value> pair_t;
	std::map<pair_t, pair_t> father;


	generic_salembier(const abstract::image<I>& f,
			  const oln_neighborhood_type(I)& nbh) :
	  f(f.exact()),
	  nbh(nbh),
	  // FIXME: status should NOT be initialized here!
	  // (but in "init()") (??)
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
	      if (level_greater(h_min, f[p])) // Was: f[p] < h_min
		{
		  h_min = f[p];
		  p_min = p;
		}
	    }
	  hqueue.add(h_min, p_min);
	}


	level_iterator flood(const value& h)
	{
	  level_iterator m;
	  while (not hqueue.empty(h))
	    {
	      point p = hqueue.first(h);

	      status[p] = get(number_nodes, h, 0u);

	      oln_neighb_type(Nbh) q(nbh, p);
	      for_all(q)
		{
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
	    }
	  number_nodes[h] = get(number_nodes, h, 0u) + 1;

	  // Let *m be the greatest level lower than h.
	  m = levels.find(h);
	  if (m == levels.end())
	    abort();
	  ++m;                                  // Was: « m = h - 1 ».
	  while (m != levels.end() and          // Was: « m >= 0 ».
		 node_at_level.find(*m) == node_at_level.end())
	    ++m;                                // Was: « m = h - 1 ».
	  int i = get(number_nodes, h, 0u) - 1; // Was:
                                                // « i = number_nodes[h] - 1 ».
	  if (m != levels.end())                // Was: « m >= 0 ».
	    {
	      int j = get(number_nodes, *m, 0u);
	      father[pair_t(i, h)] = pair_t(j, *m);
	    }
	  else
	    {
	      father[pair_t(i, h)] = pair_t(i, h);
	    }
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

      }; // end of struct generic_salembier


    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_GENERIC_SALEMBIER_HH
