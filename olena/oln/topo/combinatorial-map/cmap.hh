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

#ifndef OLENA_TOPO_COMBINATORIAL_MAP_CMAP_HH
# define OLENA_TOPO_COMBINATORIAL_MAP_CMAP_HH

# include <oln/topo/tarjan/flat-zone.hh>
# include <oln/topo/inter-pixel/inter-pixel.hh>
# include <oln/topo/combinatorial-map/internal/zeta.hh>
# include <oln/topo/combinatorial-map/internal/cmap-functor.hh>

# include <set>
# include <list>
# include <algorithm>
# include <iterator>

namespace oln {

  namespace topo {

    namespace combinatorial_map {

      using tarjan::flat_zone;
      using inter_pixel::interpixel;

      typedef std::set<unsigned> labels_t;

      template <class I>
      class cmap
      {
      public:
	typedef DPoint(I)	dpoint_t;
	typedef Point(I)	point_t;
	typedef FwdDirIter(I)	fwd_dir_iter_t;
	typedef BkdDirIter(I)	bkd_dir_iter_t;
	typedef Head(I)		head_t;
	typedef Zeta(I)		zeta_t;

      public:
	cmap(const I & input,
	     const interpixel<I> & ip,
	     const flat_zone<I> & cc) :
	  _zeta(input.nrows() + 1, input.ncols() + 1),
	  _beta(cc.nlabels()),
	  _moth(cc.nlabels()),
	  _daugh(cc.nlabels()),
	  _lambda(_beta)
	{
	  internal::labels<unsigned>::resize(cc.nlabels());

	  unsigned ndarts = 0;

	  ndarts = _build_zeta(ip, cc);

	  internal::darts<unsigned>::resize(ndarts);
	  _alpha._resize(ndarts);
	  _sigma._resize(ndarts);
	  _lambda._resize(ndarts);

	  _build_functions(ip, cc);
	}

	//
	// neighborhood
	//
	///////////////

	// ext_neighb:
	// give a labels' set of connected component of given label which
	// are ouside of l.
	void ext_neighb(const unsigned l, labels_t & n)
	{
	  if (_beta(l) == 0)
	    return;

	  unsigned d = _beta(l);

	  do
	    {
	      n.insert(_lambda(_alpha(d)));

	      d = _sigma(_alpha(d));
	    }
	  while (d != _beta(l));
	}

	// int_neighb:
	// give a labels' set of connected component of given label which
	// are inside of l.
	void int_neighb(const unsigned l, labels_t & n)
	{
	  labels_t ext;

	  labels_t::const_iterator it;
	  for (it = _daugh(l).begin(); it != _daugh(l).end(); ++it)
	    {
	      ext.clear();
	      ext_neighb(*it, ext);

	      if (ext.find(l) != ext.end())
		n.insert(*it);
	    }
	}

	// neighb
	// give a label' set of both inside and outside connected component
	void neighb(const unsigned l, labels_t & n)
	{
	  ext_neighb(l, n);
	  int_neighb(l, n);
	}


	//
	// merging algorithm
	//
	////////////////////

	void merge(const unsigned l1, const unsigned l2)
	{
	  if (_beta(l1) == 0 || _beta(l2) == 0)
	    return;

	  labels_t s1, s2, inter;
	  unsigned d = _beta(l1);

	  do
	    {
	      s1.insert(d);
	      d = _sigma(_alpha(d));
	    }
	  while (d != _beta(l1));

	  d = _beta(l2);
	  do
	    {
	      s2.insert(_alpha(d));
	      d = _sigma(_alpha(d));
	    }
	  while (d != _beta(l2));

	  set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
			   std::inserter(inter, inter.begin()));

