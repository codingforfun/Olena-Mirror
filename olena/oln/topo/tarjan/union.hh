// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_TOPO_TARJAN_UNION_HH
# define OLENA_TOPO_TARJAN_UNION_HH

#include <oln/basics2d.hh>

#include <oln/level/fill.hh>

namespace oln {

  namespace topo {
    /// \brief FIXME: depreciated, use oln::morpho::tarjan
    namespace tarjan {

      struct empty_class
      {};

      /*! Tarjan set.
      **
      ** \todo FIXME: Obsolete since only obsolete::flat_zone use it.
      */
      template< class I, class aux_data_type>
      struct tarjan_set
      {
	typedef oln_point_type(I) point_type;
	typedef oln_value_type(I) data_type;
	typedef oln_concrete_type(I) image_type;

	typedef typename mute<I, point_type>::ret ima_parent_type;
	typedef typename mute<I, aux_data_type>::ret ima_aux_data_type;

      public:
	tarjan_set(const image_type & ima) : input(ima)
	{
	  parent = typename mute<I, point_type>::ret(ima.size());
	  level::fill(parent, inactive());

	  parent.border_adapt_assign(1, inactive());
	}

	// FIXME: these macros do not respect the coding style!

	// active and inactive are defined with a hook to be static
	// and initialized only once.
	static const point_type&
	active()
	{
	  static struct foo_def{
	    point_type elt;
	    foo_def()
	    {
	      const unsigned dim = point_type::dim;
	      for (unsigned i = 0; i < dim; ++i )
		elt.nth(i) = -1;
	    }
	  } tmp;

	  return tmp.elt;
	}

	static const point_type&
	inactive()
	{
	  static struct foo_def {
	    point_type elt;
	    foo_def() {
	      const unsigned dim = point_type::dim;
	      for (unsigned i = 0; i < dim; ++i )
		elt.nth(i) = -2;
	    }
	  } tmp;

	  return tmp.elt;
	}

	void
	make_set(const point_type& x)
	{
	  precondition(parent[x] == inactive());
	  parent[x] = active();
	}

	void
	link(const point_type& x, const point_type& y)
	{
	  parent[x] = y;
	}

	unsigned int
	attribute(const point_type& x)
	{
	  precondition(parent[x] == active());
	  precondition(aux_data[x] != 0);
	  return aux_data[x];
	}

	bool
	equiv(const point_type& x, const point_type& y)
	{
	  return ( (input[x] == input[y]));
	}


	void
	uni(const point_type& n, const point_type& p)
	{
	  point_type r = find_root(n);
	  if (r != p && equiv(r,p))
	    link(r,p);
	}

	const point_type&
	find_root(const point_type& x)
	{
	  //FIXME do it iteratively
	  if ((parent[x] != active()) && (parent[x] != inactive()))
	    {
	      parent[x] = find_root(parent[x]);
	      return parent[x];
	    }
	  else
	    return x;
	}

	const point_type&
	sure_find_root(const point_type& x)
	{
	  if ((parent[x] != active()) && (parent[x] != inactive()))
	    return parent[x];
	  else
	    return x;
	}

	const bool
	is_root(const point_type& x)
	{
	  if ((parent[x] != active()) && (parent[x] != inactive()))
	    return false;
	  return true;
	}

	const image_type &input;
	ima_parent_type parent;
	ima_aux_data_type aux_data;
      };

    } // end of namespace tarjan

  } // end of namespace topo

} // end of namespace oln

#endif // ! OLENA_TOPO_TARJAN_UNION_HH
