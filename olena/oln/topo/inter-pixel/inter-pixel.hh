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

#ifndef OLENA_TOPO_INTER_PIXEL_INTER_PIXEL_HH
# define OLENA_TOPO_INTER_PIXEL_INTER_PIXEL_HH

# include <oln/basics.hh>
# include <oln/topo/inter-pixel/node.hh>
# include <oln/topo/inter-pixel/fwd-dir-iter.hh>
# include <oln/topo/inter-pixel/bkd-dir-iter.hh>

# include <ntg/optraits.hh>

namespace oln {

  namespace topo {

    namespace inter_pixel {

# define InterPixel(ImgType)			\
typename mute< ImgType, Node(ImgType) >::ret

      template <class _I>
      class interpixel
      {
      public:
	typedef DPoint(_I)	dpoint_t;
	typedef Point(_I)	point_t;
	typedef Node(_I)	node_t;
	typedef FwdDirIter(_I)	fwd_dir_iter_t;
	typedef BkdDirIter(_I)	bck_dir_iter_t;
	typedef Dir(_I)		dir_t;
	typedef DirTraits(_I)	dir_traits_t;
	typedef Head(_I)	head_t;
	typedef InterPixel(_I)	inter_pixel_t;

      public:
	interpixel(const _I & img) :
	  _data(img.nrows() + 1, img.ncols() + 1)
	{
	  // FIXME: assume there is no boundary pixel with a max value.
	  // pearhaps we should clone img with a higher type.
	  border::adapt_assign(img, 1, ntg::optraits< Value(_I) >::max());

	  Iter(inter_pixel_t) p(_data);
	  fwd_dir_iter_t it;

	  for_all(p)
	    {
	      for_all(it)
		if (img[p + _neighb[it]] != img[p + _neighb[it.next()]])
		  _data[p].set(it);
	    }
	}

	// FIXME: Why can't we use returning reference ?
	const node_t operator[](const point_t & p) const
	{
	  return _data[p];
	}

	head_t folw(const head_t & in) const
	{
	  precondition(_data[in.first].get(in.second) == true);

	  head_t out = in;

	  do
	    {
	      out.first += _inter_neighb[out.second];

	      if (out.first == in.first || _data[out.first].rank() > 2)
		{
		  out.second = dir_traits_t::opposite(out.second);
		  break;
		}

	      dir_t next = dir_traits_t::next(out.second);
	      dir_t prev = dir_traits_t::prev(out.second);

	      out.second = _data[out.first].get(next) ? next :
		_data[out.first].get(prev) ? prev : out.second;
	    }
	  while (out.first != in.first);

	  return out;
	}

	std::ostream & print(std::ostream & ostr) const
	{
	  Iter(inter_pixel_t) p(_data);

	  for_all(p)
	    if (_data[p].rank() > 2)
	      {
		ostr << p << ":";

		fwd_dir_iter_t it;
		for_all(it)
		  if (_data[p].get(it))
		    ostr << " " << it;

		ostr << std::endl;
	      }

	  return ostr;
	}

      private:
	inter_pixel_t	_data;

      public:
	static const dpoint_t _neighb[4];
	static const dpoint_t _inter_neighb[4];
      };


    } // end inter_pixel

  } // end topo

} // end oln

# include <oln/topo/inter-pixel/inter-pixel.hxx>

#endif // !OLENA_TOPO_INTER_PIXEL_INTER_PIXEL_HH