	  if (inter.empty())
	    if (_daugh(l1).find(l2) == _daugh(l2).end())
	      return;
	    else
	      {
		labels_t::const_iterator it;
		for (it = s2.begin(); it != s2.end(); ++it)
		  if (_lambda(*it) == l1)
		    {
		      internal::darts<unsigned>::erase(_alpha(*it));
		      internal::darts<unsigned>::erase(*it);
		    }
	      }
	  else
	    {
	      labels_t::const_iterator it;
	      for (it = inter.begin(); it != inter.end(); ++it)
		{
		  internal::darts<unsigned>::erase(_alpha(*it));
		  internal::darts<unsigned>::erase(*it);
		}
	    }

	  internal::labels<unsigned>::substitute(l2, l1);

	  // FIXME: don't know how to fix this -- burrus_n
	  // cc.merge(l1, l2);
	  assert(0);
	}

	//
	// print
	//
	////////
	std::ostream& print(std::ostream & ostr) const
	{
	  ostr << _alpha << std::endl;
	  ostr << _sigma << std::endl;
	  ostr << _lambda << std::endl;
	  ostr << _moth << std::endl;
	  ostr << _daugh << std::endl;
	  ostr << _beta << std::endl;

	  return ostr;
	}

      private:

	//
	// building combinatorial map
	////////////////////////

	unsigned _build_zeta(const interpixel<I> & ip,
			     const flat_zone<I> & cc)
	{
	  unsigned ndarts = 0;
	  unsigned moth = 0;

	  std::list<point_t> acc;
	  fwd_dir_iter_t i;

	  for (unsigned l = 2; l <= cc.nlabels(); ++l)
	    if (_moth(l) == 0)
	      {
		acc.clear();

		point_t n1 = cc.get_root(l);

		moth = cc.get_label(n1 + dpoint_t(0, -1));

		head_t h1 = ip.folw(head_t(n1, DirTraits(I)::first()));

		acc.push_back(h1.first);

		while (!acc.empty())
		  {
		    n1 = acc.front();

		    for_all(i)
		      if (ip[n1].get(i) && !_zeta[n1][i])
			{
			  unsigned l = cc.get_label(n1 + ip._neighb[i]);

			  if (moth != l)
			    {
			      _moth(l) = moth;
			      _daugh(moth).insert(l);
			    }

			  _zeta[n1][i] = ++ndarts;
			  head_t h2 = ip.folw(head_t(n1, i));

			  if (_zeta[h2.first].empty())
			    acc.push_back(h2.first);

			  _zeta[h2.first][h2.second] = ++ndarts;
			}

		    acc.pop_front();
		  }
	      }

	  return ndarts;
	}

	void _build_functions(const interpixel<I> & ip,
			      const flat_zone<I> & cc)
	{
	  Iter(zeta_t) it(_zeta);
	  fwd_dir_iter_t i;

	  for_all(it)
	    {
	      if (!_zeta[it].empty())
		for_all(i)
		{
		  unsigned d = _zeta[it][i];

		  if (d)
		    {
		      fwd_dir_iter_t j(i.next());
		      for_all(j)
			{
			  unsigned s = _zeta[it][j];

			  if (s)
			    {
			      unsigned l = cc.get_label(it + ip._neighb[i]);

			      _alpha(d) = d & 1 ? d + 1 : d - 1;
			      _sigma(d) = s;

			      _beta(l) = d;
			      _lambda(d) = l;

			      break;
			    }
			}
		    }
		}
	    }
	}

      private:
	zeta_t _zeta;

	internal::beta<unsigned> _beta;
	internal::moth<unsigned>  _moth;
	internal::daugh<unsigned, labels_t> _daugh;

	internal::alpha<unsigned>  _alpha;
	internal::sigma<unsigned>  _sigma;
	internal::lambda<unsigned> _lambda;
      };

    } // end combinatorial_map

  } // end topo

} // end oln

template<class I>
inline std::ostream &
operator<<(std::ostream & ostr,
	   const oln::topo::combinatorial_map::cmap<I> & cm);

inline std::ostream &
operator<<(std::ostream & ostr,
	   const oln::topo::combinatorial_map::labels_t & l);

# include <oln/topo/combinatorial-map/cmap.hxx>

#endif // !OLENA_TOPO_COMBINATORIAL_MAP_CMAP_HH
