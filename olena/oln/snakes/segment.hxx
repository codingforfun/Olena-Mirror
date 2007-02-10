// Copyright (C) 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_SNAKES_SEGMENT_HXX
# define OLENA_SNAKES_SEGMENT_HXX

namespace oln {

  namespace snakes {

    template <class I>
    segment<I>::segment(std::list<point_type>& initial_contour) :
      prev_segment(*this), next_segment(*this)
    {
      for (typename std::list<point_type>::const_iterator
	     it = initial_contour.begin();
	   it != initial_contour.end();
	   ++it)
	nodes.push_back(*it);
    }

    template <class I>
    inline
    ntg::float_s
    segment<I>::energy(const I& gradient) const
    {
      ntg::float_s e = 0.f;
      typename segment<I>::const_iter_type p = nodes.begin();
      typename segment<I>::const_iter_type c = nodes.begin();
      typename segment<I>::const_iter_type n = nodes.begin();

      ++n;
      e += c->energy(gradient, prev_node(), *n);
      for (++c, ++n; n != nodes.end(); ++p, ++c, ++n)
	e += c->energy(gradient, *p, *n);
      e += c->energy(gradient, *p, next_node());
      return e;
    }

    template <class I>
    std::list<typename segment<I>::point_type>
    segment<I>::contour(void) const
    {
      std::list<point_type> result;

      for (typename segment<I>::const_iter_type it = nodes.begin();
	   it != nodes.end(); ++it)
	result.push_back(*it);
      return result;
    }

    template <class I>
    typename segment<I>::const_iter_type
    segment<I>::begin(void) const
    {
      return nodes.begin();
    }
    template <class I>
    typename segment<I>::iter_type
    segment<I>::begin(void)
    {
      return nodes.begin();
    }

    template <class I>
    typename segment<I>::const_iter_type
    segment<I>::end(void) const
    {
      return nodes.end();
    }
    template <class I>
    typename segment<I>::iter_type
    segment<I>::end(void)
    {
      return nodes.end();
    }

    template <class I>
    const node<I>
    segment<I>::prev_node(void) const
    {
      return prev_segment.back();
    }
    template <class I>
    const node<I>
    segment<I>::next_node(void) const
    {
      return next_segment.front();
    }

    template <class I>
    const node<I>
    segment<I>::front(void) const
    {
      return nodes.front();
    }
    template <class I>
    const node<I>
    segment<I>::back(void) const
    {
      return nodes.back();
    }

  } // end snakes

} // end oln

#endif // !OLENA_SNAKES_SEGMENT_HXX
