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

#ifndef UNION_HH
# define UNION_HH

#include <oln/basics2d.hh>
#include <ntg/all.hh>

#include <oln/level/fill.hh>

namespace oln {
  namespace tarjan {

    struct EMPTY_CLASS
    {};

    template< class _I, class aux_data_t>
    struct tarjan_set
    {
      typedef Point(_I) point_t;
      typedef Value(_I) data_t;
      typedef Concrete(_I) image_t;

      typedef typename mute<_I, point_t>::ret ima_parent_t;
      typedef typename mute<_I, aux_data_t>::ret ima_aux_data_t;

    public:
      tarjan_set(const image_t & ima) : input(ima)
      {
 	parent = typename mute<_I, point_t>::ret(ima.size());
 	level::fill(parent, INACTIVE());

	border::adapt_assign(parent, 1, INACTIVE());

//    	aux_data = typename mute<_I, aux_data_t>::ret(ima.size());
      }


      // ACTIVE and INACTIVE are defined with a hook to be static
      // and initialized ionly once.
      static const point_t & ACTIVE()
      {
	static struct foo_def{
	  point_t elt;
	  foo_def()
	  {
	    const unsigned dim = point_t::dim;
	    for (unsigned i = 0; i < dim; ++i )
	      elt.nth(i) = -1;
	  }
	} tmp;

	return tmp.elt;
      }

      static const point_t &INACTIVE()
      {
	static struct foo_def{
	  point_t elt;
	  foo_def() {
	    const unsigned dim = point_t::dim;
	    for (unsigned i = 0; i < dim; ++i )
	      elt.nth(i) = -2;
	  }
	} tmp;

	return tmp.elt;
      }


      void make_set(const point_t& x)
      {
	precondition(parent[x] == INACTIVE());
	parent[x] = ACTIVE();
// 	aux_data[x] = 1;
	// FIXME stuff with aux_data
      }

      void link(const point_t& x, const point_t& y)
      {
	//    delete aux_data[x];
// 	aux_data[y] += aux_data[x];
	parent[x] = y;
      }

      unsigned int attribute(const point_t& x)
      {
	precondition(parent[x] == ACTIVE());
	precondition(aux_data[x] != 0);
	return aux_data[x];
      }

      bool equiv(const point_t& x, const point_t& y)
      {
// 	precondition((parent[x] == ACTIVE()) || (parent[x] == INACTIVE()));
// 	precondition((parent[y] == ACTIVE()) || (parent[y] == INACTIVE()));

// 	// FIXME FAKE return TRUE
//  	return true;
  	return ( (input[x] == input[y]));// || (attribute(x) < 5000));
// 	return ( (input[x] == input[y]) || (parent[x] == ACTIVE()) );
      }


      void uni(const point_t& n, const point_t& p)
      {
	point_t r = find_root(n);
	if (r != p)
	  {
 	    if (equiv(r,p))
	      link(r,p);
	    else
	      {
//  		aux_data[p] = 5000;
// 		parent[p] = INACTIVE();
	      }
	  }
      }

      const point_t & find_root(const point_t& x)
      {
	//FIXME do it iteratively
	if ((parent[x] != ACTIVE()) && (parent[x] != INACTIVE()))
	  {
	    parent[x] = find_root(parent[x]);
	    return parent[x];
	  }
	else
	  return x;
      }


      const point_t & sure_find_root(const point_t& x)
      {
	if ((parent[x] != ACTIVE()) && (parent[x] != INACTIVE()))
	  return parent[x];
	else
	  return x;
      }

      const bool is_root(const point_t& x)
      {
	if ((parent[x] != ACTIVE()) && (parent[x] != INACTIVE()))
	  return false;
	return true;
      }

      const image_t &input;
      ima_parent_t parent;
      ima_aux_data_t aux_data;
    };

  } // end tarjan
} // end oln

#endif // ! UNION_HH
