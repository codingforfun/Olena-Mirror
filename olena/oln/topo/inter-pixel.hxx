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

#ifndef OLENA_TOPO_BOUNDARY_INTER_PIXEL_HXX
# define OLENA_TOPO_BOUNDARY_INTER_PIXEL_HXX

using namespace oln::topo::boundary;
using namespace oln::topo::boundary::internal;

template <class T>
const typename _node<T>::cycle_t _node<T>::east = 0;

template <class T>
const typename _node<T>::cycle_t _node<T>::north = 1;

template <class T>
const typename _node<T>::cycle_t _node<T>::west = 2;

template <class T>
const typename _node<T>::cycle_t _node<T>::south = 3;


template <class _I>
const DPoint(_I) interpixel<_I>::_neighb[4] = {DPoint(_I)(0,0),
					       DPoint(_I)(-1,0),
					       dpoint_t(-1,-1),
					       dpoint_t(0,-1)};
template <class _I>
const DPoint(_I) interpixel<_I>::_inter_neighb[4] = {DPoint(_I)(0,1),
						     DPoint(_I)(-1,0),
						     DPoint(_I)(0,-1),
						     DPoint(_I)(1,0)};

template <class _I>
inline std::ostream & operator<<(std::ostream & ostr,
				 const interpixel<_I> & ip)
{
  return ip.print(ostr);
}


#endif // !OLENA_TOPO_BOUNDARY_INTER_PIXEL_HXX
