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

#ifndef OLENA_APPLI_ASTRO_TREE_COHERENCE_CHECKS
# define OLENA_APPLI_ASTRO_TREE_COHERENCE_CHECKS

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

    namespace coherence_check {

      /**
       ** Check if tree starts from an unique root point.
       **
       ** \param I Type of maxtree's image.
       **
       ** \arg tree Maxtree structure to check.
       **
       ** Check if the maxtree built has an unique starting
       ** point (global root point)
       **
       ** \warning Maxtree should have already been computed.
       **
       ** \code
       ** Algorithm used :
       ** The count of points that are parent of themselves
       ** should be exactly one.
       ** \endcode
       **
       ** \return true if tree is ok.
       **
       */
      template<typename I>
      bool
      global_root_unicity(oln::appli::astro::clean<I>& tree)
      {
	typedef oln_type_of(I, point) point_type;
	int count = 0;
	oln_type_of(I, fwd_piter) p(tree.input.size());
	for_all_p(p)
	  {
	    assert(tree.input.hold(p));
	    if (tree.parent[p] == p)
	      ++count;
	  }
	assert(count == 1);
	if (count == 1)
	  return true;
	else
	  return false;
      }

      /**
       ** Check if parent relashionship is coherent in the tree.
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
       ** for all points (p) from input
       **   check if children's parent is (p)
       **
       ** \endcode
       **
       ** \return true if relationships are ok.
       **
       */
      template<typename I>
      bool
      parent_relationship_integrity(oln::appli::astro::clean<I>& tree)
      {
	typedef oln_type_of(I, point) point_type;
	oln_type_of(I, fwd_piter) p(tree.input.size());
	for_all_p(p)
	  {
	    std::vector<point_type> child = tree.children[p];
	    typename std::vector<point_type>::const_iterator pchild;
	    for (pchild = child.begin();
		 pchild != child.end();
		 pchild++)
	      {
		assert(tree.parent[*pchild] == p);
		if (tree.parent[*pchild] != p)
		  return false;
	      }
	  }
	return true;
      }

      /**
       ** Check if children relashionship is coherent in the tree.
       **
       ** \param I Exact type of maxtree's image.
       **
       ** \arg tree maxtree structure to check.
       **
       ** \warning Maxtree should have already been computed.
       **
       ** \code
       ** Algorithm used :
       **
       ** For all points (p) of input
       **   if (p) is not the root
       **     search for parent (par) of p
       **     ensure (p) is contained in children of (par)
       **   else
       **     // nothing, root parent of hitself
       **
       ** \endcode
       **
       ** \return true if relationships are ok.
       **
       */
      template<typename I>
      bool
      children_relationship_integrity(oln::appli::astro::clean<I>& tree)
      {
	typedef oln_type_of(I, point) point_type;
	oln_type_of(I, fwd_piter) p(tree.input.size());
	for_all_p(p)
	  {
	    point_type par = tree.parent[p];
	    bool found = false;
	    if (par != p) // special case for the root
	      {
		std::vector<point_type> child = tree.children[par];
		typename std::vector<point_type>::const_iterator pchild;
		for (pchild = child.begin();
		     pchild != child.end() and not found;
		     pchild++)
		  {
		    if (*pchild == p)
		      found = true;
		  }
		assert(found == true);
		if (not found)
		  return false;
	      }
	  }
	return true;
      }

      /**
       ** Check if there is no recursion between parent and children.
       **
       ** \param I Exact type of maxtree's image.
       **
       ** \arg tree maxtree structure to check.
       **
       ** A node cannot be children of hitelf, this check if this
       ** property is satisfied.
       **
       ** \warning Maxtree should have already been computed.
       **
       ** \code
       ** Algorithm used :
       **
       ** For all points (p) of input
       **   for all children (c) of (p)
       **     ensure (c) is not (p)
       **
       ** \endcode
       **
       ** \return true if there is no recursion.
       **
       */
      template<typename I>
      bool
      no_children_recursion_check(oln::appli::astro::clean<I>& tree)
      {
	typedef oln_type_of(I, point) point_type;
	oln_type_of(I, fwd_piter) p(tree.input.size());
	for_all_p(p)
	  {
	    std::vector<point_type> child = tree.children[p];
	    typename std::vector<point_type>::const_iterator pchild;
	    for (pchild = child.begin();
		 pchild != child.end();
		 pchild++)
	      {
		assert(*pchild != p);
		if (*pchild == p)
		  return false;
	      }
	  }
	return true;
      }


      /**
       ** Check if maxtree construction was well done.
       **
       ** \param I Exact type of maxtree's image.
       **
       ** \arg tree maxtree structure to check.
       **
       ** The maxtree structure is based on Tarjan's Union-Find
       ** algorithm which works on decreasing point levels.
       ** This property guarantees that leafs are seen first in
       ** first pass order. The check exists to ensure this.
       **
       ** \warning Maxtree should have already been computed.
       **
       ** \code
       ** Algorithm used :
       **
       ** For all points (p) in decreasing order
       **   ensure parent of (p) was not seen before
       **   for all children (c) of (p)
       **     ensure they were seen before
       **   mark (p) as seen
       **
       ** \endcode
       **
       ** \return true if tree is good.
       **
       */
      template<typename I>
      bool
      check_child_seen_before_parent(oln::appli::astro::clean<I>& tree)
      {
	typedef oln_type_of(I, point) point_type;
	oln_ch_concrete_type(I, bool) seen(tree.input.size());
	level::fill(seen, false);
	typename std::vector<std::vector<point_type> >::reverse_iterator pvect;
	typename std::vector<point_type>::const_iterator p;
	for (pvect = tree.S.rbegin(); pvect != tree.S.rend(); pvect++)
	  {
	    for (p = pvect->begin(); p != pvect->end(); p++)
	      {
		const std::vector<point_type> children = tree.children_get(*p);
		typename std::vector<point_type>::const_iterator pchild;
		assert(seen[tree.parent[*p]] == false);
		for (pchild = children.begin();
		     pchild != children.end();
		     pchild++)
		  {
		    assert(seen[*pchild] == true);
		    if (seen[*pchild] != true)
		      return false;
		  }
		seen[*p] = true;
	      }
	  }
	return true;
      }

      /**
       ** Check if all points of the image are contained
       ** in the maxtree.
       **
       ** \param I Exact type of maxtree's image.
       **
       ** \arg tree maxtree structure to check.
       **
       ** Maxtree should cover the whole area of the base image.
       **
       ** \warning Maxtree should have already been computed.
       **
       ** \code
       ** Algorithm used :
       **
       ** Tree is traversed using breadth-first traversal with a queue.
       ** Every point is marked, at the end all the points must
       ** have been marked.
       **
       ** \endcode
       **
       ** \return True if all points were marked.
       **
       */
      template<typename I>
      bool
      complete_image_coverage(oln::appli::astro::clean<I>& tree)
      {
	typedef oln_type_of(I, point) point_type;
	oln_ch_concrete_type(I, bool) seen(tree.input.size());
	level::fill(seen, false);
	std::queue<point_type> q;
	q.push(tree.find_root(point_type(0,0)));
	while (not q.empty())
	  {
	    point_type &p = q.front();
	    q.pop();
	    const std::vector<point_type> children = tree.children_get(p);
	    typename std::vector<point_type>::const_iterator pchild;
	    assert(seen[p] == false);
	    seen[p] = true;
	    for (pchild = children.begin();
		 pchild != children.end();
		 pchild++)
	      q.push(*pchild);
	  }

	// FIXME : real pw::check function
	//	return pw::check(pw::value(seen) == true);
	oln_type_of(I, fwd_piter) p(seen.size());
	for_all_p (p)
	  if (not seen[p])
	    return false;
	return true;
	// end FIXME
      }


    } // end of namespace coherence_check

  } // end of namespace maxtree

} // end of namespace oln

#endif // ! OLENA_APPLI_ASTRO_TREE_COHERENCE_CHECKS
