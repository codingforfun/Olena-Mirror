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

#ifndef OLENA_TOPO_BOUNDARY_INTER_PIXEL_HH
# define OLENA_TOPO_BOUNDARY_INTER_PIXEL_HH

# include <ntg/cycle.hh>

# include <map>
# include <set>

using namespace ntg;

namespace oln {

  namespace topo {

    namespace boundary {

      namespace internal {
	template <class T>
	class _node;
      }

      //
      //  Class interpixel<_I>
      //
      //////////////////////////////////

      template <class _I>
      class interpixel
      {
      public:
	typedef DPoint(_I) dpoint_t;
	typedef Point(_I) point_t;
	typedef Value(_I) T;

	typedef typename mute< _I, internal::_node<T> >::ret data_t;
	typedef typename mute< _I, bool >::ret mark_t;

	typedef typename internal::_node<T>::cycle_t cycle_t;

      public:
	interpixel(const _I & img) :
	  _data(img.nrows() + 1, img.ncols() + 1),
	  _mark(img.nrows() + 1, img.ncols() + 1)
	{
	  // FIXME: assume there is no boundary pixel with a max value.
	  // pearhaps we should clone img with a higher type.
	  border::adapt_assign(img, 1, optraits<T>::max());

	  Iter(data_t) p(_data);

	  for_all(p)
	    {
	      cycle_t i = 0;
	      do
		if (img[p + _neighb[i]] != img[p + _neighb[i + 1]])
		  _data[p].set(i);
	      while (++i);
	    }
	}

	const internal::_node<T> & operator[](const point_t & p) const
	{
	  return _data[p];
	}
	internal::_node<T> & operator[](const point_t & p)
	{
	  return _data[p];
	}

	const image2d_size size() const { return _data.size(); }

	const point_t folw(const point_t & p, cycle_t & i)
	{
	  precondition(_data[p].get(i) == true);

	  point_t cur = p;

	  do
	    {
	      _mark[cur] = true;
	      cur += _inter_neighb[i];

	      if (cur == p || _data[cur].rank() > 2)
		{
		  i += 2;
		  _mark[cur] = true;
		  break;
		}

	      if(_data[cur].get(i + 1))
		++i;
	      else if (_data[cur].get(i - 1))
		--i;
	    }
	  while (cur != p);

	  return cur;
	}

	void get_int_border(const point_t & p,
			    std::vector<point_t> & int_border) const
	{
	  precondition(_data[p].get(internal::_node<T>::south) == true);

	  cycle_t i = internal::_node<T>::south;
	  cycle_t i_last;
	  point_t cur = p;

	  do
	    {
	      cur += _inter_neighb[i];
	      i_last = i;

	      if (_data[cur].get(i + 1))
		++i;
	      else if (!_data[cur].get(i))
		--i;

	      point_t pt = cur + _neighb[i + 1];

	      if (i == internal::_node<T>::south)
		{
		  if (!_mark[pt + dpoint_t(0, 1)] && i != i_last)
		    int_border.push_back(pt + dpoint_t(0, 1));

		  if (!_mark[pt + dpoint_t(1, 1)])
		    int_border.push_back(pt + dpoint_t(1, 1));
		}
	      else
		if (i == internal::_node<T>::north)
		  {
		    if (!_mark[pt + dpoint_t(1, 0)] && i != i_last)
		      int_border.push_back(pt + dpoint_t(1, 0));

		    if (!_mark[pt + dpoint_t(0, 0)])
		      int_border.push_back(pt + dpoint_t(0, 0));
		  }
	    }
	  while (cur != p);
	}

	bool find_inside_node(point_t & p, const unsigned l) const
	{
	  typedef std::map< int, std::multiset<int> > map_t;
	  typedef std::vector<point_t> vec_t;

	  static unsigned last = 0;
	  static std::vector<point_t> out;

	  precondition(_data[p].rank() >= 2);

	  if (last != l)
	    {
	      last = l;

	      out.clear();

	      vec_t int_border;
	      get_int_border(p, int_border);

	      map_t run;

	      typename vec_t::const_iterator it;
	      for (it = int_border.begin(); it != int_border.end(); ++it)
		run[it->row()].insert(it->col());

	      for(map_t::iterator i = run.begin();
		  i != run.end(); ++i)
		{
		  assertion((i->second).size() % 2 == 0);

		  std::set<int>::iterator j;
		  for(j = i->second.begin(); j != i->second.end(); ++j)
		    {
		      typename std::set<int>::iterator beginning = j++;
		      for (int k = *beginning; k != *(j)+1; ++k)
			out.push_back(point_t(i->first,k));
		    }
		}
	    }

	  typename vec_t::const_iterator iv;
	  for (iv = out.begin(); iv != out.end(); ++iv)
	    if (_data[*iv].rank() >= 2 && !_mark[*iv])
	      {
		p = *iv;
		return true;
	      }

	  return false;
	}

	std::ostream & print(std::ostream & ostr) const
	{
	  Iter(data_t) p(_data);

	  for_all(p)
	    if (_data[p].rank() > 2)
	      {
		ostr << p << ":";

		cycle_t i = 0;
		do
		  ostr << " " << _data[p].get(i);
		while (++i);

		ostr << std::endl;
	      }

	  return ostr;
	}

      private:
	data_t	_data;
	mark_t	_mark;

      public:
	static const dpoint_t _neighb[4];
	static const dpoint_t _inter_neighb[4];
      };

      namespace internal {

	template <class T>
	class _node
	{
	public:
	  typedef cycle<T, bounded_u<0, 4> > cycle_t;

	public:
	  _node() : _rank(0)
	  {
	    cycle_t i = 0;
	    do
	      _dirs[i] = false;
	    while (++i);
	  }

	  _node & operator=(const _node & rhs)
	  {
	    _rank = rhs.rank();

	    cycle_t i = 0;
	    do
	      _dirs[i] = rhs.get(i);
	    while (++i);

	    return *this;
	  }

	  bool get(const cycle_t i) const
	  {
	    assertion(i < 4);

	    return _dirs[i];
	  }

	  void set(const cycle_t i)
	  {
	    assertion(i < 4);

	    if (_dirs[i] == false)
	      ++_rank;

	    _dirs[i] = true;
	  }

	  unsigned rank() const { return _rank; }

	private:
	  bool _dirs[4];
	  unsigned _rank;

	public:
	  static const cycle_t east;
	  static const cycle_t north;
	  static const cycle_t west;
	  static const cycle_t south;
	};

      } // end internal

    } // end boundary

  } // end topo

} // end oln

# include <oln/topo/inter-pixel.hxx>

#endif // !OLENA_TOPO_BOUNDARY_INTER_PIXEL_HH
