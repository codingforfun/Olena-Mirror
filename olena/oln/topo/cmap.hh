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

#ifndef OLENA_TOPO_CMAP_CMAP_HH
# define OLENA_TOPO_CMAP_CMAP_HH

# include <oln/topo/inter-pixel.hh>
# include <oln/topo/flat-zone.hh>
# include <oln/topo/cmap-functor.hh>

# include <set>
# include <algorithm>
# include <functional>
# include <iterator>

namespace oln {

  namespace topo {

    namespace cmap {

      typedef std::set<unsigned> labels_t;

      namespace internal {
	template <class T>
	class _zeta;
      }

      //
      //  Class cmap<_I>
      //
      //////////////////////////////////

      template <class _I>
      class cmap
      {
      public:
	typedef DPoint(_I) dpoint_t;
	typedef Point(_I) point_t;
	typedef Value(_I) T;

	typedef typename mute< _I, internal::_zeta<T> >::ret zeta_t;

      private:
	typedef typename interpixel<_I>::cycle_t cycle_t;

      public:
	cmap(const _I & input)
	  : _fz(input),
	    _ip(input),
	    _nlabels(_fz.nlabels()),
	    _ndarts(0),
	    _lambda(_beta)
	{
	  internal::labels<unsigned>::resize(_nlabels);
	  _beta._resize(_nlabels);
	  _moth._resize(_nlabels);
	  _daugh._resize(_nlabels);

	  std::vector<unsigned> known_labels;
	  std::vector<bool> mark(_nlabels + 1, false);

	  known_labels.push_back(1);
	  mark[1] = true;

	  _build_level(known_labels, mark);

	  std::vector<unsigned>::iterator it = known_labels.begin();
	  for (++it; it != known_labels.end(); ++it)
	    {
	      point_t p = _fz.get_root(*it);

	      while (_ip.find_inside_node(p, *it))
		_build_level(known_labels, mark, *it, p);
	    }
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

	  _fz.merge(l1, l2);
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

	tarjan::flat_zone<_I> _fz;
	boundary::interpixel<_I> _ip;

	unsigned _nlabels;
	unsigned _ndarts;

	internal::alpha<unsigned>  _alpha;
	internal::sigma<unsigned>  _sigma;
	internal::lambda<unsigned> _lambda;

	internal::moth<unsigned>  _moth;
	internal::daugh<unsigned, labels_t> _daugh;
	internal::beta<unsigned> _beta;

      private:

	//
	// building combinatorial map
	////////////////////////

	void _build_level(std::vector<unsigned> & known_labels,
			  std::vector<bool> & mark,
			  const unsigned moth = 1,
			  const point_t p = point_t(0,0))
	{
	  static zeta_t zeta(_ip.size());

	  std::list<point_t> acc;
	  typename std::list<point_t>::iterator it;

	  cycle_t dir = 0;
	  do
	    if (_ip[p].get(dir))
	      break;
	  while (++dir);

	  point_t n1 = _ip.folw(p, dir);

	  zeta[n1] = internal::_zeta<T>(dir);
	  acc.push_back(n1);

	  for (it = acc.begin(); it != acc.end(); ++it)
	    {
	      n1 = *it;

	      dir = zeta[n1].cur();

	      do
		{
		  if (_ip[n1].get(dir) && !zeta[n1][dir])
		    {
		      zeta[n1][dir] = ++_ndarts;
		      point_t n2 = _ip.folw(n1, dir);

		      if (zeta[n2] == internal::_zeta<T>::null())
			{
			  zeta[n2] = internal::_zeta<T>(dir);
			  acc.push_back(n2);
			}

		      zeta[n2][dir] = ++_ndarts;
		    }
		  dir = zeta[n1].cur_inc();
		}
	      while (zeta[n1].cur() != zeta[n1].init());
	    }

	  internal::darts<unsigned>::resize(_ndarts);
	  _alpha._resize(_ndarts);
	  _sigma._resize(_ndarts);
	  _lambda._resize(_ndarts);

	  for (it = acc.begin(); it != acc.end(); ++it)
	    {
	      cycle_t i = 0;
	      do
		{
		  unsigned d = zeta[*it][i];

		  if (d)
		    {
		      for (cycle_t j = i + 1; j != i; ++j)
			{
			  unsigned s = zeta[*it][j];

			  if (s)
			    {
			      unsigned l = _fz.get_label(*it + _ip._neighb[i]);

			      _alpha(d) = d & 1 ? d + 1 : d - 1;
			      _sigma(d) = s;

			      if (!_beta(l))
				_beta(l) = d;
			      _lambda(d) = l;

			      if (l != moth)
				{
				  _moth(l) = moth;
				  _daugh(moth).insert(l);

				  if (mark[l] == false)
				    {
				      mark[l] = true;
				      known_labels.push_back(l);
				    }
				}

			      break;
			    }
			}
		    }
		}
	      while (++i);
	    }
	}
      };

      namespace internal {

	template <class T>
	class _zeta
	{
	public:
	  typedef cycle<T, bounded_u<0, 4> > cycle_t;

	public:
	  _zeta()
	    : _init(0), _cur(0)
	  {
	    for (unsigned i = 0; i < 4; ++i)
	      _darts[i] = 0;
	  }

	  // FIXME: unsigned -> cycle_t conversion bug
	  _zeta(const cycle_t & init)
	    : _init(init), _cur(init)
	  {
	    for (unsigned i = 0; i < 4; ++i)
	      _darts[i] = 0;
	  }

	  bool operator==(const _zeta & rhs) const
	  {
	    cycle_t i;
	    do
	      if (_darts[i] != rhs[i])
		return false;
	    while(++i);

	  // FIXME: unsigned -> cycle_t conversion bug
// 	    for (unsigned i = 0; i < 4; ++i)
// 	      if (_darts[i] != rhs[i])
// 		return false;

	    return true;
	  }

	  bool operator!=(const _zeta & rhs) const
	  {
	    return !(*this == rhs);
	  }

	  unsigned operator[](const cycle_t & i) const { return _darts[i]; }
	  unsigned & operator[](const cycle_t & i) { return _darts[i]; }

	  const cycle_t & init() const { return _init; }
	  const cycle_t & cur() const { return _cur; }
	  const cycle_t & cur_inc() { return ++_cur; }

	private:
	  cycle_t _init, _cur;

	  unsigned _darts[4];

	public:
	  static const _zeta & null()
	  {
	    static _zeta _result;

	    return _result;
	  }
	};

      } // end internal

    } // end cmap

  } // end topo

} // end oln

template<class _I>
inline std::ostream &
operator<<(std::ostream & ostr,
	   const oln::topo::cmap::cmap<_I> & cm);

inline std::ostream &
operator<<(std::ostream & ostr,
	   const oln::topo::cmap::labels_t & l);

# include <oln/topo/cmap.hxx>

#endif // !OLENA_TOPO_CMPA_CMAP_HH
