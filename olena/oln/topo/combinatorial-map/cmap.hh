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

# include <algorithm>
# include <iterator>

# include <oln/topo/tarjan/flat-zone.hh>
# include <oln/topo/inter-pixel/inter-pixel.hh>
# include <oln/topo/combinatorial-map/internal/zeta.hh>
# include <oln/topo/combinatorial-map/internal/allfunc.hh>
# include <oln/topo/combinatorial-map/internal/level.hh>

namespace oln {

  namespace topo {

    namespace combinatorial_map {

      using tarjan::flat_zone;
      using inter_pixel::interpixel;

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
	cmap(const I & input, const interpixel<I> & ip, flat_zone<I> & cc) :
	  _ndarts(0),
	  _zeta(input.nrows() + 1, input.ncols() + 1),
	  _cc(cc),
	  _level(cc.nlabels()),
	  _beta(cc.nlabels())
	{
	  _build_zeta(ip);

	  _sigma.resize(_ndarts);
	  _lambda.resize(_ndarts);

	  _build_functions(ip);
	}

	//
	// neighborhood
	//
	///////////////

	// ext_neighb:
	// give a labels' set of connected component of given label which
	// are ouside of l.
// 	void ext_neighb(const unsigned l, labels_t & n)
// 	{
// 	  if (_beta(l) == 0)
// 	    return;

// 	  unsigned d = _beta(l);

// 	  do
// 	    {
// 	      n.insert(_lambda(_alpha(d)));

// 	      d = _sigma(_alpha(d));
// 	    }
// 	  while (d != _beta(l));
// 	}

	// int_neighb:
	// give a labels' set of connected component of given label which
	// are inside of l.
// 	void int_neighb(const unsigned l, labels_t & n)
// 	{
// 	  labels_t ext;

// 	  labels_t::const_iterator it;
// 	  for (it = _daugh(l).begin(); it != _daugh(l).end(); ++it)
// 	    {
// 	      ext.clear();
// 	      ext_neighb(*it, ext);

// 	      if (ext.find(l) != ext.end())
// 		n.insert(*it);
// 	    }
// 	}

	// neighb
	// give a label' set of both inside and outside connected component
// 	void neighb(const unsigned l, labels_t & n)
// 	{
// 	  ext_neighb(l, n);
// 	  int_neighb(l, n);
// 	}


	//
	// merging algorithm
	//
	////////////////////

	void merge(const unsigned l1, const unsigned l2)
	{
	  static std::vector<unsigned> inter;

	  // FIXME: Do not check whether a label is valide on beta function
	  assertion(_beta(l1) && _beta(l2));

	  if (_level.father(l2) != l1)
	    {
	      // l1 and l2 are connected
	      assertion(_make_inter1(l1, l2, inter));
	    }
	  else
	    {
	      // l2 is included in l1
	      assertion(_make_inter2(l1, l2, inter));
	    }

	  _update_darts(l1, l2, inter);

	  _level.merge(l1, l2);

	  _cc.merge(l1, l2);

	  inter.clear();
	}

	//
	// print
	//
	////////

	std::ostream& print(std::ostream & ostr) const
	{
	  ostr << _sigma << std::endl;
	  ostr << _lambda << std::endl;
	  ostr << _beta << std::endl;

 	  ostr << _level << std::endl;

	  return ostr;
	}

      private:

	//
	// building combinatorial map
	//
	/////////////////////////////

	void _build_zeta(const interpixel<I> & ip)
	{
	  unsigned father = 0;

	  std::list<point_t> acc;
	  fwd_dir_iter_t i;

	  for (unsigned l = 2; l <= _cc.nlabels(); ++l)
	    if (_level.father(l) == 0)
	      {
		acc.clear();

		point_t n1 = _cc.get_root(l);

		father = _cc.get_label(n1 + dpoint_t(0, -1));

		head_t h1 = ip.folw(head_t(n1, DirTraits(I)::first()));

		acc.push_back(h1.first);

		while (!acc.empty())
		  {
		    n1 = acc.front();

		    for_all(i)
		      if (ip[n1].get(i) && !_zeta[n1][i])
			{
			  unsigned l = _cc.get_label(n1 + ip._neighb[i]);

			  if (father != l && !_level.father(l))
			    _level.insert(father, l);

			  _zeta[n1][i] = ++_ndarts;
			  head_t h2 = ip.folw(head_t(n1, i));

			  if (_zeta[h2.first].empty())
			    acc.push_back(h2.first);

			  _zeta[h2.first][h2.second] = ++_ndarts;
			}

		    acc.pop_front();
		  }
	      }
	}

	void _build_functions(const interpixel<I> & ip)
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
			      unsigned l = _cc.get_label(it + ip._neighb[i]);

			      _sigma.assign(d, s);
			      _lambda.assign(d, l);
			      _beta.assign(l, d);

			      break;
			    }
			}
		    }
		}
	    }
	}

	//
	// internal method for merge
	//
	////////////////////////////

	bool _make_inter1(const unsigned l1,
			  const unsigned l2,
			  std::vector<unsigned> & inter)
	{
	  static std::vector<bool> buff(_ndarts);

 	  unsigned d = _beta(l1);

 	  do
 	    {
 	      buff[d] = true;
 	      d = _sigma(internal::alpha<unsigned>::result(d));
 	    }
 	  while (d != _beta(l1));

 	  d = _beta(l2);

	  unsigned d_ = 0;

 	  do
 	    {
	      d_ = internal::alpha<unsigned>::result(d);

 	      if (buff[d_])
		inter.push_back(d_);
	      d = _sigma(d_);
 	    }
 	  while (d != _beta(l2));

	  buff.clear();

	  return !inter.empty();
	}

	bool _make_inter2(const unsigned l1,
			  const unsigned l2,
			  std::vector<unsigned> & inter)
	{
	  unsigned d = _beta(l2);
	  unsigned d_;

	  do
	    {
	      d_ = internal::alpha<unsigned>::result(d);

	      if (_lambda(d_) == l1)
		inter.push_back(d_);

 	      d = _sigma(d_);
	    }
	  while (d != _beta(l2));

	  return !inter.empty();
	}

	void _update_darts(const unsigned l1,
			   const unsigned l2,
			   std::vector<unsigned> & inter)
	{
	  std::vector<unsigned>::const_iterator i;
	  for (i = inter.begin(); i != inter.end(); ++i)
	    {
	      unsigned d = *i;

	      _sigma.erase(d);
	      _lambda.erase(d);
	    }

	  // FIXME: find a way to get this in constant time
	  for (unsigned j = 1; j < _ndarts; ++j)
	    if (_lambda(j) == l2)
	      _lambda._assign(j, l1);

	  _beta.erase(l2);
	}

      private:
	unsigned			_ndarts;
	zeta_t				_zeta;
	flat_zone<I> &			_cc;
	internal::level<unsigned>	_level;

	internal::beta<unsigned>	_beta;
	internal::sigma<unsigned>	_sigma;
	internal::lambda<unsigned>	_lambda;
      };

    } // end combinatorial_map

  } // end topo

} // end oln

template<class I>
inline std::ostream &
operator<<(std::ostream & ostr,
	   const oln::topo::combinatorial_map::cmap<I> & cm);

# include <oln/topo/combinatorial-map/cmap.hxx>

#endif // !OLENA_TOPO_COMBINATORIAL_MAP_CMAP_HH
