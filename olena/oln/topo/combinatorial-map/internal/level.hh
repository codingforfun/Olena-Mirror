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

#ifndef OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_LEVEL_HH
# define OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_LEVEL_HH

# include <iostream>
# include <vector>

namespace oln {

  namespace topo {

    namespace combinatorial_map {

      namespace internal {

	template <class U>
	struct node
	{
	  node() : fchild(0), lb(0), rb(0), father(0) {}

	  U fchild, lb, rb, father;
	};

	template <class U>
	class level
	{
	public:
	  level(unsigned n) : _tree(n+1)
	  {
	    assertion(n);
	  }

	  U father(const U & child) const
	  {
	    assertion(child < _tree.size());

	    return _tree[child].father;
	  }

	  U children(const U & father) const
	  {
	    static unsigned f = 0;
	    static unsigned last = 0;

	    assertion(father < _tree.size());

	    if (f != father)
	      {
		f = father;
		last = _tree[f].fchild;
	      }
	    else
	      last = (!last || _tree[last].rb == _tree[f].fchild) ?
		0 : _tree[last].rb;

	    return last;
	  }

	  void insert(const U & father, const U & child)
	  {
	    node<U> & f = _tree[father];
	    node<U> & c = _tree[child];

	    if (!f.lb)
	      f.lb = f.rb = father;

	    unsigned fchild = f.fchild;

	    if (fchild)
	      {
		unsigned tail = _tree[fchild].lb;

		_tree[tail].rb = child;
		c.lb = tail;
		_tree[fchild].lb = child;
		c.rb = fchild;
	      }
	    else
	      f.fchild = c.lb = c.rb = child;

	    c.father = father;
	  }

	  void merge(const U & l1, const U & l2)
	  {
	    node<U> & a = _tree[l1];
	    node<U> & b = _tree[l2];

	    // assert l2 is not the first child of its father
	    if (_tree[b.father].fchild == l2)
	      _tree[b.father].fchild = (l2 != b.rb) ? b.rb : 0;

	    // append l2' children to l1 ones
	    if (b.fchild)
	      {
		// FIXME: find a way to get this constant time
		unsigned l = 0;
		while ((l = children(l2)))
		  _tree[l].father = l1;

		if (a.fchild)
		  {
		    _tree[_tree[a.fchild].lb].rb = b.fchild;
		    _tree[b.fchild].lb = _tree[a.fchild].lb;

		    _tree[a.fchild].lb = _tree[b.fchild].lb;
		    _tree[b.fchild].rb = a.fchild;
		  }
		else
		  a.fchild = b.fchild;
	      }

	    // pop up l2 from the list child
	    _tree[b.rb].lb = b.lb;
	    _tree[b.lb].rb = b.rb;

	    b.fchild = b.lb = b.rb = b.father = 0;
	  }

	public:
	  std::ostream & print(std::ostream & ostr) const
	  {
	    for (unsigned i = 1; i < _tree.size(); ++i)
	      ostr << "father(" << i << ") = " << father(i) << std::endl;

	    ostr << std::endl;

	    for (unsigned i = 1; i < _tree.size(); ++i)
	      {
		ostr << "children(" << i << ") = ";

		unsigned l = children(i);

		if (l)
		  {
		    ostr << l;
		    while ((l = children(i)))
		      ostr << ", " << l;
		    ostr << std::endl;
		  }
		else
		  ostr << "none" << std::endl;
	      }

	    return ostr;
	  }

	private:
	  std::vector< node<U> > _tree;
	};

      } // end internal

    } // end combinatorial_map

  } // end topo

} // end oln

template<class U>
inline std::ostream &
operator<<(std::ostream & ostr,
	   const oln::topo::combinatorial_map::internal::level<U> & l);

# include <oln/topo/combinatorial-map/internal/level.hxx>

#endif // OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_LEVEL_HH
