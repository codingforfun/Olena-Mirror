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

#ifndef OLENA_SNAKES_SEGMENT_HH
# define OLENA_SNAKES_SEGMENT_HH

#include <oln/snakes/node.hh>

#include <vector>
#include <list>

namespace oln {

  namespace snakes {

    template <class I>
    class segment
    {
    public:
      typedef typename std::vector<node<I> >::iterator iter_type;
      typedef typename std::vector<node<I> >::const_iterator const_iter_type;
      typedef typename I::point_type point_type;

    public:
      segment(std::list<point_type>& initial_contour);

    public:
      inline
      ntg::float_s
      energy(const I& gradient) const;
      ///< Just iterate through the vector and sums up point energies.

    public:
      std::list<point_type>
      contour(void) const;
      ///< Return the points of the segment.

    public:
      const_iter_type begin(void) const;
      iter_type begin(void);
      ///< Return an iterator on the first node of the segment.
      const_iter_type end(void) const;
      iter_type end(void);
      ///< Return an iterator on the last node of the segment.

    public:
      const node<I> prev_node(void) const;
      ///< Node before the first node of this segment.
      const node<I> next_node(void) const;
      ///< Node after the next node of this segment.

    private:
      std::vector<node<I> >	nodes;
      segment&			prev_segment;
      segment&			next_segment;

      const node<I> front(void) const;
      const node<I> back(void) const;

      friend
      std::ostream&
      ::operator<< <>(std::ostream&, const segment&);
    };

  } // end snakes

} // end oln


template <class I>
std::ostream& operator<<(std::ostream& os, const oln::snakes::segment<I>& s)
{
  os << "Segment:" << std::endl;
  for (typename oln::snakes::segment<I>::const_iter_type it = s.begin();
       it != s.end();
       ++it)
    os << *it << std::endl;
  return os;
}

#include <oln/snakes/segment.hxx>

#endif // !OLENA_SNAKES_SEGMENT_HH
