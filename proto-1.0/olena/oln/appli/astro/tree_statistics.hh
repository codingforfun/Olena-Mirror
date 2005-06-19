// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_APPLI_ASTRO_TREE_STATISTICS
# define OLENA_APPLI_ASTRO_TREE_STATISTICS

# include <mlc/any.hh>

# include <oln/core/abstract/image_entry.hh>
# include <oln/core/ch_value_type.hh>
# include <oln/level/fill.hh>

# include <oln/core/pw/check.hh>
# include <oln/appli/astro/clean.hh>
# include <oln/basics.hh>

# include <queue>

namespace oln {

  namespace maxtree {

    namespace statistics {

      /**
       ** Counts the number of leafs in the tree.
       ** 
       ** \param I Type of maxtree's image.
       **
       ** \arg tree Maxtree structure to check.
       **
       ** \warning Maxtree should have already been computed.
       **
       ** \code
       ** Algorithm used :
       **
       ** Tree is traversed using breadth-first traversal with a queue.
       ** Increments count on leaf (no children).
       ** 
       ** \endcode
       **
       ** \return Number of leafs (terminal nodes).
       **
       */
      template<typename I>
      int leaf_count(oln::appli::astro::clean<I>& tree)
      {
	typedef oln_type_of(I, point) point_type;
	std::queue<point_type> q;
	q.push(tree.find_root(point_type(0,0)));
	int count = 0;
	while (not q.empty())
	  {
	    point_type &p = q.front();
	    q.pop();
	    const std::vector<point_type> children = tree.children_get(p);
	    typename std::vector<point_type>::const_iterator pchild;
	    if (children.empty())
	      ++count;
	    else
	      for (pchild = children.begin();
		   pchild != children.end();
		   pchild++)
		q.push(*pchild);
	  }
	return count;
      }

      /**
       ** Counts the number of (internal) nodes in the tree.
       ** 
       ** \param I Type of maxtree's image.
       **
       ** \arg tree Maxtree structure to check.
       **
       ** \warning Maxtree should have already been computed.
       **
       ** \code
       ** Algorithm used :
       **
       ** Number of points - leafs count
       ** 
       ** \endcode
       **
       ** \return Number of internal nodes.
       **
       */
      template<typename I>
      int node_count(oln::appli::astro::clean<I>& tree)
      {
	return tree.input.width() * tree.input.height()
	  - leaf_count(tree);
      }

      /**
       ** Makes the children per node average.
       ** 
       ** \param I Type of maxtree's image.
       **
       ** \arg tree Maxtree structure to check.
       **
       ** \warning Maxtree should have already been computed.
       **
       ** \code
       ** Algorithm used :
       **
       ** Tree is traversed using breadth-first traversal with a queue.
       ** Counts the children and nodes, The average is 
       ** children_count / node_count.
       ** 
       ** \endcode
       **
       ** \return Children per node average.
       **
       */
      template<typename I>
      float children_average(oln::appli::astro::clean<I>& tree)
      {
	typedef oln_type_of(I, point) point_type;
	std::queue<point_type> q;
	q.push(tree.find_root(point_type(0,0)));
	double node_count = 0;
	double children_count = 0;
	while (not q.empty())
	  {
	    point_type &p = q.front();
	    q.pop();
	    ++node_count;
	    const std::vector<point_type> children = tree.children_get(p);
	    typename std::vector<point_type>::const_iterator pchild;
	    for (pchild = children.begin();
		 pchild != children.end();
		 pchild++)
	      {
		q.push(*pchild);
		++children_count;
	      }
	  }
	return children_count / node_count;
      }

      /**
       ** Max depth of tree.
       ** 
       ** \param I Type of maxtree's image.
       **
       ** \arg tree Maxtree structure to check.
       **
       ** \warning Maxtree should have already been computed.
       **
       ** This algorith is not the best way to compute max depth!
       **
       **
       ** \return Maximal depth of tree.
       **
       */
      template<typename I>
      int depth(oln::appli::astro::clean<I>& tree)
      {
	typedef oln_type_of(I, point) point_type;
	oln_type_of(I, fwd_piter) p(tree.input.size());
	int max_depth = 0;
	for_all_p(p)
	  {
	    const std::vector<point_type> children = tree.children_get(p);
	    if (children.empty())
	      {
		point_type q = p;
		int depth = 0;
		while (tree.parent[q] != q)
		  {
		    ++depth;
		    q = tree.parent[q];
		  }
		max_depth = max_depth < depth ? depth : max_depth;
	      }
	  }
	return max_depth;
      }

      /**
       ** Graphical tree output with Dotty
       ** (http://www.research.att.com/sw/tools/graphviz/)
       **
       ** \param I Type of maxtree's image.
       **
       ** \arg tree Maxtree structure to check.
       ** \arg filename Output will be written to the file.
       **
       ** \warning Maxtree should have already been computed.
       **
       **
       **
       */
      template<typename I>
      void dotty_output(oln::appli::astro::clean<I>& tree, const std::string& filename)
      {
	typedef oln_type_of(I, point) point_type;
	std::queue<point_type> q;
	q.push(tree.find_root(point_type(0,0)));


	std::ofstream out(filename.c_str());

	out << "digraph MaxtreeRepresentation {" << std::endl;

	oln_type_of(I, fwd_piter) p(tree.input.size());
	for_all_p(p)
	  {
	    out << '"' << p << "\";" << std::endl;
	  }
	while (not q.empty())
	  {
	    point_type &p = q.front();
	    q.pop();
	    const std::vector<point_type> children = tree.children_get(p);
	    typename std::vector<point_type>::const_iterator pchild;
	    for (pchild = children.begin();
		 pchild != children.end();
		 pchild++)
	      {
		q.push(*pchild);
		out << '"' << p << "\" -> \"" << *pchild << "\";" << std::endl;
	      }
	  }
	out << "}" << std::endl;
	out.close();
      }

    } // end of namespace statistics
    
  } // end of namespace maxtree

} // end of namespace oln

#endif // ! OLENA_APPLI_ASTRO_TREE_STATISTICS
