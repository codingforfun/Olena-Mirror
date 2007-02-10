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

#ifndef OLENA_SNAKES_NODE_HH
# define OLENA_SNAKES_NODE_HH

#include <oln/basics2d.hh> // FIXME: Why only 2d?

namespace oln {

  namespace snakes {

    /*! A node is a point used in ring.
    **
    ** \todo FIXME: Do not work due to the function energy.
    */
    template<class I>
    class node : public I::point_type
    {
    public:
      typedef typename I::point_type	point_type;
      typedef typename I::dpoint_type	dpoint_type;

    public:
      node(point_type point) :
	I::point_type(point)
      {
      }

    public:
      /*! Return the energy
      **
      ** \todo FIXME: not implemented, do not work
      */
      inline
      ntg::float_s
      energy(const I& gradient, point_type prev, point_type next) const;

    private:
      friend std::ostream&
      ::operator<< <>(std::ostream&, const node&);
    };

  } // end snakes

} // end oln

//! Print the position of a node \a n
template <class I>
std::ostream& operator<<(std::ostream& os, const oln::snakes::node<I>& n)
{
  os << "Node:" << static_cast<typename oln::snakes::node<I>::point_type>(n);
  return os;
}

#include <oln/snakes/node.hxx>

#endif // !OLENA_SNAKES_NODE_HH
